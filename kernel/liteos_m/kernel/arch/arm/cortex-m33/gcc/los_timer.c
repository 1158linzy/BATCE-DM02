/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "los_config.h"
#include "los_tick.h"
#include "los_arch_interrupt.h"
#include "los_timer.h"
#include "los_context.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */



/* ****************************************************************************
Function    : HalTickStart
Description : Configure Tick Interrupt Start
Input       : none
output      : none
return      : LOS_OK - Success , or LOS_ERRNO_TICK_CFG_INVALID - failed
**************************************************************************** */
WEAK UINT32 HalTickStart(OS_TICK_HANDLER *handler)
{
    UINT32 ret;

    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return LOS_ERRNO_TICK_CFG_INVALID;
    }

#if (LOSCFG_USE_SYSTEM_DEFINED_INTERRUPT == 1)
#if (OS_HWI_WITH_ARG == 1)
    OsSetVector(SysTick_IRQn, (HWI_PROC_FUNC)handler, NULL);
#else
    OsSetVector(SysTick_IRQn, (HWI_PROC_FUNC)handler);
#endif
#endif

    g_sysClock = OS_SYS_CLOCK;
    g_cyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_ullTickCount = 0;

    ret = SysTick_Config(g_cyclesPerTick);
    if (ret == 1) {
        return LOS_ERRNO_TICK_PER_SEC_TOO_SMALL;
    }

    return LOS_OK;
}

/* ****************************************************************************
Function    : HalSysTickCurrCycleGet
Description : Get System cycle count
Input       : none
output      : none
return      : hwCycle --- the system cycle count
**************************************************************************** */
LITE_OS_SEC_TEXT_MINOR UINT32 HalSysTickCurrCycleGet(VOID)
{
    UINT32 hwCycle;
    UINTPTR intSave;

    intSave = LOS_IntLock();
    hwCycle = SysTick->VAL;

    /* tick has come, but may interrupt environment, not counting the Tick interrupt response, to do +1 */
    if ((SCB->ICSR & TICK_CHECK) != 0) {
        hwCycle = SysTick->VAL;
        hwCycle += g_cyclesPerTick;
    }

    LOS_IntRestore(intSave);

    return hwCycle;
}

/* ****************************************************************************
Function    : HalGetCpuCycle
Description : Get System cycle count
Input       : none
output      : cntHi  --- CpuTick High 4 byte
              cntLo  --- CpuTick Low 4 byte
return      : none
**************************************************************************** */
LITE_OS_SEC_TEXT_MINOR VOID HalGetCpuCycle(UINT32 *cntHi, UINT32 *cntLo)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINTPTR intSave;

    intSave = LOS_IntLock();

    swTick = g_ullTickCount;
    hwCycle = SysTick->VAL;

    /* tick has come, but may interrupt environment, not counting the Tick interrupt response, to do +1 */
    if ((SCB->ICSR & TICK_CHECK) != 0) {
        hwCycle = SysTick->VAL;
        swTick++;
    }

    cycle = (((swTick) * g_cyclesPerTick) + (g_cyclesPerTick - hwCycle));

    *cntHi = cycle >> SHIFT_32_BIT;
    *cntLo = cycle & CYCLE_CHECK;

    LOS_IntRestore(intSave);

    return;
}

/* ****************************************************************************
Function    : HalGetSystickCycle
Description : Get Sys tick cycle count
Input       : none
output      : cntHi  --- SysTick count High 4 byte
              cntLo  --- SysTick count Low 4 byte
return      : none
**************************************************************************** */
LITE_OS_SEC_TEXT_MINOR VOID HalGetSystickCycle(UINT32 *cntHi, UINT32 *cntLo)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINTPTR intSave;
    UINT32 systickLoad;
    UINT32 systickCur;

    intSave = LOS_IntLock();

    swTick = g_ullTickCount;

    systickLoad = SysTick->LOAD;
    systickCur = SysTick->VAL;
    if (systickLoad < systickCur) {
        LOS_IntRestore(intSave);
        return;
    }
    hwCycle = systickLoad - systickCur;

    /* tick has come, but may interrupt environment, not counting the Tick interrupt response, to do +1 */
    if ((SCB->ICSR & TICK_CHECK) != 0) {
        hwCycle = systickLoad - systickCur;
        swTick++;
    }

    cycle = hwCycle + swTick * systickLoad;
    *cntHi = cycle >> SHIFT_32_BIT;
    *cntLo = cycle & CYCLE_CHECK;

    LOS_IntRestore(intSave);

    return;
}

static BOOL g_sysSleepFlag = FALSE;

VOID HalTickLock(VOID)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

VOID HalTickUnlock(VOID)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

BOOL HalGetSysSleepFlag(VOID)
{
	return g_sysSleepFlag;
}

VOID HalClearSysSleepFlag(VOID)
{
	g_sysSleepFlag = FALSE;
}

VOID HalEnterSleep(LOS_SysSleepEnum sleep)
{
	__DSB();
	__WFI();
	__ISB();
}

WEAK VOID HalDelay(UINT32 ticks)
{

}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

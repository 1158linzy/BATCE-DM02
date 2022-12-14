/*
 * Copyright (c) 2013-2020, Huawei Technologies Co., Ltd. All rights reserved.
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

#include "soc.h"

.global  HalEnableIRQ
.global  HalDisableIRQ
.global  HalIntLock
.global  HalIntUnLock
.global  HalIntRestore
.global  HalStartToRun
.global  HalTaskSchedule
.global  HalTaskSwitch
.extern  __irq_stack_top
.extern HalTaskScheduleCheck
.extern printk
.equ        OS_TASK_STATUS_RUNNING,     0x0010
.equ        OS_TASK_STATUS_NOT_RUNNING, 0xFFEF
.section .interrupt.text

.macro PUSH_CALLER_REG
   addi sp, sp, -(32 * REGBYTES)
   SREG  t6, 2 * REGBYTES(sp)
   SREG  t5, 3 * REGBYTES(sp)
   SREG  t4, 4 * REGBYTES(sp)
   SREG  t3, 5 * REGBYTES(sp)
   SREG  t2, 6 * REGBYTES(sp)
   SREG  t1, 7 * REGBYTES(sp)
   SREG  t0, 8 * REGBYTES(sp)
   SREG  a7, 18 * REGBYTES(sp)
   SREG  a6, 19 * REGBYTES(sp)
   SREG  a5, 20 * REGBYTES(sp)
   SREG  a4, 21 * REGBYTES(sp)
   SREG  a3, 22 * REGBYTES(sp)
   SREG  a2, 23 * REGBYTES(sp)
   SREG  a1, 24 * REGBYTES(sp)
   SREG  a0, 25 * REGBYTES(sp)
   SREG  ra, 31 * REGBYTES(sp)
.endm

.macro POP_CALLER_REG
   LREG  t6, 2 * REGBYTES(sp)
   LREG  t5, 3 * REGBYTES(sp)
   LREG  t4, 4 * REGBYTES(sp)
   LREG  t3, 5 * REGBYTES(sp)
   LREG  t2, 6 * REGBYTES(sp)
   LREG  t1, 7 * REGBYTES(sp)
   LREG  t0, 8 * REGBYTES(sp)
   LREG  a7, 18 * REGBYTES(sp)
   LREG  a6, 19 * REGBYTES(sp)
   LREG  a5, 20 * REGBYTES(sp)
   LREG  a4, 21 * REGBYTES(sp)
   LREG  a3, 22 * REGBYTES(sp)
   LREG  a2, 23 * REGBYTES(sp)
   LREG  a1, 24 * REGBYTES(sp)
   LREG  a0, 25 * REGBYTES(sp)
   LREG  ra, 31 * REGBYTES(sp)
   addi sp, sp, 32 * REGBYTES
.endm

.macro PUSH_CALLEE_REG
    SREG  s11, 9 * REGBYTES(sp)
    SREG  s10, 10 * REGBYTES(sp)
    SREG  s9, 11 * REGBYTES(sp)
    SREG  s8, 12 * REGBYTES(sp)
    SREG  s7, 13 * REGBYTES(sp)
    SREG  s6, 14 * REGBYTES(sp)
    SREG  s5, 15 * REGBYTES(sp)
    SREG  s4, 26 * REGBYTES(sp)
    SREG  s3, 27 * REGBYTES(sp)
    SREG  s2, 28 * REGBYTES(sp)
    SREG  s1, 29 * REGBYTES(sp)
    SREG  s0, 30 * REGBYTES(sp)
.endm

.macro POP_ALL_REG
    LREG  t6, 2 * REGBYTES(sp)
    LREG  t5, 3 * REGBYTES(sp)
    LREG  t4, 4 * REGBYTES(sp)
    LREG  t3, 5 * REGBYTES(sp)
    LREG  t2, 6 * REGBYTES(sp)
    LREG  t1, 7 * REGBYTES(sp)
    LREG  t0, 8 * REGBYTES(sp)
    LREG  s11, 9 * REGBYTES(sp)
    LREG  s10, 10 * REGBYTES(sp)
    LREG  s9, 11 * REGBYTES(sp)
    LREG  s8, 12 * REGBYTES(sp)
    LREG  s7, 13 * REGBYTES(sp)
    LREG  s6, 14 * REGBYTES(sp)
    LREG  s5, 15 * REGBYTES(sp)
    LREG  a7, 18 * REGBYTES(sp)
    LREG  a6, 19 * REGBYTES(sp)
    LREG  a5, 20 * REGBYTES(sp)
    LREG  a4, 21 * REGBYTES(sp)
    LREG  a3, 22 * REGBYTES(sp)
    LREG  a2, 23 * REGBYTES(sp)
    LREG  a1, 24 * REGBYTES(sp)
    LREG  a0, 25 * REGBYTES(sp)
    LREG  s4, 26 * REGBYTES(sp)
    LREG  s3, 27 * REGBYTES(sp)
    LREG  s2, 28 * REGBYTES(sp)
    LREG  s1, 29 * REGBYTES(sp)
    LREG  s0, 30 * REGBYTES(sp)
    LREG  ra, 31 * REGBYTES(sp)
    addi  sp, sp, 32 * REGBYTES
.endm

HalStartToRun:
// disable interrupts
    csrci   mstatus, RISCV_MSTATUS_MIE

// indicate that sheduler is ON by setting g_taskScheduled=1
    la      t0, g_taskScheduled
    li      t1, 0x1
    sw      t1, 0x0(t0)

    j       SwitchNewTask

HalTaskSchedule:
    la      t0, g_intCount
    lw      t1, 0(t0)
    bne     t1, zero, 1f

    li      t0, RISCV_MSTATUS_MIE | RISCV_MSTATUS_MPIE
    csrrc   a1, mstatus, t0

    la      t0, g_losTask
    lw      t1, 0(t0)
    lw      t2, 4(t0)
    bne     t1, t2, 2f

    csrw    mstatus, a1
1:
    ret
2:
    addi    sp, sp, -(32 * REGBYTES)
    andi    a1, a1, RISCV_MSTATUS_MIE
    ori     a1, a1, 0x180
    slli    a1, a1, 0x4
    csrs    mstatus, a1
    csrw    mepc, ra

    j       SaveContextAndSwitchTask

HalTaskSwitch:
    la      t0, g_losTaskLock
    lw      t1, 0(t0)
    bgtz    t1, NotSwitch

    la      t0, g_losTask
    lw      t1, 0(t0)
    lw      t2, 4(t0)
    beq     t1, t2, NotSwitch

// stack pointer was passed by entry.s in register a2. store it in runTask structure
    csrr    sp, mscratch

SaveContextAndSwitchTask:
    csrr    t0, mstatus
    SREG    t0, 16 * REGBYTES(sp)
    csrr    t0, mepc
    SREG    t0, 17 * REGBYTES(sp)
    PUSH_CALLEE_REG

// Clear the task running bit of runTask.
    lh      t2, TASK_CB_STATUS(t1)
    li      t0, OS_TASK_STATUS_NOT_RUNNING
    and     t2, t2, t0
    sh      t2, TASK_CB_STATUS(t1)
    sw      sp, TASK_CB_KERNEL_SP(t1)

    call    HalTaskScheduleCheck

SwitchNewTask:
// copy newTask into runTask
    la      t0, g_losTask
    lw      t1, 0x4(t0)
    sw      t1, 0x0(t0)

// set the task running bit=1
    lh      t2, TASK_CB_STATUS(t1)
    ori     t2, t2, OS_TASK_STATUS_RUNNING
    sh      t2, TASK_CB_STATUS(t1)

// retireve stack pointer
    lw      sp, TASK_CB_KERNEL_SP(t1)

// enable global interrupts
    lw      t0, 16 * REGBYTES(sp)
    csrw    mstatus, t0

// retrieve the address at which exception happened
    lw      t0, 17 * REGBYTES(sp)
    csrw    mepc, t0

// retrieve the registers
    POP_ALL_REG

    mret

NotSwitch:
    csrr    sp, mscratch
// retrieve the registers
    POP_CALLER_REG
    mret

.section .text
HalDisableIRQ:
    li      t0, (RISCV_MSTATUS_MPIE | RISCV_MSTATUS_MIE) // mpie | mie
    csrrc   zero, mstatus, t0
    ret

HalEnableIRQ:
    csrsi  mstatus, RISCV_MSTATUS_MIE
    ret

HalIntLock:
    csrr    a0, mstatus           // return value
    li      t0, RISCV_MSTATUS_MIE   // mie
    csrrc   zero, mstatus, t0
    ret

HalIntUnLock:
    csrr    a0, mstatus           // return value
    li      t0, RISCV_MSTATUS_MIE   // mie
    csrrs   zero, mstatus, t0
    ret

HalIntRestore:
    csrw mstatus, a0
    ret

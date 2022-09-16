/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: OTA适配实现 (需设备厂商实现)
 */
#include "hilink_open_ota_adapter.h"
#include <stdint.h>
#include <stdio.h>
#include "hi_upg_api.h"
#include "hilink_open_type.h"
#include "securec.h"

typedef struct {
    /* 要升级的目标分区编号 */
    unsigned int targetIndex;
    /* 升级的偏移 */
    unsigned int fwBinOffset;
    /* 可升级区域的总大小 */
    unsigned int maxSize;
} UpgradeParam;

/* 记录升级相关的信息，比如开始地址，擦写块信息，可升级的最大区域 */
static UpgradeParam g_upgrade;

/*
 * Flash初始化
 * 返回值是true时，表示初始化正常
 * 返回值是false时，表示初始化异常
 */
bool HILINK_OtaAdapterFlashInit(void)
{
    if (HotaInit(0, 0) != 0) {
        printf("hota init fail.\r\n");
        return false;
    }
    HotaSetPackageType(0);

    if (memset_s(&g_upgrade, sizeof(UpgradeParam), 0, sizeof(UpgradeParam)) != EOK) {
        return false;
    }

    unsigned int index = 0;
    if (HotaGetUpdateIndex((hi_u8 *)&index) != 0) {
        printf("get upgrade index fail.\r\n");
        return false;
    }

    g_upgrade.targetIndex = index;

    unsigned int size = 0;
    if (hi_upg_get_max_file_len(HI_UPG_FILE_KERNEL, &size) != 0) {
        printf("get upgrade max file len fail.\r\n");
        return false;
    }
    g_upgrade.maxSize = size;

    return true;
}

/*
 * 判断需要升级的分区
 * 返回值是UPGRADE_FW_BIN1时，表示升级固件到分区1
 * 返回值是UPGRADE_FW_BIN2时，表示升级固件到分区2
 */
unsigned int HILINK_OtaAdapterGetUpdateIndex(void)
{
	return (g_upgrade.targetIndex == HI_UPG_FILE_FOR_AREA_A) ? UPGRADE_FW_BIN1 : UPGRADE_FW_BIN2;
}

/*
 * 擦除需要升级的分区
 * size表示需要擦除的分区大小
 * 返回值是RETURN_OK时，表示擦除成功
 * 返回值是RETURN_ERROR时，表示擦除失败
 */
int HILINK_OtaAdapterFlashErase(unsigned int size)
{
    (void)size;
    /* Hi3861在第一次写时自行擦除 */
    return HILINK_OK;
}

/*
 * 升级数据写入升级的分区
 * buf表示待写入数据
 * bufLen表示待写入数据的长度
 * 返回值是RETURN_OK时，表示写入成功
 * 返回值是RETURN_ERROR时，表示写入失败
 */
int HILINK_OtaAdapterFlashWrite(const unsigned char *buf, unsigned int bufLen)
{
    if (bufLen == 0) {
        return HILINK_OK;
    }

    if ((buf == NULL) || ((g_upgrade.fwBinOffset + bufLen) > g_upgrade.maxSize)) {
        return HILINK_ERR;
    }

    if (HotaWrite(buf, g_upgrade.fwBinOffset, bufLen) != 0) {
        printf("transmit fail.\r\n");
        return HILINK_ERR;
    }

    g_upgrade.fwBinOffset += bufLen;
    return HILINK_OK;
}

/*
 * 读取升级分区数据
 * offset表示读写偏移
 * buf表示输出数据的内存地址
 * bufLen表示输出数据的内存长度
 * 返回值是RETURN_OK时，表示读取成功
 * 返回值是RETURN_ERROR时，表示读取失败
 */
int HILINK_OtaAdapterFlashRead(unsigned int offset, unsigned char *buf, unsigned int bufLen)
{
    if ((buf == NULL) || (bufLen == 0)) {
        return HILINK_ERR;
    }

    if (HotaRead(offset, bufLen, buf) != 0) {
        printf("get ota content fail.\r\n");
        return HILINK_ERR;
    }

    return HILINK_OK;
}

/*
 * 分区升级结束
 * 返回值是true时，表示结束正常
 * 返回值是false时，表示结束异常
 */
bool HILINK_OtaAdapterFlashFinish(void)
{
    if (HotaHalSetBootSettings() != 0) {
        return false;
    }
    return true;
}

/* 获取升级区间最大长度 */
unsigned int HILINK_OtaAdapterFlashMaxSize(void)
{
	return g_upgrade.maxSize;
}

/*
 * 根据标志重启模组
 * flag表示重启标志
 * 当flag是RESTART_FLAG_NOW时，表示只有MCU升级时立即重启
 * 当flag是RESTART_FLAG_LATER时，表示有模组时切换分区后再重启
 */
void HILINK_OtaAdapterRestart(int flag)
{
    (void)flag;
    HotaHalRestart();
}

/*
 * 开始模组升级
 * type表示升级类型
 * 当type是UPDATE_TYPE_MANUAL时，表示本次升级流程是由用户主动发起的手动升级
 * 当type是UPDATE_TYPE_AUTO时，表示本次升级流程是经过用户同意的自动升级
 * 返回值是RETURN_OK时，表示处理成功，HiLink SDK将开始启动升级流程
 * 返回值是RETURN_ERROR时，表示处理不成功，HiLink SDK将终止本次升级流程
 * 注意：在手动场景场景下，HiLink SDK在接收到用户发出的升级指令后，将直接调用此接口；
 * 在自动升级场景下，当HiLink SDK在调用HilinkGetRebootFlag接口返回值是MODULE_CAN_REBOOT时，HiLink SDK将调用此接口。
 * 厂商可在此接口中完成和升级流程相关的处理。
 * 开机后10分钟到1小时内随机时间检测一次是否有新版本，之后以当前时间为起点，23小时加1小时内随机值周期性检测新版本。
 * 如果用户打开了自动升级开关，检测到有新版本并且是可以重启的情况下，就进行新版本的下载，下载完成后自动重启。
 * 自动升级流程可能在凌晨进行，因此厂商在实现升级流程相关功能时，确保在升级的下载安装固件和重启设备时避免对用户产生
 * 影响，比如发出声音，光亮等。
 */
int HILINK_OtaStartProcess(int type)
{
    return RETURN_OK;
}

/*
 * 模组升级结束
 * status表示升级结果
 * 当status是100时，表示升级成功
 * 当status不是100时，表示升级失败
 * 返回值是RETURN_OK时，表示处理成功，HiLink SDK将置升级标志或切换运行区标志
 * 返回值不是RETURN_OK时，表示处理不成功，HiLink SDK将终止本次升级流程
 * 注意：HiLink SDK在将固件写入到OTA升级区后，且完整性校验通过后，将调用厂商适配的此接口；
 * 厂商可在此接口中完成和升级流程相关的处理。
 * 开机后10分钟到1小时内随机时间检测一次是否有新版本，之后以当前时间为起点，23小时加1小时内随机值周期性检测新版本。
 * 如果用户打开了自动升级开关，检测到有新版本并且是可以重启的情况下，就进行新版本的下载，下载完成后自动重启。
 * 自动升级流程可能在凌晨进行，因此厂商在实现升级流程相关功能时，确保在升级的下载安装固件和重启设备时避免对用户产生
 * 影响，比如发出声音，光亮等；升级类型是否为自动升级可参考接口HilinkOtaStartProcess的参数type的描述。
 */
int HILINK_OtaEndProcess(int status)
{
    return RETURN_OK;
}

/*
 * 判断模组是否能立即升级并重启
 * 返回值是MODULE_CAN_REBOOT时，表示模组可以立即升级并重启，HiLink SDK将开始自动升级流程。
 * 返回值是MODULE_CANNOT_REBOOT时，表示模组不能立即升级并重启，HiLink SDK将不进行本次自动升级流程。
 * 注意：在用户同意设备可以自动升级的情况下，HiLink SDK调用此接口获取设备当前业务状态下，模组是否可以立即升级并重启的标志。
 * 只有当设备处于业务空闲状态时，接口才可以返回MODULE_CAN_REBOOT。
 * 当设备处于业务非空闲状态时，接口返回MODULE_CANNOT_REBOOT。
 */
int HILINK_GetRebootFlag(void)
{
    return MODULE_CAN_REBOOT;
}

/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: 此文件提供了升级相关的接口实现示例
 * Create: 2018-06-22
 * Notes: 其中升级相关功能的接口需要开发者或厂商进行实现;
 *        HiLink SDK在收到升级相关命令时会调用对应接口;
 *        请开发者仔细阅读文件中的注释说明和代码, 参考或修改实现。
 */
#include "hilink_ota.h"
#include <stdlib.h>
#include <string.h>
#include "hilink_osadapter.h"

#include "securec.h"
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>


/* 设备版本号长度(byte), 范围为[0, 64), 由厂商自定义 */
#define VERSION_LEN 16
/* 设备版本描述信息长度(byte), 范围为[0, 512), 由厂商自定义 */
#define VERSION_INTRO_LEN 64
/* 设备升级后重启延迟时间, 单位是秒(s), 由厂商自定义 */
#define OTA_REBOOT_TIME 60

/* 设备当前版本号 */
static char g_currentVersion[VERSION_LEN] = {0};
/* 设备升级任务句柄 */
static void *g_deviceOtaTask = NULL;

/* 需要开发者实现的升级相关函数begin */

/*
 * 获取升级服务器上最新版本信息.
 * 出参latestVersion返回获取的最新的版本号;
 * latestVersionLen为latestVersion数组长度;
 * 出参latestInfo返回获取的最新的版本描述信息;
 * latestIntroLen为latestIntro数组长度;
 * 返回值0表示操作成功;-1表示操作失败.
 * 注意: 版本号最大长度VERSION_LEN, 版本描述信息最大长度VERSION_INTRO_LEN.
 */
static int GetLatestVersionInfo(char *latestVersion, int latestVersionLen, char *latestIntro, int latestIntroLen)
{
    /*
     * 实现逻辑:
     * 1、连接升级服务器;
     * 2、获取最新固件版本信息，输出到出参latestVersion和latestIntro.
     */
    return 0;
}

/*
 * 获取设备当前版本号(到全局变量), 用于判断是否最新版本.
 * 返回值0表示操作成功;-1表示操作失败.
 * 注意: 版本号最大长度VERSION_LEN.
 */
static int GetCurrentVersion(void)
{
    /* 获取设备当前版本号到全局变量g_currentVersion */
    return 0;
}

/*
 * 设备升级的定时器，超时未升级完成，重试升级.
 * 参数action表示操作类型, 0表示关闭定时器; 1表示开启定时器.
 * 注意: (1) 需要厂商实现或调用timer接口, 并在超时回调函数中重试升级(建议采用3次重试机制);
 *       (2) 升级超时时间请根据设备实际升级时间自定义;
 *       (3) 升级完成或异常结束时, 记得及时关闭定时器.
 */
static void DeviceOtaTimer(int action)
{
    if (action == TIMER_START) {
        /* 在此处实现启动定时器 */
    } else if (action == TIMER_CLOSE) {
        /* 在此处实现关闭定时器 */
    } else {
        /* 非法参数 */
    }
}

/*
 * OTA升级任务, 实现设备的升级功能, 异步执行设备升级流程.
 * 返回值0表示操作成功;-1表示操作失败.
 * 注意: (1) 设备升级过程中同步调用hilink_ota_rpt_prg()接口上报实时升级进度;
 *       (2) 升级完成或异常结束时, 记得删除任务、关闭定时器;
 *       (3) 考虑到网络丢包, 建议进度progress=100上报3次以上, 保证APP可以收到;
 *       (4) 上报进度progress=100之后, 建议延迟2-3秒后再重启设备, 确保100%进度发送成功.
 */
static int DeviceOtaTask(void)
{
    /*
     * 设备升级流程由厂商实现，实现逻辑大体分为以下几个步骤供参考:
     *  1. 连接升级服务器;
     *  2. 下载升级包;
     *  3. 升级包校验;
     *  4. 升级包存入flash;
     *  5. 设备重启, 运行新版本固件.
     * 升级过程中同步调用hilink_ota_rpt_prg接口上报实时升级进度.
     */
    hilink_task_delete((const void *)&g_deviceOtaTask, NULL); /* 删除任务 */
    DeviceOtaTimer(TIMER_CLOSE); /* 关闭定时器 */
    return 0;
}

/* 需要开发者实现的升级相关函数end */

/* 以下接口已提供参考实现, 开发者可使用或参考修改 */

/*
 * 网关或云端向设备发送检测新版本或启动升级命令, HiLink SDK调用该接口检测新版本或启动升级.
 * 参数mode 0表示触发设备检测新版本; 1表示触发设备启动固件升级; 其他值为非法.
 * 返回值: OTA_NO_ERROR       0      无错误
 *         OTA_ARG_INVALID    (-12)  入参无效
 *         OTA_DETECT_FAILURE (-900) 触发固件检测升级失败
 *         OTA_START_FAILURE  (-901) 固件启动升级失败
 * 注意: 此函数为参考实现, 开发者可使用或参考修改, 函数中调用的创建升级任务接口的参数task优先级和
 *       task栈大小仅为参考值, 可根据实际情况修改.
 */


/*
 * 网关或云端向设备发送获取版本号命令, HiLink SDK会调用该接口获取设备最新版本号.
 * 参数version 设备最新版本号. 若当前固件版本已为最新版本时, *version返回NULL
 * 参数verLen 设备最新版本号长度, 范围为[0, 64). 若当前固件版本已为最新版本时, *verLen返回0
 * 返回值: OTA_NO_ERROR            0      无错误
 *         OTA_GET_VERSION_FAILURE (-902) 获取固件版本号失败
 * 注意: 此函数为参考实现, 开发者可使用或参考修改.
 */

/*
 * 网关或云端向设备发送获取版本描述信息命令时, HiLink SDK会调用该接口获取设备当前的版本描述信息
 * 参数introduction 设备最新版本描述信息. 若当前版本已为最新版本时, *introduction返回NULL
 * 参数introLen 设备最新版本描述信息长度, 范围为[0, 512). 若当前版本已为最新版本时, *introLen返回0
 * 返回值 OTA_NO_ERROR          0      无错误
 *        OTA_GET_INTRO_FAILURE (-903) 获取固件版本描述信息失败
 * 注意: 此函数为参考实现, 开发者可使用或参考修改
 */


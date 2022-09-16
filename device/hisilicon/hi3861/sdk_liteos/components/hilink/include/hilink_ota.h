/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: 设备升级相关定义和接口头文件
 * Create: 2018-06-22
 * Notes: 其中前三个接口是需要厂商实现的接口, 后两个接口为提供厂商调用的接口
 */
#ifndef _HILINK_OTA_H_
#define _HILINK_OTA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 升级相关接口返回值 */
#define OTA_NO_ERROR            0      /* 无错误 */
#define OTA_ARG_INVALID         (-12)  /* 无效的参数 */
#define OTA_DETECT_FAILURE      (-900) /* 触发固件升级检测失败 */
#define OTA_START_FAILURE       (-901) /* 固件启动升级失败 */
#define OTA_GET_VERSION_FAILURE (-902) /* 获取固件版本号失败 */
#define OTA_GET_INTRO_FAILURE   (-903) /* 获取固件描述信息 */

/* OTA检测命令 */
#define MODE_CHECK  0
/* OTA升级命令 */
#define MODE_UPDATE 1

/* 关闭定时器 */
#define TIMER_CLOSE 0
/* 开启定时器 */
#define TIMER_START 1

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
int hilink_ota_trig(int mode);

/*
 * 网关或云端向设备发送获取版本号命令, HiLink SDK会调用该接口获取设备最新版本号.
 * 参数version 设备最新版本号. 若当前固件版本已为最新版本时, *version返回NULL
 * 参数verLen 设备最新版本号长度, 范围为[0, 64). 若当前固件版本已为最新版本时, *verLen返回0
 * 返回值: OTA_NO_ERROR            0      无错误
 *         OTA_GET_VERSION_FAILURE (-902) 获取固件版本号失败
 * 注意: 此函数为参考实现, 开发者可使用或参考修改.
 */
int hilink_ota_get_ver(char **version, int *verLen);


#if (defined(SUPPORT_SUB_DEV_OTA) && defined(BRIDGE_BATCH_OPS))
/*
 * 网关或云端向桥设备发送获取版本号命令, HiLink SDK会调用该接口获取桥设备和桥下子设备可升级的最新版本号.
 * 参数version 为最新版本号列表. 若桥设备及桥下子设备固件版本均已为最新版本时, *version返回NULL
 * 数据格式  [{"prodId":"xxxx1","version":"1.0"},{"prodId":"xxxx2","version":"1.0"}]
 * 参数verLen 设备最新版本号长度. 若桥设备及桥下子设备固件版本均已为最新版本时, *verLen返回0
 * 返回值: OTA_NO_ERROR            0      无错误
 *         OTA_GET_VERSION_FAILURE (-902) 获取固件版本号失败
 * 注意: 此函数为参考实现, 开发者可使用或参考修改.
 */
int HilinkOtaGetBrgSubDevVer(char **version, int *verLen);
#endif /* BRIDGE_BATCH_OPS && SUPPORT_SUB_DEV_OTA */

/*
 * 网关或云端向设备发送获取版本描述信息命令时, HiLink SDK会调用该接口获取设备当前的版本描述信息
 * 参数introduction 设备最新版本描述信息. 若当前版本已为最新版本时, *introduction返回NULL
 * 参数introLen 设备最新版本描述信息长度, 范围为[0, 512). 若当前版本已为最新版本时, *introLen返回0
 * 返回值 OTA_NO_ERROR          0      无错误
 *        OTA_GET_INTRO_FAILURE (-903) 获取固件版本描述信息失败
 * 注意: 此函数为参考实现, 开发者可使用或参考修改
 */
int hilink_ota_get_intro(char **introduction, int *introLen);

/*
 * 设备调用该函数, 主动通知网关或云端固件有版本更新.
 * 返回0 无错误; 非0 上报固件升级版本信息失败
 * 注意: (1) 此函数提供给设备厂商或开发者调用.
 *       (2) 此接口只记录版本号上报事件, 最终状态会由HiLink SDK统一获取上报, 之间大概延时200ms.
 */
int hilink_ota_rpt_ver(void);

/*
 * 设备调用该函数，主动向网关或云端上报设备固件升级进度.
 * 参数progress表示设备升级进度或错误码, 取值如下:
 *  [0,100], 升级进度正常进度
 *  101,     升级失败
 *  1000,    无法网络通信
 *  1001,    镜像网络下载异常
 *  1002,    校验失败
 *  1003,    写入失败
 * 参数bootTime表示设备升级完成重启到重新上线的时间, 取值大于0, 单位秒(s).
 * 返回0 无错误; 非0 上报固件升级进度失败
 * 注意: (1) 此函数提供给设备厂商或开发者调用.
 *       (2) 此接口只记录进度上报事件, 最终状态会由HiLink SDK统一获取上报, 之间大概延时200ms.
 */
int hilink_ota_rpt_prg(int progress, int bootTime);

#ifdef __cplusplus
}
#endif

#endif /* _HILINK_OTA_H_ */

/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: WiFi nan适配层头文件
 * Note: 本文件部分接口已经提供给第三方厂商使用，为了前向兼容，暂不按编码规范整改
 */
#ifndef HILINK_WIFI_NAN_ADAPTER_H
#define HILINK_WIFI_NAN_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * WIFI感知底层驱动初始化
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanInit(void);

/*
 * WIFI感知底层驱动去初始化
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanUninit(void);

/*
 * WIFI感知启动订阅服务
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanStartSubscribe(const char *serviceName, unsigned char localHandle, void *handle);

/*
 * WIFI感知停止订阅服务
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanStopSubscribe(unsigned char localHandle);

/*
 * WIFI感知发包接口
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanSendPacket(unsigned char *macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char *msg, int len);

/*
 * WIFI感知切换到超短距状态
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanSwitchSafeDistance(void);

/*
 * WIFI感知切换到正常天线状态
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanSwitchNormalDistance(void);

/*
 * beacon帧开启和关闭
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_NanBeaconSwitch(unsigned char enable);

int HILINK_SoftApDeauthStation(const unsigned char *mac, unsigned char len);

/*
* 获取HiLink SDK支持NAN能力信息
* 返回0表示支持私有NAN，返回1表示支持标准NAN，返回2表示私有NAN和标准NAN都支持，返回3表示预留位
*/
unsigned int HILINK_GetNanAbility(void);

/*
 * 设置beacon帧
 * 返回0表示成功，返回-1表示失败
 */
int HILINK_SetBeaconPeriod(void);

#ifdef __cplusplus
}
#endif

#endif /* HILINK_WIFI_NAN_ADAPTER_H */
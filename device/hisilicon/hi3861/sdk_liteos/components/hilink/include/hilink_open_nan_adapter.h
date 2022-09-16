/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: NAN 适配接口
 */
#ifndef HILINK_OPEN_NAN_ADAPTER_H
#define HILINK_OPEN_NAN_ADAPTER_H

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
 * 设备处于待用户识别状态时通知用户: 表现为持续蜂鸣或闪灯2s.
 * 参数enable表示识别状态,1为开始蜂鸣或闪灯,0为结束蜂鸣或闪灯.
 * 注意: 函数由设备开发者或厂商实现,仅在hi3861模组使用.
 */
void HILINK_SetNanIdentifyStatus(int enable);

/*
 * 设置WIFI安全距离的功率
 * 参数power:表示安全距离对应的发射通道功率,该值不能低于-70db;
 * 需要保证空口功率小于等于-65dBm,根据真实设备来调整.
 * 返回值:0表示设置成功,-1表示设置失败.
 * 注意: 函数由设备开发者或厂商调用，仅在hi3861模组使用.
 */
int HILINK_SetSafeDistancePower(signed char power);

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

#endif

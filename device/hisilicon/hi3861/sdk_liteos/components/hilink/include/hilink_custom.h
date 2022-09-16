/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: HiLink SDK 定制化接口头文件
 */
#ifndef HILINK_CUSTOM_H
#define HILINK_CUSTOM_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(AM_PART_APOLLO3P)
/*
 * 挂起hilink任务
 * 该函数由设备开发者或厂商调用
 */
void HILINK_MainTaskSuspend();

/*
 * 恢复hilink任务
 * 该函数由设备开发者或厂商调用
 */
void HILINK_MainTaskResume();
#endif

/*
 * 获取设备表面的最强点信号发射功率强度，最强点位置的确定以及功率测试方
 * 法，参照hilink认证wifi靠近发现功率设置及测试方法指导文档，power为出参
 * ，单位dbm，返回设备表面的最强信号强度值，如果厂商不想使用wifi靠近发现功
 * 能，接口直接返-1，sdk就不做wifi靠近发现的初始化，如果需要使用wifi靠近
 * 发现，则接口返回0，power返回对应的功率值，power的有效值必须<=20dbm，如
 * 果接口返回0，但power大于20，则也不做wifi靠近发现的初始化，功能不可用
 */
typedef int (*HILINK_GetDevSurfaceWifiPower)(char *power);

/*
 * 功能: 获取当前设备唯一身份标识回调函数
 * 返回: 0，获取成功；返回非0，获取失败。
 * 注意: (1)仅android系统设备适配此接口
 *       (2)固定长度6字节
 *       (3)整个设备生命周期不可改变，包括设备重启和恢复出厂等
 */
typedef int (*HILINK_GetUniqueIdentifier)(unsigned char *id, unsigned int len);

/*
 * 功能: 获取当前设备udid，仅ohos系统适用
 * 返回: 0，获取成功；返回非0，获取失败。
 */
typedef int (*HILINK_GetDeviceUdidCallback)(char *udid, int size);

/*
 * 功能: 接收report消息后云侧返回的token回调函数
 * 返回: 0，获取成功；返回非0，获取失败。
 */
typedef void (*NotifyTokenCallback)(unsigned short token, const unsigned char *payload, unsigned int payloadLen);

/*
 * 获取家居云的URL
 * 该函数由设备开发者或厂商调用
 * 返回0为获取成功，否则失败
 */
int HILINK_GetCloudUrl(char *url, unsigned int len);

/*
 * 设置配网信息
 * 入参数据为json格式字符串，具体内容字段如下：
 * {
 *   "ssid": "xxx-xxxx", //路由器SSID字段,必选
 *   "password": "xxxxxxxx", //路由器密码字段,必选
 *   "devId": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxx", //注册信息-设备ID字段,必选
 *   "psk": "xxxxxxxxxxxxxxxxxxxxxxx", //注册信息-预制秘钥字段(转换十六进制字符串),必选
 *   "code": "xxxxxxx", //注册信息-激活码字段,必选
 *   "cloudPrimaryUrl": "xxxxx.xxxxx.xx", //主域名字段(主备域名),必选
 *   "cloudStandbyUrl": "xxxxx.xxxxx.xx", //备份域名字段(主备域名),必选
 *   "cloudUrl": "xxxxx.xxxxx.xx", //域名字段(兼容方案),可选
 *   "WifiBgnSwitch": 1, //WiFi工作模式字段,可选
 *   "timeZoneDisplay": "GMT+08:00", //时区信息,可选
 *   "timeZoneId": "Asia/Shanghai" //时区ID,可选
 * }
 * 返回0表示设置成功，其他表示设置失败(-2表示HiLink未处于接收配网数据状态)
 */
int HILINK_SetNetConfigInfo(const char *info);

/*
 * 功能: 查询HiLink SDK配置信息保存路径，仅posix或utils_file文件系统适用。
 * 参数: (1) path，出入参，保存路径的缓冲区；
 *       (2) len，入参，缓冲区长度。
 * 返回: 0，获取成功；非0，获取失败。
 * 注意: 非posix或utils_file文件系统无此接口实现
 */
int HILINK_GetConfigInfoPath(char *path, unsigned int len);

/*
 * 功能: 设置HiLink SDK配置信息保存路径，默认保存在/usrdata/hilink目录下，仅posix或utils_file文件系统适用。
 * 参数: path，路径信息，绝对路径长度在posix下不超过127，在utils_file下不超过15。
 * 返回: 0，设置成功；非0，设置失败。
 * 注意: 非posix或utils_file文件系统无此接口实现
 */
int HILINK_SetConfigInfoPath(const char *path);

/*
 * HiLink SDK外部诊断信息记录接口
 */
void HILINK_DiagnosisRecordEx(int errCode);

/*
 * 注册获取设备表面的最强点信号发射功率强度回调函数
 * 返回0表示成功，返回非0失败
 */
int HILINK_RegDevSurfacePowerCallback(HILINK_GetDevSurfaceWifiPower cb);

/*
 * 功能: 注册获取当前设备唯一身份标识回调函数
 * 返回: 0表示成功，返回非0失败
 */
int HILINK_RegUniqueIdentifierCallback(HILINK_GetUniqueIdentifier cb);

/*
 * 功能: 注册获取当前设备udid接口
 * 返回: 0表示成功，返回非0失败
 */
int HILINK_RegGetUdidCallback(HILINK_GetDeviceUdidCallback cb);

/*
 * 功能：注册上报消息时云侧返回token的回调，与 HILINK_IndicateCharState 配合确定消息上报结果
 * 参数：(1) NotifyTokenCallback 入参，token的处理函数
 * 返回：返回0表示注册成功，否则失败
 */
int HILINK_RegisterNotifyTokenCallback(NotifyTokenCallback cb);

/*
 * 功能：(1) 主动上报服务属性状态且得到消息的token1
 *       (2) 在 HILINK_RegisterNotifyTokenCallback 注册的回调中返回上报结果的token2
 *       (3) 匹配token1和token2得到本次上报结果
 * 参数：(1) svcId 入参，服务ID
 *       (2) payload 入参，json格式服务属性数据
 *       (3) payloadLen 入参，payload长度
 *       (4) token 出参，此次上报消息的token值
 * 返回：返回0表示服务状态上报成功，返回-1表示服务状态上报失败
 * 注意：(1) 该接口只有同步使用方式，对于事件类上报推荐使用同步上报
 *       (2) 同步上报：payload不为NULL且len不为0时，调用该接口时，HiLink SDK会立即上报该payload
 */
int HILINK_IndicateCharState(const char *svcId, const char *payload, unsigned int payloadLen,
    unsigned short *token);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* HILINK_CUSTOM_H */
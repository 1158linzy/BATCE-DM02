/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink产品适配头文件
 */
#ifndef HILINK_DEVICE_H
#define HILINK_DEVICE_H

#include "hilink.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    char sn[40];            /* 设备唯一标识，比如sn号，长度范围(0,40] */
    char prodId[5];         /* 设备产品ID，长度范围(0,5] */
    char subProdId[3];      /* 设备子型号，长度范围[0,3] */
    char model[32];         /* 设备型号，长度范围(0,32] */
    char devTypeId[4];      /* 设备类型ID，长度范围(0,4] */
    char devTypeName[32];   /* 设备类型英文名称，长度范围(0,32] */
    char manuId[4];         /* 设备制造商ID，长度范围(0,4] */
    char manuName[32];      /* 设备制造商英文名称，长度范围(0,32] */
    char fwv[64];           /* 设备固件版本，长度范围[0,64] */
    char hwv[64];           /* 设备硬件版本，长度范围[0,64] */
    char swv[64];           /* 设备软件版本，长度范围[0,64] */
} HILINK_DevInfo;

typedef struct {
    char svcType[32];  /* 服务类型，长度范围(0, 32] */
    char svcId[64];    /* 服务ID，长度范围(0, 64] */
} HILINK_SvcInfo;

enum HILINK_StateMachine {
    /* 设备与云端连接断开(版本向前兼容) */
    HILINK_M2M_CLOUD_OFFLINE = 0,
    /* 设备连接云端成功，处于正常工作态(版本向前兼容) */
    HILINK_M2M_CLOUD_ONLINE,
    /* 设备与云端连接长时间断开(版本向前兼容) */
    HILINK_M2M_LONG_OFFLINE,
    /* 设备与云端连接长时间断开后进行重启(版本向前兼容) */
    HILINK_M2M_LONG_OFFLINE_REBOOT,
    /* HiLink线程未启动 */
    HILINK_UNINITIALIZED,
    /* 设备处于配网模式 */
    HILINK_LINK_UNDER_AUTO_CONFIG,
    /* 设备处于10分钟超时状态 */
    HILINK_LINK_CONFIG_TIMEOUT,
    /* 设备正在连接路由器 */
    HILINK_LINK_CONNECTTING_WIFI,
    /* 设备已经连上路由器 */
    HILINK_LINK_CONNECTED_WIFI,
    /* 设备正在连接云端 */
    HILINK_M2M_CONNECTTING_CLOUD,
    /* 设备与路由器的连接断开 */
    HILINK_LINK_DISCONNECT,
    /* 设备被注册 */
    HILINK_DEVICE_REGISTERED,
    /* 设备被解绑 */
    HILINK_DEVICE_UNREGISTER,
    /* 设备复位标记置位 */
    HILINK_REVOKE_FLAG_SET,
    /* 设备协商注册信息失败 */
    HILINK_NEGO_REG_INFO_FAIL,
    /* 设备与路由器的连接失败 */
    HILINK_LINK_CONNECTED_FAIL,
    /* 打开中枢模式成功 */
    HILINK_OPEN_CENTRAL_MODE_OK,
    /* 打开中枢模式失败 */
    HILINK_OPEN_CENTRAL_MODE_FAILE,
    /* 本地端口创建成功 */
    HILINK_CREATE_CENTRAL_PORT_OK,
    /* 设备恢复交房标记 */
    HILINK_RECOVER_HANDOVER,
    /* 设备交房标记 */
    HILINK_HANDOVER,
};

/*
 * 功能：获取设备信息
 * 参数：devinfo 出入参，待填充的设备信息
 * 返回：获取成功返回 0，否则返-1
 * 注意：(1) sn不填充时将使用设备mac地址作为sn
 *       (2) 如果产品定义有子型号则需要填充subProdId
 *       (3) 所有需要填充的字段都需要以'\0'结束
 *       (4) softap配网时devTypeName与manuName用来拼接ssid，两个字段长度和尽量不超过17字节，否则会截断
 */
int HILINK_GetDevInfo(HILINK_DevInfo *devinfo);

/*
 * 功能：获取设备服务信息
 * 参数：(1) svcInfo 出入参，待填充的设备服务信息结构体指针数组
 *       (2) size 入参，svcInfo结构体指针数组大小
 * 返回：获取成功返回服务数量，否则返-1
 * 注意：所有需要填充的字段都需要以'\0'结束
 */
int HILINK_GetSvcInfo(HILINK_SvcInfo *svcInfo[], unsigned int size);

/* 获取AC 参数接口函数 */
unsigned char *HILINK_GetAutoAc(void);

/*
 * 修改服务当前字段值
 * svcId为服务的ID，payload为接收到需要修改的Json格式的字段与其值，len为payload的长度
 * 返回0表示服务状态值修改成功，不需要底层设备主动上报，由HiLink SDK上报；
 * 返回-101表示获得报文不符合要求；
 * 返回-111表示服务状态值正在修改中，修改成功后底层设备必须主动上报；
 */
int HILINK_PutCharState(const char *svcId, const char *payload, unsigned int len);

/*
 * 获取服务字段值
 * svcId表示服务ID。厂商实现该函数时，需要对svcId进行判断；
 * in表示接收到的Json格式的字段与其值；
 * inLen表示接收到的in的长度；
 * out表示保存服务字段值内容的指针,内存由厂商开辟，使用完成后，由Hilink Device SDK释放；
 * outLen表示读取到的payload的长度；
 * 返回0表示服务状态字段值获取成功，返回非0表示获取服务状态字段值不成功。
 */
int HILINK_GetCharState(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen);

/*
 * 获取SoftAp配网PIN码
 * 返回值为8位数字PIN码, 返回-1表示使用HiLink SDK的默认PIN码
 * 该接口需设备开发者实现
 * 安全认证要求，PIN码不能由sn、mac等设备固定信息生成。
 */
int HILINK_GetPinCode(void);

/*
 * 通知设备的状态
 * status表示设备当前的状态
 * 注意，此函数由设备厂商根据产品业务选择性实现
 */
void HILINK_NotifyDevStatus(int status);

/*
 * 实现模组重启前的设备操作
 * flag为0表示HiLink SDK 线程看门狗触发模组重启; 为1表示APP删除设备触发模组重启
 * 返回0表示处理成功, 系统可以重启，使用硬重启; 返回1表示处理成功, 系统可以重启，使用软重启;
 * 返回负值表示处理失败, 系统不能重启
 * 注意，此函数由设备厂商实现；若APP删除设备触发模组重启时，设备操作完务必返回0，否则会导致删除设备异常
 */
int HILINK_ProcessBeforeRestart(int flag);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* HILINK_DEVICE_H */
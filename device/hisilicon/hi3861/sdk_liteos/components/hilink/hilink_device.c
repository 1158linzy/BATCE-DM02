/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink产品适配实现源文件
 */
#include "hilink_device.h"
#include <stdlib.h>
#include "hilink.h"
#include "securec.h"

#define HILINK_APP

/* 设备产品ID */
static const char *PRODUCT_ID = "2705";
/* 设备产品子型号ID */
static const char *SUB_PRODUCT_ID = "";
/* 设备类型ID */
static const char *DEVICE_TYPE_ID = "119";
/* 设备类型英文名称 */
static const char *DEVICE_TYPE_NAME = "Light Bulb";
/* 设备制造商ID */
static const char *MANUAFACTURER_ID = "a66";
/* 设备制造商英文名称 */
static const char *MANUAFACTURER_NAME = "boantong";
/* 设备型号 */
static const char *PRODUCT_MODEL = "GJTT-02";
/* 设备SN */
static const char *PRODUCT_SN = "";
/* 设备固件版本号 */
static const char *FIRMWARE_VER = "1.1.0";
/* 设备硬件版本号 */
static const char *HARDWARE_VER = "1.1.0";
/* 设备软件版本号 */
static const char *SOFTWARE_VER = "1.0.0";

/* 服务信息定义 */
static const HILINK_SvcInfo SVC_INFO[] = {
    { "switch", "switch" }
};

int HILINK_GetDevInfo(HILINK_DevInfo *devinfo)
{
    if (devinfo == NULL) {
        return -1;
    }
    int err = EOK;
    err |= strcpy_s(devinfo->sn, sizeof(devinfo->sn), PRODUCT_SN);
    err |= strcpy_s(devinfo->prodId, sizeof(devinfo->prodId), PRODUCT_ID);
    err |= strcpy_s(devinfo->subProdId, sizeof(devinfo->subProdId), SUB_PRODUCT_ID);
    err |= strcpy_s(devinfo->model, sizeof(devinfo->model), PRODUCT_MODEL);
    err |= strcpy_s(devinfo->devTypeId, sizeof(devinfo->devTypeId), DEVICE_TYPE_ID);
    err |= strcpy_s(devinfo->devTypeName, sizeof(devinfo->devTypeName), DEVICE_TYPE_NAME);
    err |= strcpy_s(devinfo->manuId, sizeof(devinfo->manuId), MANUAFACTURER_ID);
    err |= strcpy_s(devinfo->manuName, sizeof(devinfo->manuName), MANUAFACTURER_NAME);
    err |= strcpy_s(devinfo->fwv, sizeof(devinfo->fwv), FIRMWARE_VER);
    err |= strcpy_s(devinfo->hwv, sizeof(devinfo->hwv), HARDWARE_VER);
    err |= strcpy_s(devinfo->swv, sizeof(devinfo->swv), SOFTWARE_VER);
    if (err != EOK) {
        return -1;
    }
    return 0;
}

int HILINK_GetSvcInfo(HILINK_SvcInfo *svcInfo[], unsigned int size)
{
    unsigned int svcNum = sizeof(SVC_INFO) / sizeof(HILINK_SvcInfo);
    if ((svcInfo == NULL) || (size == 0) || (size < svcNum)) {
        return -1;
    }

    for (unsigned int i = 0; i < svcNum; ++i) {
        if (memcpy_s(svcInfo[i], sizeof(HILINK_SvcInfo), &SVC_INFO[i], sizeof(HILINK_SvcInfo)) != EOK) {
            return -1;
        }
    }
    return svcNum;
}

/* AC参数 */
unsigned char A_C[48] = {
    0x49, 0x3F, 0x45, 0x4A, 0x3A, 0x72, 0x38, 0x7B, 0x36, 0x32, 0x50, 0x3C, 0x49, 0x39, 0x62, 0x38,
    0x72, 0xCB, 0x6D, 0xC5, 0xAE, 0xE5, 0x4A, 0x82, 0xD3, 0xE5, 0x6D, 0xF5, 0x36, 0x82, 0x62, 0xEB,
    0x89, 0x30, 0x6C, 0x88, 0x32, 0x56, 0x23, 0xFD, 0xB8, 0x67, 0x90, 0xA7, 0x7B, 0x61, 0x1E, 0xAE
};

/* 获取加密 AC 参数  */
unsigned char *HILINK_GetAutoAc(void)
{
    return A_C;
}

/*
 * 修改服务当前字段值
 * svcId为服务的ID，payload为接收到需要修改的Json格式的字段与其值，len为payload的长度
 * 返回0表示服务状态值修改成功，不需要底层设备主动上报，由Hilink Device SDK上报；
 * 返回-101表示获得报文不符合要求；
 * 返回-111表示服务状态值正在修改中，修改成功后底层设备必须主动上报；
 */
int HILINK_PutCharState(const char *svcId, const char *payload, unsigned int len)
{

#ifdef HILINK_APP
	extern int hilink_put_char_state(const char *svc_id, const char *payload,
			unsigned int len);

	hilink_put_char_state(svcId, payload, len);
#endif

    return 0;
}

/*
 * 获取服务字段值
 * svcId表示服务ID。厂商实现该函数时，需要对svcId进行判断；
 * in表示接收到的Json格式的字段与其值；
 * inLen表示接收到的in的长度；
 * out表示保存服务字段值内容的指针,内存由厂商开辟，使用完成后，由Hilink Device SDK释放；
 * outLen表示读取到的payload的长度；
 * 返回0表示服务状态字段值获取成功，返回非0表示获取服务状态字段值不成功。
 */
int HILINK_GetCharState(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen)
{

#ifdef HILINK_APP
	extern int hilink_get_char_state(const char *svc_id, const char *in,
			unsigned int in_len, char **out, unsigned int *out_len);

	hilink_get_char_state(svcId, in, inLen, out, outLen);
#endif
    return 0;
}

/*
 * 功能: 获取SoftAp配网PIN码
 * 返回: 返回00000000到99999999之间的8位数字PIN码, 返回-1表示使用HiLink SDK的默认PIN码
 * 注意: (1)安全认证要求，PIN码不能由sn、mac等设备固定信息生成
 *       (2)该接口实现需要与devicepartner平台匹配
 */
int HILINK_GetPinCode(void)
{
    return -1;
}

/*
 * 通知设备的状态
 * status表示设备当前的状态
 * 注意，此函数由设备厂商根据产品业务选择性实现
 */
extern int hilink_report_state();
void HILINK_NotifyDevStatus(int status)
{
    switch (status) {
        case HILINK_M2M_CLOUD_OFFLINE:
            /* 设备与云端连接断开，请在此处添加实现 */
            break;
        case HILINK_M2M_CLOUD_ONLINE:
            /* 设备连接云端成功，请在此处添加实现 */
            hilink_report_state();
            break;
        case HILINK_M2M_LONG_OFFLINE:
            /* 设备与云端连接长时间断开，请在此处添加实现 */
            break;
        case HILINK_M2M_LONG_OFFLINE_REBOOT:
            /* 设备与云端连接长时间断开后进行重启，请在此处添加实现 */
            break;
        case HILINK_UNINITIALIZED:
            /* HiLink线程未启动，请在此处添加实现 */
            break;
        case HILINK_LINK_UNDER_AUTO_CONFIG:
            /* 设备处于配网模式，请在此处添加实现 */
            break;
        case HILINK_LINK_CONFIG_TIMEOUT:
            /* 设备处于10分钟超时状态，请在此处添加实现 */
            break;
        case HILINK_LINK_CONNECTTING_WIFI:
            /* 设备正在连接路由器，请在此处添加实现 */
            break;
        case HILINK_LINK_CONNECTED_WIFI:
            /* 设备已经连上路由器，请在此处添加实现 */
            break;
        case HILINK_M2M_CONNECTTING_CLOUD:
            /* 设备正在连接云端，请在此处添加实现 */
            break;
        case HILINK_LINK_DISCONNECT:
            /* 设备与路由器的连接断开，请在此处添加实现 */
            break;
        case HILINK_DEVICE_REGISTERED:
            /* 设备被注册，请在此处添加实现 */
            break;
        case HILINK_DEVICE_UNREGISTER:
            /* 设备被解绑，请在此处添加实现 */
            break;
        case HILINK_REVOKE_FLAG_SET:
            /* 设备被复位标记置位，请在此处添加实现 */
            break;
        case HILINK_NEGO_REG_INFO_FAIL:
            /* 设备协商配网信息失败 */
            break;
        case HILINK_LINK_CONNECTED_FAIL:
            /* 设备与路由器的连接失败 */
            break;
        default:
            break;
    }

    return;
}

/*
 * 实现模组重启前的设备操作
 * flag为0表示HiLink SDK 线程看门狗触发模组重启; 为1表示APP删除设备触发模组重启
 * 返回0表示处理成功, 系统可以重启, 使用硬重启;
 * 返回1表示处理成功, 如果通过HILINK_SetSdkAttr()注册了软重启(sdkAttr.rebootSoftware), 则使用软重启,
 * 否则不重启HiLink应用, 回退HiLink内部状态, 重新进入配网;
 * 返回负值表示处理失败, 系统不能重启
 * 注意，此函数由设备厂商实现
 */
int HILINK_ProcessBeforeRestart(int flag)
{
    /* HiLink SDK线程看门狗超时触发模组重启 */
    if (flag == 0) {
        /* 实现模组重启前的操作(如:保存系统状态等) */
        return -1;
    }

    /* APP删除设备触发模组重启 */
    if (flag == 1) {
        /* 实现模组重启前的操作(如:保存系统状态等) */
        return 0;
    }

    return -1;
}

/*
 * 获取设备表面的最强点信号发射功率强度，最强点位置的确定以及功率测试方
 * 法，参照hilink认证wifi靠近发现功率设置及测试方法指导文档，power为出参
 * ，单位dbm，返回设备表面的最强信号强度值，如果厂商不想使用wifi靠近发现功
 * 能，接口直接返-1，sdk就不做wifi靠近发现的初始化，如果需要使用wifi靠近
 * 发现，则接口返回0，power返回对应的功率值，power的有效值必须<=20dbm，如
 * 果接口返回0，但power大于20，则也不做wifi靠近发现的初始化，功能不可用
 */
int HILINK_GetDevSurfacePower_Def(char *power)
{
    *power = 15;
    return 0;
}

/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: NAN 适配接口函数(需设备厂商实现)
 */
#include "hilink_open_nan_adapter.h"
#include <stdlib.h>

#include "hilink_open_type.h"
#include "hi_types_base.h"
#include "hi_reset.h"
#include "hi_wifi_api.h"
#include "hi_wifi_sdp_api.h"
#include "hilink_device.h"

#define NAN_SYNC_MOD_MASK 0x3
#define NAN_HASH_RESULT_LEN 32
#define NAN_SAFE_DISTANCE_RSSI (-27) //-52
#define NAN_DISTANCE_UNKNOW 0
#define NAN_DISTANCE_NORMAL 1
#define NAN_DISTANCE_SAFE 2
#define NAN_SAFE_DISTANCE_RETRY_TIMES 100
#define NAN_NORMAL_DISTANCE_RETRY_TIMES 0
#define HILINK_SOFTAP_BEACON_PERIOD 34
#define AP_NAME   "ap0"

static char g_safeDistancePower = NAN_SAFE_DISTANCE_RSSI;
static char g_isSafeDistance = NAN_DISTANCE_UNKNOW;

int HILINK_NanInit(void)
{
    g_isSafeDistance = NAN_DISTANCE_UNKNOW;
    if (hi_wifi_sdp_init(AP_NAME) != 0) {
        return HILINK_ERR;
    }
    return HILINK_OK;
}

int HILINK_NanUninit(void)
{
    g_isSafeDistance = NAN_DISTANCE_UNKNOW;
    if (hi_wifi_sdp_deinit() != 0) {
        return HILINK_ERR;
    }
    return HILINK_OK;
}

int HILINK_NanStartSubscribe(const char *serviceName, unsigned char localHandle, void *handle)
{
    if ((serviceName == NULL) || (handle == NULL)) {
        return HILINK_ERR;
    }
    char hashResult[NAN_HASH_RESULT_LEN] = {0};
    mbedtls_sha256((const unsigned char *)serviceName, hilink_strlen(serviceName), (unsigned char *)hashResult, 0);
    int ret = hi_wifi_sdp_start_service(hashResult, localHandle, (hi_wifi_sdp_recv_cb)handle, WIFI_SDP_SUBSCRIBE);
    if (ret != 0) {
        return  HILINK_ERR;
    }
    return HILINK_OK;
}

int HILINK_NanStopSubscribe(unsigned char localHandle)
{
    int ret = hi_wifi_sdp_stop_service(localHandle, WIFI_SDP_SUBSCRIBE);
    if (ret != 0) {
        return  HILINK_ERR;
    }
    return HILINK_OK;
}

int HILINK_NanSendPacket(unsigned char *macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char *msg, int len)
{
    if ((macAddr == NULL) || (msg == NULL)) {
        return  HILINK_ERR;
    }
    int ret = hi_wifi_sdp_send(macAddr, peerHandle, localHandle, msg, len);
    if (ret != 0) {
        return  HILINK_ERR;
    }
    return HILINK_OK;
}

int HILINK_NanSwitchSafeDistance(void)
{
    if (g_isSafeDistance == NAN_DISTANCE_SAFE) {
        return HILINK_OK;
    }
    int ret = hi_wifi_sdp_adjust_tx_power(AP_NAME, g_safeDistancePower);
    if (ret != 0) {
        return HILINK_ERR;
    }
    g_isSafeDistance = NAN_DISTANCE_SAFE;
    ret = hi_wifi_sdp_set_retry_times(NAN_SAFE_DISTANCE_RETRY_TIMES);
    if (ret != 0) {
        printf("set safe distance retry times fail ignore\r\n");
    }
    return HILINK_OK;
}

int HILINK_NanSwitchNormalDistance(void)
{
    if (g_isSafeDistance == NAN_DISTANCE_NORMAL) {
        return HILINK_OK;
    }
    int ret = hi_wifi_sdp_set_retry_times(NAN_NORMAL_DISTANCE_RETRY_TIMES);
    if (ret != 0) {
        printf("set safe distance retry times fail ignore\r\n");
    }
    ret = hi_wifi_sdp_restore_tx_power(AP_NAME);
    if (ret != 0) {
        return  HILINK_ERR;
    }
    g_isSafeDistance = NAN_DISTANCE_NORMAL;
    return HILINK_OK;
}

int HILINK_NanBeaconSwitch(unsigned char enable)
{
    int ret = hi_wifi_sdp_beacon_switch(AP_NAME, enable);
    if (ret != 0) {
        return  HILINK_ERR;
    }
    return HILINK_OK;
}

int HILINK_SetSafeDistancePower(signed char power)
{
    if (g_safeDistancePower != power) {
        g_safeDistancePower = power;
    }
    return HILINK_OK;
}

int HILINK_SoftApDeauthStation(const unsigned char *mac, unsigned char len)
{
    int ret;
    if (mac == NULL) {
        return HILINK_ERR;
    }
    ret = hi_wifi_softap_deauth_sta(mac, len);
    if (ret != 0) {
        printf("deauth sta fail(%d)\r\n", ret);
        return HILINK_ERR;
    }
    return HILINK_OK;
}

void HILINK_SetNanIdentifyStatus(int enable)
{
	(void)enable;
}

unsigned int HILINK_GetNanAbility(void)
{
    int nanSyncMode = hi_wifi_sdp_get_sync_mode();
    return ((unsigned int)nanSyncMode & NAN_SYNC_MOD_MASK);
}

int HILINK_SetBeaconPeriod(void)
{
    if (hi_wifi_softap_set_beacon_period(HILINK_SOFTAP_BEACON_PERIOD) != 0) {
        return HILINK_ERR;
    }
    return HILINK_OK;
}

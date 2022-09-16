/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: interface file for kitframework client
 * Author: yangzuxun
 * Create: 2020-07-20
 */

#ifndef HOS_LITE_KIT_FRAMEWORK_CLIENT_H
#define HOS_LITE_KIT_FRAMEWORK_CLIENT_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

// request auth result
#define ERR_OK               0
#define SERVICE_UNAVAILABLE (-1)
#define INVALID_PARAM       (-2)

#define  PASS     0
#define  FAIL    (-1)
#define  UNKNOWN (-2)

// define authentication control policy
#define POLICY_ALERT    (0x1 << 0)  // 警告，kit自行决定
#define POLICY_DENY     (0x1 << 1)  // 拒绝
#define POLICY_PASS     (0x1 << 2)  // 放通
#define POLICY_INVISBLE (0x1 << 3)  // 设备不可见

// kit id
#define DV_KIT    "DvKit"
#define CAST_KIT  "CastKit"
#define HI_LINK   "HiLinkKit"
#define HI_CAR    "HiCarKit"
#define SHARE     "ShareKit"
#define MEET_TIME "MeetTimeKit"
#define ONE_HOP   "OneHopKit"
#define ONE_TAG   "OneTagKit"

// CheckModuleValid return value
#define RET_OK        1
#define RET_FALSE     0
#define RET_INVALID (-1)

typedef enum {
    TYPE_AA,       // hap or static form
    TYPE_DEBUG,    // debug version
    TYPE_BDREPORT, // big data report module
    TYPE_D2D,      // D2D module
    TYPE_D2C,      // D2C module
} CheckType;

typedef struct AuthStatus AuthStatus;
typedef void (*IAuthResultCallback)(const AuthStatus* status);

/**
 * @brief The function is used to initial the authtication client and connect to the
 * kit server, and return the connect status. The function must be called before
 * RequestAuth(const char* networkId, const char* kitId, IAuthResultCallback callback).
 * @param kitId: the Kit identity(package name) which describe the kits to be checked.
 * @return error_code: REQ_OK or SERVICE_UNAVAILABLE or INVALID_PARAM
 */
int32_t CreateKitFrameworkClient(const char* kitId);

/**
 * @brief The function is used to trigger device auth, and return the auth result.
 * The caller can only call the function without calling CreateKitFrameworkClient(const char* kitId) before.
 * @param networkId: the name defined in soft-bus, NULL for local device.
 * @param kitId: the Kit identity(package name) which describe the kits to be checked.
 * @param callback: the Auth check result callback.
 * @return error_code: REQ_OK or SERVICE_UNAVAILABLE or INVALID_PARAM
 */
int32_t RequestAuth(const char* networkId, const char* kitId, IAuthResultCallback callback);

/**
 * @brief The function is used to close the connection of kit server and release the client.
 * The function must be called after
 * RequestAuth(const char* networkId, const char* kitId, IAuthResultCallback callback).
 * @param kitId: the Kit identity(package name) which describe the kits to be checked.
 */
void CloseKitFrameworkClient(const char* kitId);

/**
 * @brief The function is used to check the type whether exists or not
 * @param type: the check module type.
 * @return error_code: RET_OK or RET_FALSE or RET_INVALID
 */
int32_t CheckModuleValid(CheckType type);

/**
 * @brief The function is used to get device auth status;
 * @param[in] status: The auth status of kit.
 * @return The auth status of device.
 */
int32_t GetDeviceStatus(const AuthStatus* status);

/**
 * @brief The function is used to get system auth status;
 * @param[in] status: The auth status of kit.
 * @return The auth status of system.
 */
int32_t GetSystemStatus(const AuthStatus* status);

/**
 * @brief The function is used to get kit auth status;
 * @param[in] status: The auth status of kit.
 * @return The auth status of kit.
 */
int32_t GetKitStatus(const AuthStatus* status);

/**
 * @brief The function is used to get kit auth policy;
 * @param[in] status: The auth status of kit.
 * @return The auth policy of kit.
 */
int32_t GetAuthPolicy(const AuthStatus* status);

/**
 * @brief The function is used to get ticket;
 * @param[in] status: The auth status of kit.
 * @return NULL or ticket , max size is 256.
 */
const char* GetTicket(const AuthStatus* status);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
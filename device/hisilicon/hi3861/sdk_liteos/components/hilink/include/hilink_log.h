/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: HiLink SDK日志打印接口头文件
 * Notes: 本文件已提供外部使用, 为保证兼容性, 接口和类型定义暂不按照编程规范修改
 */
#ifndef HILINK_LOG_H
#define HILINK_LOG_H

#include "hilink_typedef.h"
#include "hilink_osadapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 打印日志, release版本可用, 支持是否匿名化打印
 * 匿名化打印原则:
 * (1)用户敏感数据
 * (2)只支持3字节以上的数据, 4~9字节的数据打印前3字节, 超过9字节的打印前9字节
 * (3)IP地址要求10.***.***.10, 本接口不支持, 私网IP不要打印
 * 敏感信息界定:
 * 可以关联到用户个人的数据, 如私网IP/MAC, 设备devid等
 */
void LogStringInfo(const char *tag, const char *message, bool isFuzzy);

#ifdef ENABLE_LOG

typedef enum {
    LOG_LVL_INVALID = -1, /* 消除CodeDex警告, 请不要用此值 */
    LOG_LVL_MIN     = 0,
    LOG_LVL_EMERG   = 0, /* 非常紧急, 系统不可用 */
    LOG_LVL_ALERT   = 1, /* 紧急级别, 必须立即采取措施 */
    LOG_LVL_CRIT    = 2, /* 致命级别 */
    LOG_LVL_ERR     = 3, /* 错误级别 */
    LOG_LVL_WARN    = 4, /* 告警级别 */
    LOG_LVL_NOTICE  = 5, /* 通知级别 */
    LOG_LVL_INFO    = 6, /* 信息级别 */
    LOG_LVL_DEBUG   = 7, /* 调试级别 */
    LOG_LVL_MAX     = LOG_LVL_DEBUG
} debug_level_t;

bool log_is_print(debug_level_t level);
void set_log_level(debug_level_t level);

#define hilink_data(...) \
    do { \
        hilink_printf(__VA_ARGS__); \
    } while (0)

#define hilink_debug(...) \
    do { \
        if (log_is_print(LOG_LVL_DEBUG)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_info(...) \
    do { \
        if (log_is_print(LOG_LVL_INFO)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_notice(...) \
    do { \
        if (log_is_print(LOG_LVL_NOTICE)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_warning(...) \
    do { \
        if (log_is_print(LOG_LVL_WARN)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_error(...) \
    do { \
        if (log_is_print(LOG_LVL_ERR)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_critical(...) \
    do { \
        if (log_is_print(LOG_LVL_CRIT)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_alert(...) \
    do { \
        if (log_is_print(LOG_LVL_ALERT)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#define hilink_emerg(...) \
    do { \
        if (log_is_print(LOG_LVL_EMERG)) { \
            hilink_printf("%s() %d, ", __func__, __LINE__); \
            hilink_printf(__VA_ARGS__); \
        } \
    } while (0)

#else

#define hilink_data(...)
#define hilink_debug(...)
#define hilink_info(...)
#define hilink_notice(...)
#define hilink_warning(...)
#define hilink_error(...)
#define hilink_critical(...)
#define hilink_alert(...)
#define hilink_emerg(...)

#endif /* ENABLE_LOG */

int HILINK_ExportPrintf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* HILINK_LOG_H */

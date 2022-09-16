/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: 工厂测试头文件
 * Create: 2019-03-21
 */
#ifndef _FACTORY_TEST_H_
#define _FACTORY_TEST_H_

#include "hilink_osadapter.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WORK_MODE_FACTORY 0xfaeafa01 /* 工厂测试模式 */
#define WORK_MODE_BUSINESS 0xfaeafa00 /* 商用模式 */

#define FACTORY_HASH_DATA_LEN 64
#define FACTORY_DATA_LOCK_LEN 64
#define FACTORY_MODE_LEN 8
#define FACTORY_SALT_LEN 8
#define FACTORY_FLAG_LEN 8

#define UPGRADE_ZONE_A 1 /* 升级分区1 */
#define UPGRADE_ZONE_B 2 /* 升级分区2 */

typedef struct {
    /* dataLock码 */
    unsigned char dataLock[FACTORY_DATA_LOCK_LEN];
    /* 工作模式 */
    unsigned char workMode[FACTORY_MODE_LEN];
    /* 盐值 */
    unsigned char salt[FACTORY_SALT_LEN];
    /* 摘要信息 */
    unsigned char digest[FACTORY_HASH_DATA_LEN];
    /* 是否首次启动标记 */
    unsigned char bootFlag[FACTORY_FLAG_LEN];
} FactoryInfo;

/* 获取产测信息, 厂家实现 */
int HILINK_GetFactoryInfo(FactoryInfo *info);

/* 设置启动flag, 厂家实现 */
int HILINK_SetBootFlag(unsigned int flag);

/* 设置datalock摘要信息, 厂家实现 */
int HILINK_SetDigest(const char *buf, int len);

/* 设置工作模式, 厂家实现 */
int HILINK_SetWorkMode(unsigned int mode);

/* 测试命令处理, 厂家实现 */
int HILINK_FactoryCmd(const char *inBuf, int inLen, char *outBuf, int *outLen);

/*
 * 获取升级区域,
 * 返回值, 1:分区1, 2:分区2
 */
int HILINK_OtaGetZone(void);

/*
 * 局域网产测升级接口
 * type表示文件类型, 0: filelist.json 文件  1: filelist.json.asc 文件, 2:升级包bin文件
 * fileSize表示文件总大小
 * data表示本次发送的文件数据
 * len表示次发送的文件数据长度
 * flag为0表示发送的第一个包, 非0表示后续的包
 * 返回0表示成功，-1表示失败
 */
int HILINK_OtaSendFile(int type, int fileSize, const char *data, int len, int flag);

/*
 * 修改工作模式
 * dataLock表示识别码
 * dataLockLen表示识别码长度
 * workMode表示工作模式. 产测模式: 0xfaeafa01, 商用模式: 0xfaeafa00
 * 返回0表示成功,-1表示失败
 */
int HILINK_ModifyWorkMode(const char *dataLock, int dataLockLen, unsigned int workMode);

#ifdef __cplusplus
}
#endif

#endif


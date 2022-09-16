/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: implement of oem_auth_result_storage
 * Author: Kit Framework group
 * Create: 2020-12-26
 */

#include "oem_auth_result_storage.h"

#include <stdio.h>
#include "securec.h"
#include "hi_tsensor.h"
#include "utils_file.h"

#define RESET_FILE_NAME "auth_reset"
#define TICKET_FILE_NAME "auth_ticket"
#define AUTH_STATS_FILE_NAME "auth_authStats"
#define WRITE_FLASH_MAX_TEMPERATURE 80

static const uint32_t MAX_FILE_BYTES_LIMIT = 5120;

bool OEMIsOverTemperatureLimit(void)
{
    hi_s16 temperature = 0;
    int ret = hi_tsensor_read_temperature(&temperature);
    if (ret != 0) {
        return true;
    }
    return ((temperature >= WRITE_FLASH_MAX_TEMPERATURE) ? true : false);
}

static bool IsFileExist(const char* path)
{
    if (path == NULL) {
        return false;
    }
    int32_t fd = UtilsFileOpen(path, O_RDONLY_FS, 0);
    if (fd < 0) {
        return false;
    }
    (void)UtilsFileClose(fd);
    return true;
}

static int32_t CreateFile(const char* path)
{
    if (path == NULL) {
        return -1;
    }
    int32_t fd = UtilsFileOpen(path, O_CREAT_FS, 0);
    if (fd < 0) {
        return -1;
    }
    (void)UtilsFileClose(fd);
    return 0;
}

static int32_t WriteFile(const char* path, const char* data, uint32_t dataLen, int flag)
{
    if (path == NULL || data == NULL || dataLen == 0) {
        printf("OEM Adapter: Invalid parameter\n");
        return -1;
    }
    if (dataLen > MAX_FILE_BYTES_LIMIT) {
        printf("OEM Adapter: Write data over limit\n");
        return -1;
    }
    if (OEMIsOverTemperatureLimit()) {
        return -1;
    }

    int32_t fd = UtilsFileOpen(path, flag, 0);
    if (fd < 0) {
        printf("OEM Adapter: Open file failed\n");
        return -1;
    }

    int32_t ret = 0;
    if (UtilsFileWrite(fd, data, dataLen) != dataLen) {
        printf("OEM Adapter: Write data failed\n");
        ret = -1;
    }
    (void)UtilsFileClose(fd);
    return ret;
}

static int32_t ReadFile(const char* path, char* buffer, uint32_t bufferLen)
{
    if (path == NULL || buffer == NULL || bufferLen == 0) {
        printf("OEM Adapter: Invalid parameter\n");
        return -1;
    }
    if (!IsFileExist(path)) {
        return -1;
    }
    uint32_t fileSize = 0;
    if (UtilsFileStat(path, &fileSize) != 0) {
        printf("OEM Adapter: Stat file failed\n");
        return -1;
    }
    if (fileSize > bufferLen) {
        printf("OEM Adapter: Read data over buffer length\n");
        return -1;
    }

    int32_t fd = UtilsFileOpen(path, O_RDONLY_FS, 0);
    if (fd < 0) {
        printf("OEM Adapter: Open file failed\n");
        return -1;
    }
    int32_t ret = 0;
    if (UtilsFileRead(fd, buffer, fileSize) != fileSize) {
        printf("OEM Adapter: Read data failed\n");
        ret = -1;
    }
    (void)UtilsFileClose(fd);
    return ret;
}

static int32_t DeleteFile(const char* path)
{
    if (path == NULL) {
        return -1;
    }
    return UtilsFileDelete(path);
}

bool OEMIsResetFlagExist(void)
{
    return IsFileExist(RESET_FILE_NAME);
}

int32_t OEMCreateResetFlag(void)
{
    return CreateFile(RESET_FILE_NAME);
}

int32_t OEMDeleteResetFlag(void)
{
    return DeleteFile(RESET_FILE_NAME);
}

bool OEMIsAuthStatusExist(void)
{
    return IsFileExist(AUTH_STATS_FILE_NAME);
}

int32_t OEMWriteAuthStatus(const char* data, uint32_t len)
{
    if (data == NULL || len == 0) {
        return -1;
    }
    return WriteFile(AUTH_STATS_FILE_NAME, data, len, O_CREAT_FS | O_TRUNC_FS | O_RDWR_FS);
}

int32_t OEMReadAuthStatus(char* buffer, uint32_t bufferLen)
{
    if (buffer == NULL || bufferLen == 0) {
        return -1;
    }
    return ReadFile(AUTH_STATS_FILE_NAME, buffer, bufferLen);
}

int32_t OEMDeleteAuthStatus(void)
{
    return DeleteFile(AUTH_STATS_FILE_NAME);
}

int32_t OEMGetAuthStatusFileSize(uint32_t* len)
{
    if (len == NULL) {
        return -1;
    }
    return UtilsFileStat(AUTH_STATS_FILE_NAME, len);
}

bool OEMIsTicketExist(void)
{
    return IsFileExist(TICKET_FILE_NAME);
}

int32_t OEMWriteTicket(const char* data, uint32_t len)
{
    if (data == NULL || len == 0) {
        return -1;
    }
    return WriteFile(TICKET_FILE_NAME, data, len, O_CREAT_FS | O_TRUNC_FS | O_RDWR_FS);
}

int32_t OEMReadTicket(char* buffer, uint32_t bufferLen)
{
    if (buffer == NULL || bufferLen == 0) {
        return -1;
    }
    return ReadFile(TICKET_FILE_NAME, buffer, bufferLen);
}

int32_t OEMDeleteTicket(void)
{
    return DeleteFile(TICKET_FILE_NAME);
}

int32_t OEMGetTicketFileSize(uint32_t* len)
{
    return UtilsFileStat(TICKET_FILE_NAME, len);
}

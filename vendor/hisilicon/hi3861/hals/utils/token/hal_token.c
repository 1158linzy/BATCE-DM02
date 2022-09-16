/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "hal_token.h"
#include <stdbool.h>
#include <stdint.h>
#include "ohos_errno.h"
#include "ohos_types.h"
#include "hi_flash.h"

#define BITS_PER_BYTE 8

#define SECTOR_ALIGN_BYTES 4096

// token's max length is 151 Bytes, we using 256 Bytes to erase each token aera
#define MAX_TOKEN_AREA_SIZE 256
#define TOKEN_SIZE 151
// 4 Bytes for token flag
// if token's both area are available, when read token, always return area which flag is bigger;
// and recover area which flag is small while write token.
#define TOKEN_FLAG_SIZE 4

#define TOKEN_WITH_FLAG_SIZE (TOKEN_SIZE + TOKEN_FLAG_SIZE)

// 4 Bytes for token magic number, if data not in {0x01, 0x02, 0x03, 0x04} order means the token area is not initialled.
// if token area is initialled, the token magic number's next Byte data is token actual value.
static const char g_tokenMagicNum[] = {1, 2, 3, 4};
#define TOKEN_MAGIC_NUM_SIZE (sizeof(g_tokenMagicNum)/sizeof(g_tokenMagicNum[0]))

// token flag offset address 
#define TOKEN_ADDR 0x1E3000

// token area A offset address 
#define TOKEN_A_ADDR TOKEN_ADDR

// token area B offset address 
#define TOKEN_B_ADDR (TOKEN_A_ADDR + SECTOR_ALIGN_BYTES)

/* *
 * @brief Write token raw data to the flash, and this function is only for token read and write.
 * @param start The start address of write area on flash
 * @param tokenRawData The token raw data need to be wrote.
 * @param len The lenght of the data need to be wrote.
 * @returns 0 if it succeed and write data to the flash, others value is falied.
 */
static int FlashWriteTokenRawData(unsigned int start, const char* tokenRawData, unsigned int len)
{
    if (start % SECTOR_ALIGN_BYTES != 0) {
        printf("Token : Unsupport address not align yet, may cause data overlap error\n");
        return -1;
    }
    if (tokenRawData == NULL || len <= 0) {
        printf("Token : Invalid input of flash write\n");
        return -1;
    }
    if (hi_flash_write(start, len, tokenRawData, 1) != 0) {
        printf("Token : Write data to falsh failed\n");
        return -1;
    }
    return 0;
}

/* *
 * @brief Erase the flash  partition,
 * and this function is only for token read and write.
 * @param start The start address of erase area  on flash
 * @param size The size need to be erased on flash in Bytes
 * @returns    0 if it succeeds and erase the area with data 0;
 * -1 if it fails, the actual data on the erase area is unknown.
 */
static int HalTokenFlashErase(unsigned int start, unsigned int size)
{
    if (start % SECTOR_ALIGN_BYTES != 0) {
        printf("Token : Unsupport start address not align yet, may cause data overlap error\n");
        return -1;
    }
    if (size <= 0) {
        printf("Token : Invalid size\n");
        return -1;
    }
    char buff[MAX_TOKEN_AREA_SIZE] = {0};
    if (hi_flash_write(start, sizeof(buff), buff, 1) != 0) {
        printf("Token : Erase flash failed\n");
        return -1;
    }
    return 0;
}

/* *
 * @brief Init the token area with data 0 when token flag head is not 0x1,0x2,0x3,0x4 order
 * @returns 0 if it succeeds and fill the token area with data 0;
 * -1 if it fails, the actual data on the token area is unknown.
 */
int HalTokenFlashInit()
{
    // init MAX_TOKEN_AREA_SIZE Bytes flash to save token or flag as token max length is 151 Bytes
    // and we do not need erase full size of the partition
    unsigned int addrs[] = {TOKEN_A_ADDR, TOKEN_B_ADDR};
    for (int i = 0; i < sizeof(addrs) / sizeof(addrs[0]); i++) {
        if (HalTokenFlashErase(addrs[i], MAX_TOKEN_AREA_SIZE) != 0) {
            printf("Token : Init token area %d failed\n", i);
            return -1;
        }
    }
    return 0;
}

/* *
 * @brief Read data from the flash, and this function is only for token read and write.
 * @param start The start address of erase area on flash
 * @param tokenRawData The raw token data need to be read.
 * @param len The length of the data need to be wrote.
 * @returns    0 if it succeeds and erase the area with data 0;
 * -1 if it fails, the actual data on the ease area is unknown.
 */
static int FlashReadTokenRawData(unsigned int start, char* tokenRawData, unsigned int len)
{
    if (start % SECTOR_ALIGN_BYTES != 0) {
        printf("Token : Unsupport start address not align yet, may cause data overlap error.\n");
        return -1;
    }
    if (tokenRawData == NULL || len <= 0) {
        printf("Token : Invalid input of flash read.\n");
        return -1;
    }

    if (hi_flash_read(start, len, tokenRawData) != 0) {
        printf("Token : Flash read failed\n");
        return -1;
    }

    return 0;
}

static int ReadTokenWithFlag(unsigned int start, char* result, unsigned int len)
{
    const unsigned int buffLen = TOKEN_MAGIC_NUM_SIZE + TOKEN_WITH_FLAG_SIZE + 1;
    char buf[buffLen];
    (void)memset_s(buf, buffLen, 0, buffLen);

    if (FlashReadTokenRawData(start, buf, buffLen) != 0) {
        printf("Token : Read flash token area failed\n");
        return -1;
    }

    int isTokenValid = 1;
    // check is initialed or not
    for (unsigned int i = 0; i < TOKEN_MAGIC_NUM_SIZE; i++) {
        if (buf[i] != g_tokenMagicNum[i]) {
            isTokenValid = 0;
            break;
        }
    }

    if (isTokenValid == 0) {
        // token area isn't valid
        printf("The token area is invalid\n");
        return -1;
    }
    (void)memcpy_s(result, TOKEN_WITH_FLAG_SIZE, buf + TOKEN_MAGIC_NUM_SIZE, TOKEN_WITH_FLAG_SIZE);
    return 0;
}

static int WriteTokenWithFlag(unsigned int start, const char* tokenWithFlag, unsigned int len)
{
    const unsigned int buffLen = TOKEN_MAGIC_NUM_SIZE + TOKEN_WITH_FLAG_SIZE + 1;
    char buf[buffLen];
    (void)memset_s(buf, buffLen, 0, buffLen);
    
    for (unsigned int i = 0; i < TOKEN_MAGIC_NUM_SIZE; i++) {
        buf[i] = g_tokenMagicNum[i];
    }
    (void)memcpy_s(buf + TOKEN_MAGIC_NUM_SIZE, TOKEN_WITH_FLAG_SIZE, tokenWithFlag, TOKEN_WITH_FLAG_SIZE);
    if (FlashWriteTokenRawData(start, buf, buffLen) != 0) {
        printf("Token : Write flash token area failed.\n");
        return -1;
    }
    return 0;
}

static uint32_t GetTokenFlag(const char tokenWithFlag[])
{
    uint32_t result = 0;
    for (unsigned int i = 0; i < TOKEN_FLAG_SIZE; i++) {
        result |= ((uint8_t)tokenWithFlag[TOKEN_SIZE + i]) << ((TOKEN_FLAG_SIZE - 1 - i) * BITS_PER_BYTE);
    }
    return result;
}

static void SetTokenFlag(unsigned char flag[], uint32_t value)
{
    for (unsigned int i = 0; i < TOKEN_FLAG_SIZE; i++) {
        flag[i] = (value >> (BITS_PER_BYTE * (TOKEN_FLAG_SIZE - 1 - i))) & 0xFF;
    }
}

/* *
 * @brief Read token value from the flash token A or B area, and this function is only for token read and write.
 *
 * @param token The data buffer malloced by caller.
 * @param len The data buffer length.
 *
 * @returns    -1 if it fails, the actual data is unknown.
 * 0 if it succeeds and means read token from area A or area B's data.
 * -2 if it succeeds and means current is no token exist on the device.
 */
static int OEMReadToken(char* token, unsigned int len)
{
    if (token == NULL || len < 0) {
        return -1;
    }
    char tokenWithFlagA[TOKEN_WITH_FLAG_SIZE] = {0};
    char tokenWithFlagB[TOKEN_WITH_FLAG_SIZE] = {0};
    int32_t retA = ReadTokenWithFlag(TOKEN_A_ADDR, tokenWithFlagA, TOKEN_WITH_FLAG_SIZE);
    int32_t retB = ReadTokenWithFlag(TOKEN_B_ADDR, tokenWithFlagB, TOKEN_WITH_FLAG_SIZE);

    if ((retA != 0) && (retB != 0)) {
        printf("Read Token : Using device info to generate online token.\n");
        // -2 means current is no token exist on the device
        return -2;
    } else if ((retA == 0) && (retB != 0)) {
        // token area A has data, area B is NULL, return A;
        printf("DEBUG: token area A has data, area B is NULL, return A\n");
        (void)memcpy_s(token, len, tokenWithFlagA, len);
        return 0;
    } else if ((retA != 0) && (retB == 0)) {
        // token area B has data, area A is NULL, return B;
        printf("DEBUG: token area B has data, area A is NULL, return B\n");
        (void)memcpy_s(token, len, tokenWithFlagB, len);
        return 0;
    } else {
        // token area A and B both have data, return area which flag is larger than the other one.
        uint32_t flagA = GetTokenFlag(tokenWithFlagA);
        uint32_t flagB = GetTokenFlag(tokenWithFlagB);
        printf("DEBUG: FlashReadToken, the flagA is %d, flagB is %d\n", flagA, flagB);
        if (flagA > flagB) {
            (void)memcpy_s(token, len, tokenWithFlagA, len);
            return 0;
        } else {
            (void)memcpy_s(token, len, tokenWithFlagB, len);
            return 0;
        }
    }
}

/* *
 * @brief Write token value to the token A or B area on the flash, and this function is only for token read and write.
 *
 * @param token The input token data.
 * @param len The token's length.
 *
 * @returns    -1 if it fails, write token failed.
 * 0 if it succeeds and means write token to area A or area B's data.
 */
static int OEMWriteToken(const char* token, unsigned int len)
{
    if ((token == NULL) || (len < 0)) {
        return -1;
    }
    char tokenWithFlagA[TOKEN_WITH_FLAG_SIZE] = {0};
    char tokenWithFlagB[TOKEN_WITH_FLAG_SIZE] = {0};
    int32_t retA = ReadTokenWithFlag(TOKEN_A_ADDR, tokenWithFlagA, TOKEN_WITH_FLAG_SIZE);
    int32_t retB = ReadTokenWithFlag(TOKEN_B_ADDR, tokenWithFlagB, TOKEN_WITH_FLAG_SIZE);

    if ((retA != 0) && (retB != 0)) {
        // no token on the device, write token to A area;
        printf("Write Token : no token data on device\n");
        unsigned char flag[TOKEN_FLAG_SIZE] = {0};
        if ((memcpy_s(tokenWithFlagA, TOKEN_WITH_FLAG_SIZE, token, len) != 0) ||
            (memcpy_s(tokenWithFlagA + len, TOKEN_WITH_FLAG_SIZE - len, flag, TOKEN_FLAG_SIZE) != 0)) {
            return -1;
        }
        if (WriteTokenWithFlag(TOKEN_A_ADDR, tokenWithFlagA, TOKEN_WITH_FLAG_SIZE) != 0) {
            printf("Write Token : flash write token area A failed.\n");
            return -1;
        }
        return 0;
    } else if ((retA == 0) && (retB != 0)) {
        // token area A has data, area B is NULL, write token to B area;
        (void)memset_s(tokenWithFlagB, TOKEN_WITH_FLAG_SIZE, 0, TOKEN_WITH_FLAG_SIZE);
        uint32_t flagA = GetTokenFlag(tokenWithFlagA);
        unsigned char flag[TOKEN_FLAG_SIZE] = {0};        
        SetTokenFlag(flag, (uint32_t)(flagA + 1));
        if ((memcpy_s(tokenWithFlagB, TOKEN_WITH_FLAG_SIZE, token, len) != 0) ||
            (memcpy_s(tokenWithFlagB + len, TOKEN_WITH_FLAG_SIZE, flag, TOKEN_FLAG_SIZE) != 0)) {
            return -1;
        }
        if (WriteTokenWithFlag(TOKEN_B_ADDR, tokenWithFlagB, TOKEN_WITH_FLAG_SIZE) != 0) {
            printf("Write Token : flash write token area B failed.\n");
            return -1;
        }
        return 0;
    } else if ((retA != 0) && (retB == 0)) {
        // token area B has data, area A is NULL, write token to A area;
        (void)memset_s(tokenWithFlagA, TOKEN_WITH_FLAG_SIZE, 0, TOKEN_WITH_FLAG_SIZE);
        uint32_t flagB = GetTokenFlag(tokenWithFlagB);
        unsigned char flag[TOKEN_FLAG_SIZE] = {0};
        SetTokenFlag(flag, (uint32_t)(flagB + 1));
        if ((memcpy_s(tokenWithFlagA, TOKEN_WITH_FLAG_SIZE, token, len) != 0) ||
            (memcpy_s(tokenWithFlagA + len, TOKEN_WITH_FLAG_SIZE, flag, TOKEN_FLAG_SIZE) != 0)) {
            return -1;
        }
        if (WriteTokenWithFlag(TOKEN_A_ADDR, tokenWithFlagA, TOKEN_WITH_FLAG_SIZE) != 0) {
            printf("Write Token : flash write token area A failed.\n");
            return -1;
        }
        return 0;
    } else {
        // token area A and B both have data, write token to the area which flag is smaller than the other one.
        uint32_t flagA = GetTokenFlag(tokenWithFlagA);
        uint32_t flagB = GetTokenFlag(tokenWithFlagB);
        if (flagA > flagB) {
            // area A's token is new, recover area B;
            (void)memset_s(tokenWithFlagB, TOKEN_WITH_FLAG_SIZE, 0, TOKEN_WITH_FLAG_SIZE);
            unsigned char flag[TOKEN_FLAG_SIZE] = {0};
            SetTokenFlag(flag, (uint32_t)(flagA + 1));
            if ((memcpy_s(tokenWithFlagB, TOKEN_WITH_FLAG_SIZE, token, len) != 0) ||
                (memcpy_s(tokenWithFlagB + len, TOKEN_WITH_FLAG_SIZE, flag, TOKEN_FLAG_SIZE) != 0)) {
                return -1;
            }
            if (WriteTokenWithFlag(TOKEN_B_ADDR, tokenWithFlagB, TOKEN_WITH_FLAG_SIZE) != 0) {
                printf("Write Token : flash write token area B failed.\n");
                return -1;
            }
            return 0;
        } else {
            // area B's token is new, recover area A;
            (void)memset_s(tokenWithFlagA, TOKEN_WITH_FLAG_SIZE, 0, TOKEN_WITH_FLAG_SIZE);
            unsigned char flag[TOKEN_FLAG_SIZE] = {0};
            SetTokenFlag(flag, (uint32_t)(flagB + 1));
            if ((memcpy_s(tokenWithFlagA, TOKEN_WITH_FLAG_SIZE, token, len) != 0) ||
                (memcpy_s(tokenWithFlagA + len, TOKEN_WITH_FLAG_SIZE, flag, TOKEN_FLAG_SIZE) != 0)) {
                return -1;
            }
            if (WriteTokenWithFlag(TOKEN_A_ADDR, tokenWithFlagA, TOKEN_WITH_FLAG_SIZE) != 0) {
                printf("Write Token : flash write token area A failed.\n");
                return -1;
            }
            return 0;
        }
    }
}

static int OEMGetAcKey(char *acKey, unsigned int len)
{
    // OEM need add here, get AcKey
    if ((acKey == NULL) || (len < 0)) {
        printf("Invalid input parameters\n");
        return -1;
    }
    char acKeyFromOS[] = {
    		0x62, 0x6e, 0x76, 0x53, 0x40, 0x2b, 0x22, 0x6e, 0x54, 0x3c, 0x31, 0x45,
			0x3e, 0x6b, 0x4c, 0x23, 0x23, 0xcb, 0x57, 0x3a, 0x7b, 0x1c, 0x38, 0xb6,
			0x53, 0xe9, 0x04, 0xe3, 0x2b, 0x00, 0x6c, 0x73, 0x9b, 0x4b, 0x46, 0x26,
			0x54, 0x4c, 0x65, 0x13, 0xc9, 0x07, 0x85, 0xf7, 0xde, 0xd5, 0x43, 0x70
    };
    if (memcpy_s(acKey, len, acKeyFromOS, sizeof(acKeyFromOS)) != 0) {
        return -1;
    }
    return 0;
}

static int OEMGetProdId(char *productId, unsigned int len)
{
    if ((productId == NULL) || (len < 0)) {
        printf("Invalid input parameters\n");
        return -1;
    }
    // OEM need add product Id here,
    const char* ProdId = "2705";
    if (memcpy_s(productId, len, ProdId, strlen(ProdId)) != 0) {
        return -1;
    }
    return 0;
}

static int OEMGetProdKey(char *productKey, unsigned int len)
{
    // OEM need add here, get ProdKey
    // char productKeyBuf[] = "1234567890ABCDEF1234567890ABCDEF";
    char productKeyBuf[] = "dba510ffde0e4d8aa1d2021ad8fa1bc7";
    if (len > sizeof(productKeyBuf)) {
        return EC_FAILURE;
    }

    (void)memcpy(productKey, productKeyBuf, len);

    return 0;
}

int HalReadToken(char *token, unsigned int len)
{
    if (token == NULL) {
        return EC_FAILURE;
    }

    return OEMReadToken(token, len);
}

int HalWriteToken(const char *token, unsigned int len)
{
    if (token == NULL) {
        return EC_FAILURE;
    }

    return OEMWriteToken(token, len);
}

int HalGetAcKey(char *acKey, unsigned int len)
{
    if (acKey == NULL) {
        return EC_FAILURE;
    }

    return OEMGetAcKey(acKey, len);
}

int HalGetProdId(char *productId, unsigned int len)
{
    if (productId == NULL) {
        return EC_FAILURE;
    }

    return OEMGetProdId(productId, len);
}

int HalGetProdKey(char *productKey, unsigned int len)
{
    if (productKey == NULL) {
        return EC_FAILURE;
    }

    return OEMGetProdKey(productKey, len);
}

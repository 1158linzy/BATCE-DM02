/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_DISTRIBUTEDSCHEDULE_MSGPACK_H
#define OHOS_DISTRIBUTEDSCHEDULE_MSGPACK_H

#include "dmslite_tlv_common.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    COMMAND_ID = 1,
    CALLEE_BUNDLE_NAME = 2,
    CALLEE_ABILITY_NAME = 3,
    CALLER_SIGNATURE = 4,
    DMS_VERSION = 5
} FieldType;

bool PreprareBuild();
bool MarshallUint8(uint8_t field, FieldType fieldType);
bool MarshallUint16(uint16_t field, FieldType fieldType);
bool MarshallUint32(uint32_t field, FieldType fieldType);
bool MarshallUint64(uint64_t field, FieldType fieldType);
bool MarshallInt8(int8_t field, FieldType fieldType);
bool MarshallInt16(int16_t field, FieldType fieldType);
bool MarshallInt32(int32_t field, FieldType fieldType);
bool MarshallInt64(int64_t field, FieldType fieldType);
bool MarshallString(const char* field, uint8_t type);
uint16_t GetPacketSize();
char* GetPacketBufPtr();
void CleanBuild();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* OHOS_DISTRIBUTEDSCHEDULE_MSGPACK_H */

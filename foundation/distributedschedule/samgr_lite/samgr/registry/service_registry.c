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
#include "service_registry.h"
#include <ohos_errno.h>

int __attribute__((weak)) SAMGR_RegisterServiceApi(const char *service, const char *feature,
                                                   const Identity *identity, IUnknown *iUnknown)
{
    (void)service;
    (void)feature;
    (void)iUnknown;
    (void)identity;
    return EC_FAILURE;
}

IUnknown *__attribute__((weak)) SAMGR_FindServiceApi(const char *service, const char *feature)
{
    (void)service;
    (void)feature;
    return NULL;
}

int32 __attribute__((weak)) SAMGR_RegisterSystemCapabilityApi(const char *sysCap, BOOL isReg)
{
    (void)sysCap;
    (void)isReg;
    return EC_FAILURE;
}

BOOL __attribute__((weak)) SAMGR_QuerySystemCapabilityApi(const char *sysCap)
{
    (void)sysCap;
    return FALSE;
}

int32 __attribute__((weak)) SAMGR_GetSystemCapabilitiesApi(char sysCaps[MAX_SYSCAP_NUM][MAX_SYSCAP_NAME_LEN],
                                                           int32 *size)
{
    (void)sysCaps;
    (void)size;
    return EC_FAILURE;
}
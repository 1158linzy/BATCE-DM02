/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: implement of oem_auth_config
 * Author: Kit Framework group
 * Create: 2020-12-26
 */

#include "oem_auth_config.h"

#include <stdio.h>
#include <stdlib.h>
#include "securec.h"

#include "cJSON.h"

#define KIT_INFO_JSON_KEY "KitInfos"
#define KIT_INFO_PAIR_KEY 0
#define KIT_INFO_PAIR_VAL 1
#define KIT_INFO_PAIR_LEN 2

static const char* KitInfos[][KIT_INFO_PAIR_LEN] = {
    {"HiLinkKit", "1.0.0"},
};

int32_t OEMReadAuthServerInfo(char* buff, uint32_t len)
{
    return -1;
}

char* OEMLoadKitInfos(void)
{
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        return NULL;
    }
    cJSON* infos = cJSON_CreateArray();
    if (infos == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    const uint32_t kitCount = sizeof(KitInfos) / (sizeof(char*) * KIT_INFO_PAIR_LEN);
    for (uint32_t i = 0; i < kitCount; i++) {
        cJSON* obj = cJSON_CreateObject();
        if (obj == NULL) {
            printf("OEM Adapter: Create json object failed\n");
            continue;
        }
        const char* key = KitInfos[i][KIT_INFO_PAIR_KEY];
        const char* value = KitInfos[i][KIT_INFO_PAIR_VAL];
        if (key == NULL || value == NULL || cJSON_AddStringToObject(obj, key, value) == NULL) {
            printf("OEM Adapter: Add kit info %s to json object failed\n", key);
            cJSON_Delete(obj);
            continue;
        }
        if (!cJSON_AddItemToArray(infos, obj)) {
            cJSON_Delete(obj);
            printf("OEM Adapter: Add kit info %s to json array failed\n", key);
        }
    }

    if (!cJSON_AddItemToObject(root, KIT_INFO_JSON_KEY, infos)) {
        printf("OEM Adapter: Add kit infos to json object failed\n");
        cJSON_Delete(infos);
        cJSON_Delete(root);
        return NULL;
    }
    char* result = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return result;
}

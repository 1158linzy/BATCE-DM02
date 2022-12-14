/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
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
#include "frame_config.h"
#include "media_log.h"

using namespace std;

namespace OHOS {
namespace Media {
int32_t FrameConfig::GetFrameConfigType()
{
    return type_;
}

list<Surface *> FrameConfig::GetSurfaces()
{
    return surfaceList_;
}

void FrameConfig::AddSurface(Surface &surface)
{
    surfaceList_.emplace_back(&surface);
}

void FrameConfig::RemoveSurface(Surface &surface)
{
    surfaceList_.remove(&surface);
}

void *FrameConfig::GetValue(uint32_t key)
{
    auto p = keyMap_.find(key);
    return (p == keyMap_.end()) ? nullptr : (&p->second);
}

void FrameConfig::SetValue(uint32_t key, const void *value)
{
    switch (key) {
        case PARAM_KEY_IMAGE_ENCODE_QFACTOR:
            keyMap_[key] = *(static_cast<const int32_t *>(value));
            break;
        default:
            break;
    }
}

FrameConfig::FrameConfig(int32_t type) : type_(type) {}
} // namespace Media
} // namespace OHOS
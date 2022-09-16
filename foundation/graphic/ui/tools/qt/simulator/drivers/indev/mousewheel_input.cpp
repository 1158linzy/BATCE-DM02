﻿/*
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
#include "mousewheel_input.h"
#include <QtCore/qcoreevent.h>
#include <QtCore/qvariant.h>
#include <qdebug.h>

static int16_t g_rotate = 0;

namespace OHOS {
#if USE_MOUSEWHEEL
#if ENABLE_ROTATE_INPUT
bool MousewheelInput::Read(DeviceData &data)
{
    data.rotate = g_rotate;
    g_rotate = 0;
    return false;
}
void MousewheelInput::MousewheelHandler(QWheelEvent *event)
{
    if (event == nullptr) {
        return;
    }
    g_rotate += -event->delta() / 120; // 120 : Qt use 120 as one mousewheel step value
}
#endif
#endif
} // namespace OHOS

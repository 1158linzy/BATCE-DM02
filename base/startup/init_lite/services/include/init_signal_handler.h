/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BASE_STARTUP_INITLITE_SIGNAL_HANDLE_H
#define BASE_STARTUP_INITLITE_SIGNAL_HANDLE_H

#ifdef __LINUX__
#include <semaphore.h>
#include <sys/types.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

void SignalInitModule();

#ifdef __LINUX__
void SignalRegWaitSem(pid_t waitPid, sem_t* waitSem);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // BASE_STARTUP_INITLITE_SIGNAL_HANDLE_H

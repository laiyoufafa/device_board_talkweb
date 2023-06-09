/*
 * Copyright (c) 2022 Talkweb Co., Ltd.
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

#ifndef __OHOS_RUN_H__
#define __OHOS_RUN_H__

#include "los_task.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BEFORE_OHOS_RUN(func) \
void before_ohos_run(void) { \
    func(); \
}

#define OHOS_APP_RUN(func) \
void ohos_app_main(void) { \
    LOS_TaskDelay(100); \
    printf("\n\033[1;32m<--------------- OHOS Application Start Here --------------->\033[0m\n"); \
    func(); \
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __OHOS_RUN_H__ */
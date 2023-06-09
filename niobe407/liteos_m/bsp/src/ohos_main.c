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
#include <stdio.h>
#include "los_task.h"
#include "los_compiler.h"
#include "cmsis_os2.h"
#include "samgr_lite.h"
#include "ohos_init.h"
#include "ohos_types.h"
#include "watch_dog.h"

#define TALKWEB_SERVICE_STACKSIZE       (4096)
#define TALKWEB_SERVICE_TASK_PRIOR      26
#define TALKWEB_SERVICE_TASK_NAME       "talkweb_sys_service"
#define TALKWEB_SERVICE_TASK_DELAY      1000

__attribute__((weak)) void ohos_app_main(void)
{
    printf("No application run, Maybe you should config your application in BUILD.gn!\n");
    return;
}

__attribute__((weak)) void before_ohos_run(void)
{
    return;
}

static void talkweb_sys_service(void)
{
    sys_service_config();

    ohos_app_main();

    while (1) {
        feed_dog();
        LOS_TaskDelay(TALKWEB_SERVICE_TASK_DELAY);
    }
}

static void OHOS_Main(void)
{
    UINT32 uwRet = -1;
    UINT32 taskID = 0;
    TSK_INIT_PARAM_S stTask = {0};
    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)talkweb_sys_service;
    stTask.uwStackSize = TALKWEB_SERVICE_STACKSIZE;
    stTask.pcName = TALKWEB_SERVICE_TASK_NAME;
    stTask.usTaskPrio = TALKWEB_SERVICE_TASK_PRIOR; /* Os task priority is 6 */
    uwRet = LOS_TaskCreate(&taskID, &stTask);
    if (uwRet != LOS_OK) {
        printf("Task1 create failed\n");
    }
}

/**
 * @brief This is the ohos entry, and you could call this in your main funciton after the
 *        necessary hardware has been initialized.
 */
void OHOS_Boot(void)
{
    UINT32 ret;

    before_ohos_run();

    ret = LOS_KernelInit();
    if (ret == LOS_OK) {
        OHOS_SystemInit();
        LOS_Start();
    }
    return;  // and should never come here
}

SYS_RUN(OHOS_Main);
/*
 * Copyright (c) 2021-2022 Talkweb Co., Ltd.
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
#include <string.h>
#include <unistd.h>

#include "ohos_run.h"
#include "cmsis_os2.h"
#include "rng_init.h"

extern RNG_HandleTypeDef hrng;

void thread_entry(void)
{
    unsigned int rng_num = 0;
    RngInit(); //RNG模块初始化
    while (1)
    {
        if(HAL_OK == HAL_RNG_GenerateRandomNumber(&hrng, &rng_num))
           printf("Generate 32bit Random number is %u\r\n", rng_num);		
        else
           printf("Generate Random Number fail!\r\n");	
        osDelay(1000);
    }
}

static void get_ramdom_number_example(void)
{
    osThreadAttr_t attr;
    
    attr.name = "thread1";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024 * 4;
    attr.priority = 25;

    if (osThreadNew((osThreadFunc_t)thread_entry, NULL, &attr) == NULL)
    {
        printf("Falied to create thread!\n");
    }
}

OHOS_APP_RUN(get_ramdom_number_example);

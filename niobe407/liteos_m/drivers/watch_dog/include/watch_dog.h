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

#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_iwdg.h"

#define WATCHDOG_MIN_TIMEOUT 1
#define WATCHDOG_MAX_TIMEOUT 4096

int watch_dog_init(unsigned int timeout);

void feed_dog();

#endif /* __WATCH_DOG_H__ */
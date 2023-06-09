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

#ifndef __CAN_INIT_H__
#define __CAN_INIT_H__

#ifdef __cplusplus
extern "C " {
#endif /* __cplusplus */

extern CAN_HandleTypeDef hcan1;

void MX_CAN1_Init(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CAN_INIT_H__ */
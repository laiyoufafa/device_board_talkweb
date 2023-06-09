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
#include "hdf_base_hal.h"

#define GPIO_ERR 0XFFFFFFFF
#define GPIO_REGISTER_TAG 0XA5
#define GPIO_REMOVE_TAG 0X00

static unsigned char g_GpioRegisterCache[NIOBE_GPIO_PORT_MAX][NIOBE_GPIO_PIN_MAX] = {0};

static const unsigned int HDF_LL_GPIO_PORT_MAP[NIOBE_GPIO_PORT_MAX] = {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI
};

static const unsigned int HDF_LL_GPIO_PIN_MAP[NIOBE_GPIO_PIN_MAX] = {
    LL_GPIO_PIN_0,
    LL_GPIO_PIN_1,
    LL_GPIO_PIN_2,
    LL_GPIO_PIN_3,
    LL_GPIO_PIN_4,
    LL_GPIO_PIN_5,
    LL_GPIO_PIN_6,
    LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_9,
    LL_GPIO_PIN_10,
    LL_GPIO_PIN_11,
    LL_GPIO_PIN_12,
    LL_GPIO_PIN_13,
    LL_GPIO_PIN_14,
    LL_GPIO_PIN_15
};

static const unsigned int HDF_LL_GPIO_CLOCK_MAP[NIOBE_GPIO_PORT_MAX] = {
    LL_AHB1_GRP1_PERIPH_GPIOA,
    LL_AHB1_GRP1_PERIPH_GPIOB,
    LL_AHB1_GRP1_PERIPH_GPIOC,
    LL_AHB1_GRP1_PERIPH_GPIOD,
    LL_AHB1_GRP1_PERIPH_GPIOE,
    LL_AHB1_GRP1_PERIPH_GPIOF,
    LL_AHB1_GRP1_PERIPH_GPIOG,
    LL_AHB1_GRP1_PERIPH_GPIOH,
    LL_AHB1_GRP1_PERIPH_GPIOI
};

static const unsigned int HDF_LL_GPIO_MODE_MAP[NIOBE_GPIO_MODE_MAX] = {
    LL_GPIO_MODE_INPUT,
    LL_GPIO_MODE_OUTPUT,
    LL_GPIO_MODE_ALTERNATE,
    LL_GPIO_MODE_ANALOG
};

static const unsigned int HDF_LL_GPIO_SPEED_MAP[NIOBE_GPIO_SPEED_MAX] = {
    LL_GPIO_SPEED_FREQ_LOW,
    LL_GPIO_SPEED_FREQ_MEDIUM,
    LL_GPIO_SPEED_FREQ_HIGH,
    LL_GPIO_SPEED_FREQ_VERY_HIGH
};

static const unsigned int HDF_LL_GPIO_OUTPUTTYPE_MAP[NIOBE_GPIO_OUTPUTTYPE_MAX] = {
    LL_GPIO_OUTPUT_PUSHPULL,
    LL_GPIO_OUTPUT_OPENDRAIN
};

static const unsigned int HDF_LL_GPIO_PULL_MAP[NIOBE_GPIO_PULL_MAX] = {
    LL_GPIO_PULL_NO,
    LL_GPIO_PULL_UP,
    LL_GPIO_PULL_DOWN
};

static const unsigned int HDF_LL_GPIO_ALTERNATE_MAP[ALTERNATE_MAX] = {
    LL_GPIO_AF_0,
    LL_GPIO_AF_1,
    LL_GPIO_AF_2,
    LL_GPIO_AF_3,
    LL_GPIO_AF_4,
    LL_GPIO_AF_5,
    LL_GPIO_AF_6,
    LL_GPIO_AF_7,
    LL_GPIO_AF_8,
    LL_GPIO_AF_9,
    LL_GPIO_AF_10,
    LL_GPIO_AF_11,
    LL_GPIO_AF_12,
    LL_GPIO_AF_13,
    LL_GPIO_AF_14,
    LL_GPIO_AF_15
};

static bool GpioUseRegister(GPIO_PORT_MAP port, GPIO_PIN_MAP pin)
{
    if (port >= NIOBE_GPIO_PORT_MAX) {
        HDF_LOGE("NiobeGpioRegister param[port] match fail\r\n");
        return false;
    }

    if (pin >= NIOBE_GPIO_PIN_MAX) {
        HDF_LOGE("NiobeGpioRegister param[pin] match fail\r\n");
        return false;
    }

    if (g_GpioRegisterCache[port][pin] == GPIO_REGISTER_TAG) {
        HDF_LOGE("ERR: NiobeGpioRegister clash, port_pin = [%d, %d]\r\n", port, pin);
        return false;
    }

    g_GpioRegisterCache[port][pin] = GPIO_REGISTER_TAG;
    return true;
}

static bool GpioUseRemove(GPIO_PORT_MAP port, GPIO_PIN_MAP pin)
{
    if (port >= NIOBE_GPIO_PORT_MAX) {
        HDF_LOGE("GpioUseRemove param[port] match fail\r\n");
        return false;
    }

    if (pin >= NIOBE_GPIO_PIN_MAX) {
        HDF_LOGE("GpioUseRemove param[pin] match fail\r\n");
        return false;
    }

    g_GpioRegisterCache[port][pin] = GPIO_REMOVE_TAG;
    return true;
}

static unsigned int GetLLGpioClkMatch(GPIO_PORT_MAP port)
{
    if (port >= NIOBE_GPIO_PORT_MAX) {
        HDF_LOGE("ERR: NiobeLLGpioClkMatch fail, port is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_CLOCK_MAP[port];
}

static unsigned int GetLLGpioPortMatch(GPIO_PORT_MAP port)
{
    if (port >= NIOBE_GPIO_PORT_MAX) {
        HDF_LOGE("ERR: NiobeLLGpioPortMatch fail, port is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_PORT_MAP[port];
}

static unsigned int GetLLGpioPinMatch(GPIO_PIN_MAP pin)
{
    if (pin >= NIOBE_GPIO_PIN_MAX) {
        HDF_LOGE("ERR: NiobeLLGpioPinMatch fail, pin is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_PIN_MAP[pin];
}

static unsigned int GetLLGpioModeMatch(GPIO_MODE_MAP mode)
{
    if (mode >= NIOBE_GPIO_MODE_MAX) {
        HDF_LOGE("ERR: GetLLGpioModeMatch fail, mode is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_MODE_MAP[mode];
}

static unsigned int GetLLGpioSpeedMatch(GPIO_SPEED_MAP speed)
{
    if (speed >= NIOBE_GPIO_SPEED_MAX) {
        HDF_LOGE("ERR: GetLLGpioSpeedMatch fail, speed is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_SPEED_MAP[speed];
}

static unsigned int GetLLGpioOutputTypeMatch(GPIO_OUTPUTTYPE_MAP outputType)
{
    if (outputType >= NIOBE_GPIO_OUTPUTTYPE_MAX) {
        HDF_LOGE("ERR: GetLLGpioOutputTypeMatch fail, outputType is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_OUTPUTTYPE_MAP[outputType];
}

static unsigned int GetLLGpioPullMatch(GPIO_PULL_MAP pull)
{
    if (pull >= NIOBE_GPIO_PULL_MAX) {
        HDF_LOGE("ERR: GetLLGpioPullMatch fail, pull is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_PULL_MAP[pull];
}

static unsigned int GetLLGpioAlternateMatch(PIN_ALTERNATE_MAP alternate)
{
    if (alternate >= ALTERNATE_MAX) {
        HDF_LOGE("ERR: GetLLGpioAlternateMatch fail, alternate is match fail.\r\n");
        return GPIO_ERR;
    }

    return (unsigned int)HDF_LL_GPIO_ALTERNATE_MAP[alternate];
}

static bool MakeLLGpioInit(HDF_GPIO_ATTR *attr)
{
    if (attr == NULL) {
        HDF_LOGE("ERR: MakeLLGpioMatch param is NULL\r\n");
        return false;
    }
    if (GpioUseRegister(attr->port, attr->pin) != true) {
        return false;
    }
    unsigned int llClk = GetLLGpioClkMatch(attr->port);
    unsigned int llPort = GetLLGpioPortMatch(attr->port);
    unsigned int llPin = GetLLGpioPinMatch(attr->pin);
    unsigned int llMode = GetLLGpioModeMatch(attr->mode);
    unsigned int llSpeed = GetLLGpioSpeedMatch(attr->speed);
    unsigned int llOutputType = GetLLGpioOutputTypeMatch(attr->outputType);
    unsigned int llPull = GetLLGpioPullMatch(attr->pull);
    unsigned int llAlternate = GetLLGpioAlternateMatch(attr->alternate);
    if (llClk == GPIO_ERR || llPort == GPIO_ERR || llPin == GPIO_ERR || llMode == GPIO_ERR ||
        llSpeed == GPIO_ERR || llOutputType == GPIO_ERR || llPull == GPIO_ERR || llAlternate == GPIO_ERR) {
        return false;
    }

    LL_GPIO_InitTypeDef GPIO_Initstruct;
    GPIO_Initstruct.Pin = llPin;
    GPIO_Initstruct.Mode = llMode;
    GPIO_Initstruct.OutputType = llOutputType;
    GPIO_Initstruct.Pull = llPull;
    GPIO_Initstruct.Speed = llSpeed;
    GPIO_Initstruct.Alternate = llAlternate;
    LL_AHB1_GRP1_EnableClock(llClk);
    if (LL_GPIO_Init(llPort, &GPIO_Initstruct) == ERROR) {
        HDF_LOGE("[%s]: LL_GPIO_Init fail \r\n", __func__);
        return false;
    }
    return true;
}

bool NiobeInitGpioInit(const HDF_GPIO_ATTR* attr)
{
    if (attr == NULL) {
        HDF_LOGE("ERR: NiobeInitGpioInit param is NULL\r\n");
        return false;
    }

    if (MakeLLGpioInit(attr) == false) {
        HDF_LOGE("MakeLLGpioInit fail\r\n");
        return false;
    }

    return true;
}

bool NiobeHdfGpioInit(const struct DeviceResourceNode *resourceNode, struct DeviceResourceIface *dir)
{
    if ((resourceNode == NULL) || (dir == NULL)) {
        HDF_LOGE("ERR: NiobeHdfGpioInit param is NULL\r\n");
        return false;
    }

    char gpio_str[32] = {0};
    int gpio_num_max = 0;
    HDF_GPIO_ATTR gpioAttr = {0};
    struct DeviceResourceIface *gpioDir = dir;
    struct DeviceResourceNode *gpioNode = resourceNode;

    if (gpioDir->GetUint32(gpioNode, "gpio_num_max", &gpio_num_max, 0) != HDF_SUCCESS) {
        HDF_LOGE("i2c config gpio_num_max fail\r\n");
        return false;
    }

    if (gpio_num_max > GPIO_NUM_CONFIG_MAX) {
        HDF_LOGE("i2c config gpio_num_max is too much, gpio_num = %d, NUM_CONFIG_MAX = %d\r\n",
            gpio_num_max, GPIO_NUM_CONFIG_MAX);
        return false;
    }

    for (int i = 0; i < gpio_num_max; i++) {
        sprintf_s(gpio_str, sizeof(gpio_str)-1, "gpio_num_%d", i + 1);
        if (gpioDir->GetUint32Array(resourceNode, gpio_str, &gpioAttr,
            sizeof(gpioAttr)/sizeof(unsigned int), 0) != HDF_SUCCESS) {
            HDF_LOGE("i2c config %s fail\r\n", gpio_str);
            return false;
        }
        if (MakeLLGpioInit(&gpioAttr) == false) {
            HDF_LOGE("MakeLLGpioInit fail\r\n");
            return false;
        }
        memset_s(&gpioAttr, sizeof(gpioAttr), 0, sizeof(gpioAttr));
    }
    return true;
}

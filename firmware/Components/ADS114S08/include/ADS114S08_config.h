#pragma once

extern "C" {

#include "main.h"
#include "spi.h"
#include "tim.h"

namespace ADS114S08 {

const auto SPI{&hspi1};
const auto TIM{&htim4};
const auto START_PORT{ADC_START_GPIO_Port};
const auto START_PIN{ADC_START_Pin};
const auto DRDY_PORT{ADC_DRDY_GPIO_Port};
const auto DRDY_PIN{ADC_DRDY_Pin};
const auto RST_PORT{ADC_RST_GPIO_Port};
const auto RST_PIN{ADC_RST_Pin};

} // namespace ADS114S08
}
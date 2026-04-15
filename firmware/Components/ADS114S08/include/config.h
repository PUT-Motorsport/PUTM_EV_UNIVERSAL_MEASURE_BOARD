#pragma once

#include "main.h"

namespace ADS114S08 {

const auto SPI = reinterpret_cast<SPI_HandleTypeDef*>(SPI1);
const auto TIM = reinterpret_cast<TIM_HandleTypeDef*>(TIM4);
const auto START_PORT = reinterpret_cast<GPIO_TypeDef*>(ADC_START_GPIO_Port);
const auto START_PIN = ADC_START_Pin;
const auto DRDY_PORT = reinterpret_cast<GPIO_TypeDef*>(ADC_DRDY_GPIO_Port);
const auto DRDY_PIN = ADC_DRDY_Pin;
const auto RST_PORT = reinterpret_cast<GPIO_TypeDef*>(ADC_RST_GPIO_Port);
const auto RST_PIN = ADC_RST_Pin;

} // namespace ADS114S08
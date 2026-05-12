/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "spi.h"
#include "stm32g0xx_hal_gpio.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADS114S08.h"
#include "PUTM_EV_CAN_LIBRARY/include/can_driver.hpp"
#include <array>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

class Adc : public ADS114S08::Driver {
  public:
    enum class Status {
        STOP,
        IDLE,
        WAIT_FOR_DRDY,
        DATA_READY,
        WAIT_FOR_SPI,
        DECODE,
    };
    volatile Status status{Status::STOP};

    struct Channel {
        enum class Type : uint8_t {
            SINGLE_ENDED,
            DIFFERENTIAL,
        };
        enum class Voltage : uint8_t {
            VOLTAGE_3V3,
            VOLTAGE_5V,
            VOLTAGE_12V,
        };
        enum class Gain : uint8_t {
            GAIN_1,
            GAIN_5,
            GAIN_25,
            GAIN_100,
            GAIN_1000,
        };

        ADS114S08::INPMUX_Field input;
        Type type;
        Voltage voltage;
        Gain gain{Gain::GAIN_1};
        bool status{false};
        uint16_t raw_value{0};
    };

    Adc(ADS114S08::Driver driver_config) : ADS114S08::Driver{driver_config} {}

    int config_channel(ADS114S08::INPMUX_Field input, Channel::Voltage voltage,
                       Channel::Gain gain) {
        for(auto& e : channels) {
            if(e.input == input) {
                if(((voltage == Channel::Voltage::VOLTAGE_5V ||
                     voltage == Channel::Voltage::VOLTAGE_12V) &&
                    e.type == Channel::Type::SINGLE_ENDED))
                    return 1;
                else if(voltage == Channel::Voltage::VOLTAGE_3V3 &&
                        e.type == Channel::Type::DIFFERENTIAL)
                    return 1;
                else {
                    e.voltage = voltage;
                    e.gain = gain;
                    e.status = true;
                    active_channels_count++;
                    return 0;
                }
            }
        }
        return 1;
    }

    int deactivate_channel(ADS114S08::INPMUX_Field input) {
        for(auto& e : channels) {
            if(e.input == input) {
                e.status = false;
                active_channels_count--;
                if(active_channels_count <= 0) {
                    status = Status::STOP;
                }
                return 0;
            }
        }
        return 1;
    }

    int start() {
        if(active_channels_count <= 0) {
            return 1;
        }
        if(status == Status::STOP) {
            current_channel = 0;
            for(size_t i{}; i < channels.max_size(); i++) {
                if(channels[i].status == true) {
                    current_channel = i;
                    break;
                }
            }
            select_single_ended(channels[current_channel].input);

            switch(get_mode()) {
            case ADS114S08::DR_MODE_Field::CONTINUOUS_CONVERSION_MODE:
                start_continous_conversions();
                break;
            case ADS114S08::DR_MODE_Field::SINGLE_SHOT_CONVERSION_MODE:
                start_single_conversion();
                break;
            }
            status = Status::WAIT_FOR_DRDY;
            return 0;
        }
        return 1;
    }

    int next() {
        if(status == Status::IDLE) {
            do {
                current_channel = (current_channel + 1) % channels.max_size();
            } while(channels[current_channel].status == false);
            select_single_ended(channels[current_channel].input);

            switch(get_state()) {
            case Mode::CONTINUOUS_CONVERSION_MODE: {
                break;
            }
            case Mode::STANDBY_MODE: {
                start_single_conversion();
                break;
            }
            default:
                return 1;
            }
            status = Status::WAIT_FOR_DRDY;
            return 0;
        }
        return 1;
    }

    int read() {
        if(status == Status::DATA_READY) {
            status = Status::WAIT_FOR_SPI;
            if(data_read_IT() != HAL_OK)
                return 1;
            else
                return 0;
        }
        return 1;
    }

    uint16_t update() {
        if(status == Status::DECODE) {
            switch(get_state()) {
            case Mode::CONTINUOUS_CONVERSION_MODE: {
                channels[current_channel].raw_value = data_decode_IT();
                break;
            }
            case Mode::SINGLE_CONVERSION_MODE: {
                channels[current_channel].raw_value = data_decode_IT();
                break;
            }
            default:
                break;
            }
            status = Status::IDLE;
        }
        return channels[current_channel].raw_value;
    }

  private:
    std::array<Channel, 12> channels{
        {{ADS114S08::INPMUX_Field::AIN0, Channel::Type::SINGLE_ENDED},
         {ADS114S08::INPMUX_Field::AIN1, Channel::Type::SINGLE_ENDED},
         {ADS114S08::INPMUX_Field::AIN2, Channel::Type::SINGLE_ENDED},
         {ADS114S08::INPMUX_Field::AIN3, Channel::Type::SINGLE_ENDED},
         {ADS114S08::INPMUX_Field::AIN4, Channel::Type::SINGLE_ENDED},
         {ADS114S08::INPMUX_Field::AIN5, Channel::Type::SINGLE_ENDED},
         {ADS114S08::INPMUX_Field::AIN6, Channel::Type::DIFFERENTIAL},
         {ADS114S08::INPMUX_Field::AIN7, Channel::Type::DIFFERENTIAL},
         {ADS114S08::INPMUX_Field::AIN8, Channel::Type::DIFFERENTIAL},
         {ADS114S08::INPMUX_Field::AIN9, Channel::Type::DIFFERENTIAL},
         {ADS114S08::INPMUX_Field::AIN10, Channel::Type::DIFFERENTIAL},
         {ADS114S08::INPMUX_Field::AIN11, Channel::Type::DIFFERENTIAL}}};
    uint8_t current_channel{0};
    int8_t active_channels_count{0};
};

/* USER CODE END PTD */

/* Private define
 * ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro
 * -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables
 * ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

Adc adc{ADS114S08::Driver{&hspi1, ADC_START_GPIO_Port, ADC_START_Pin,
                          ADC_DRDY_GPIO_Port, ADC_DRDY_Pin, ADC_RST_GPIO_Port,
                          ADC_RST_Pin}};
/* USER CODE END PV */

/* Private function prototypes
 * -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code
 * ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_FDCAN1_Init();
    MX_TIM4_Init();
    MX_SPI1_Init();

    /* USER CODE BEGIN 2 */

    // Initialize CAN
    // putm_ev_can::CanDriver can_m{};

    // Initialize ADC
    adc.init();

    // Bypass PGA
    adc.config_pga(ADS114S08::PGA_EN_Field::POWERED_DOWN_AND_BYPASSED,
                   ADS114S08::PGA_GAIN_Field::GAIN_1);

    // Configure continuous conversion, 200 samples per second datarate and
    // adc internal clock
    adc.config_datarate(ADS114S08::DR_SEL_Field::SEL_200_SPS,
                        ADS114S08::DR_MODE_Field::SINGLE_SHOT_CONVERSION_MODE,
                        ADS114S08::DR_CLK_Field::INTERNAL_4_096MHZ);

    adc.start();
    uint16_t adc_raw_data{};

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        switch(adc.get_state()) {
        case Adc::Mode::SINGLE_CONVERSION_MODE: {
            switch(adc.status) {
            case Adc::Status::IDLE: {
                adc.next();
                break;
            }
            case Adc::Status::DATA_READY: {
                adc.read();
                break;
            }
            case Adc::Status::DECODE: {
                adc_raw_data = adc.update();
                break;
            }
            }
            break;
        }
        default:
            break;
        }
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified
     * parameters in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 8;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin == ADC_DRDY_Pin) {
        if(adc.status == Adc::Status::WAIT_FOR_DRDY) {
            adc.status = Adc::Status::DATA_READY;
        };
    }
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
    if(hspi == &hspi1) {
        if(adc.status == Adc::Status::WAIT_FOR_SPI)
            adc.status = Adc::Status::DECODE;
    }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return
     * state
     */
    __disable_irq();
    while(1) {
    }
    /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

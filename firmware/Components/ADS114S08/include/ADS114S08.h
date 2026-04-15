#pragma once

#include "stm32g0xx_hal.h"
#include <cstdint>

namespace ADS114S08 {

const auto SPI = reinterpret_cast<SPI_HandleTypeDef*>(SPI1);
const auto TIM = reinterpret_cast<TIM_HandleTypeDef*>(TIM4);
constexpr uint8_t NUM_REGISTERS = 18;

// Register addresses
constexpr uint8_t ID_ADDR = 0x00;
constexpr uint8_t STATUS_ADDR = 0x01;
constexpr uint8_t INPMUX_ADDR = 0x02;
constexpr uint8_t PGA_ADDR = 0x03;
constexpr uint8_t DATARATE_ADDR = 0x04;
constexpr uint8_t REF_ADDR = 0x05;
constexpr uint8_t IDACMAG_ADDR = 0x06;
constexpr uint8_t IDACMUX_ADDR = 0x07;
constexpr uint8_t VBIAS_ADDR = 0x08;
constexpr uint8_t SYS_ADDR = 0x09;
constexpr uint8_t OFCAL0_ADDR = 0x0A;
constexpr uint8_t OFCAL1_ADDR = 0x0B;
constexpr uint8_t OFCAL2_ADDR = 0x0C;
constexpr uint8_t FSCAL0_ADDR = 0x0D;
constexpr uint8_t FSCAL1_ADDR = 0x0E;
constexpr uint8_t FSCAL2_ADDR = 0x0F;
constexpr uint8_t GPIODAT_ADDR = 0x10;
constexpr uint8_t GPIOCON_ADDR = 0x11;

// Register default values
constexpr uint8_t REG_ID_DEFAULT = 0x04;
constexpr uint8_t REG_STATUS_DEFAULT = 0x80;
constexpr uint8_t REG_INPMUX = 0x01;
constexpr uint8_t REG_PGA_DEFAULT = 0x00;
constexpr uint8_t REG_BITRATE_DEFAULT = 0x14;
constexpr uint8_t REG_REF_DEFAULT = 0x10;
constexpr uint8_t REG_IDACMAG_DEFAULT = 0x00;
constexpr uint8_t REG_IDACMUX_DEFAULT = 0xFF;
constexpr uint8_t REG_VBIAS_DEFAULT = 0x00;
constexpr uint8_t REG_SYS_DEFAULT = 0x10;
constexpr uint8_t REG_OFCAL0_DEFAULT = 0x00;
constexpr uint8_t REG_OFCAL1_DEFAULT = 0x00;
constexpr uint8_t REG_FSCAL0_DEFAULT = 0x00;
constexpr uint8_t REG_FSCAL1_DEFAULT = 0x40;
constexpr uint8_t REG_GPIODAT_DEFAULT = 0x00;
constexpr uint8_t REG_GPIOCON_DEFAULT = 0x00;

// Control Commands
constexpr uint8_t NOP_CMD = 0x00;
constexpr uint8_t WAKEUP_CMD = 0x02;
constexpr uint8_t POWERDOWN_CMD = 0x04;
constexpr uint8_t RESET_CMD = 0x06;
constexpr uint8_t START_CMD = 0x08;
constexpr uint8_t STOP_CMD = 0x0A;
constexpr uint8_t SYOCAL_CMD = 0x16;
constexpr uint8_t SYGCAL_CMD = 0x17;
constexpr uint8_t SFOCAL_CMD = 0x19;
constexpr uint8_t RDATA_CMD = 0x12;
constexpr uint8_t RREG_CMD = 0x20;
constexpr uint8_t WREG_CMD = 0x40;

// 00h ID xxh RESERVED DEV_ID[2:0]
union ID {
    uint8_t Value;
    struct {
        uint8_t DEV_ID : 3;
        uint8_t RESERVED : 5;
    } u;
};

// 01h STATUS 80h
union STATUS {
    uint8_t Value;
    struct {
        uint8_t FL_REF_L0 : 1;
        uint8_t RESERVED : 5;
        uint8_t RDY : 1;
        uint8_t FL_POR : 1;
    } u;
};

// 02h INPMUX
union INPMUX {
    uint8_t Value;
    struct {
        uint8_t MUXN : 4;
        uint8_t MUXP : 4;
    } u;
};

// 03h PGA
union PGA {
    uint8_t Value;
    struct {
        uint8_t GAIN : 3;
        uint8_t PGA_EN : 2;
        uint8_t RESERVED : 3;
    } u;
};

// 04h DATARATE
union DATARATE {
    uint8_t Value;
    struct {
        uint8_t DR : 4;
        uint8_t RESERVED0 : 1;
        uint8_t MODE : 1;
        uint8_t CLK : 1;
        uint8_t RESERVED1 : 1;
    } u;
};

// 05h REF
union REF {
    uint8_t Value;
    struct {
        uint8_t REFCON : 2;
        uint8_t REFSEL : 2;
        uint8_t REFN_BUF : 1;
        uint8_t REFP_BUF : 1;
        uint8_t FL_REF_EN : 1;
        uint8_t RESERVED : 1;
    } u;
};

// 06h IDACMAG
union IDACMAG {
    uint8_t Value;
    struct {
        uint8_t IMAG : 4;
        uint8_t RESERVED : 4;
    } u;
};

// 07h IDACMUX
union IDACMUX {
    uint8_t Value;
    struct {
        uint8_t I1MUX : 4;
        uint8_t I2MUX : 4;
    } u;
};

// 08h VBIAS
union VBIAS {
    uint8_t Value;
    struct {
        uint8_t VB_AIN0 : 1;
        uint8_t VB_AIN1 : 1;
        uint8_t VB_AIN2 : 1;
        uint8_t VB_AIN3 : 1;
        uint8_t VB_AIN4 : 1;
        uint8_t VB_AIN5 : 1;
        uint8_t VB_AINC : 1;
        uint8_t RESERVED : 1;
    } u;
};

// 09h SYS
union SYS {
    uint8_t Value;
    struct {
        uint8_t RESERVED : 2;
        uint8_t TIMEOUT : 1;
        uint8_t CAL_SAMP : 2;
        uint8_t SYS_MON : 3;
    } u;
};

// 0Bh OFCAL0
union OFCAL0 {
    uint8_t Value;
    struct {
        uint8_t OFC : 8;
    } u;
};

// 0Ch OFCAL1
union OFCAL1 {
    uint8_t Value;
    struct {
        uint8_t OFC : 8;
    } u;
};

// FSCAL0 0Eh
union FSCAL0 {
    uint8_t Value;
    struct {
        uint8_t FSC : 8;
    } u;
};

// FSCAL1 0Fh
union FSCAL1 {
    uint8_t Value;
    struct {
        uint8_t FSC : 8;
    } u;
};

// GPIODAT 10h
union GPIODAT {
    uint8_t Value;
    struct {
        uint8_t DAT : 4;
        uint8_t DIR : 4;
    } u;
};

// GPIOCON 11h
union GPIOCON {
    uint8_t Value;
    struct {
        uint8_t CON : 4;
        uint8_t RESERVED : 4;
    } u;
};

// Input Multiplexer(INPMUX) Register Field Descriptions
// Selects the ADC positive input channel.
namespace INPMUX_Field {
constexpr uint8_t AIN0 = 0;
constexpr uint8_t AIN1 = 1;
constexpr uint8_t AIN2 = 2;
constexpr uint8_t AIN3 = 3;
constexpr uint8_t AIN4 = 4;
constexpr uint8_t AIN5 = 5;
constexpr uint8_t AIN6 = 6;
constexpr uint8_t AIN7 = 7;
constexpr uint8_t AIN8 = 8;
constexpr uint8_t AIN9 = 9;
constexpr uint8_t AIN10 = 10;
constexpr uint8_t AIN11 = 11;
constexpr uint8_t AINCOM = 12;
} // namespace INPMUX_Field

// Programmable conversion delay selection
namespace PGA_DELAY_Field {
constexpr uint8_t DELAY_14 = 0;
constexpr uint8_t DELAY_25 = 1;
constexpr uint8_t DELAY_64 = 2;
constexpr uint8_t DELAY_256 = 3;
constexpr uint8_t DELAY_1024 = 4;
constexpr uint8_t DELAY_2048 = 5;
constexpr uint8_t DELAY_4096 = 6;
constexpr uint8_t DELAY_1 = 7;
} // namespace PGA_DELAY_Field

// Enables or bypasses the PGA.
namespace PGA_ENABLE_Field {
constexpr uint8_t POWERED_DOWN_AND_BYPASSED = 0;
constexpr uint8_t ENABLED = 1;
} // namespace PGA_ENABLE_Field

// Configures the PGA gain
namespace PGA_PAGE_SELECTION {
constexpr uint8_t GAIN_1 = 0;
constexpr uint8_t GAIN_2 = 1;
constexpr uint8_t GAIN_4 = 2;
constexpr uint8_t GAIN_8 = 3;
constexpr uint8_t GAIN_16 = 4;
constexpr uint8_t GAIN_32 = 5;
constexpr uint8_t GAIN_64 = 6;
constexpr uint8_t GAIN_128 = 7;
} // namespace PGA_PAGE_SELECTION

// Enables the global chop function.
namespace DATARATE_SELECTION_G_CHOP {
constexpr uint8_t DISABLE = 0;
constexpr uint8_t ENABLE = 1;
} // namespace DATARATE_SELECTION_G_CHOP

// Configures the clock source
namespace DATARATE_SELECTION_CLK {
constexpr uint8_t INTERNAL_4_096MHZ = 0;
constexpr uint8_t EXTERNAL_CLOCK = 1;
} // namespace DATARATE_SELECTION_CLK

// Configures the ADC for conversion mode
namespace DATARATE_SELECTION_MOD {
constexpr uint8_t CONTINUOUS_CONVERSION_MODE = 0;
constexpr uint8_t SINGLE_SHOT_CONVERSION_MODE = 1;
} // namespace DATARATE_SELECTION_MOD

// Configures the ADC filter type
namespace DATARATE_SELECTION_FILTER {
constexpr uint8_t SINC3 = 0;
constexpr uint8_t LOW_LATENCY_FILTER = 1;
} // namespace DATARATE_SELECTION_FILTER

// Data rate selection
namespace DATARATE_SELECTION_DR {
constexpr uint8_t SEL_2_5_SPS = 0;
constexpr uint8_t SEL_5_SPS = 1;
constexpr uint8_t SEL_10_SPS = 2;
constexpr uint8_t SEL_16_6_SPS = 3;
constexpr uint8_t SEL_20_SPS = 4;
constexpr uint8_t SEL_50_SPS = 5;
constexpr uint8_t SEL_60_SPS = 6;
constexpr uint8_t SEL_100_SPS = 7;
constexpr uint8_t SEL_200_SPS = 8;
constexpr uint8_t SEL_400_SPS = 9;
constexpr uint8_t SEL_800_SPS = 10;
constexpr uint8_t SEL_1000_SPS = 11;
constexpr uint8_t SEL_2000_SPS = 12;
constexpr uint8_t SEL_4000_SPS = 13;
constexpr uint8_t SEL_4000_SPS_2 = 14;
} // namespace DATARATE_SELECTION_DR

// System monitor configuration
namespace SYS_CONTROL_SYSMON {
constexpr uint8_t DISABLE = 0;
constexpr uint8_t NORMAL = 1;
constexpr uint8_t INTERNAL_TEMP = 2;
constexpr uint8_t AVDD_AVSS_4_MEASUREMENT_GAIN_SET_TO_1 = 3;
constexpr uint8_t DVDD_4_MEASUREMENT_GAIN_SET_TO_1 = 4;
constexpr uint8_t BURNOUT_CURRENT_SOURCES_ENABLED_0_2_MA = 5;
constexpr uint8_t BURNOUT_CURRENT_SOURCES_ENABLED_1_MA = 6;
constexpr uint8_t BURNOUT_CURRENT_SOURCES_ENABLED_10_MA = 7;
} // namespace SYS_CONTROL_SYSMON

// Calibration sample size selection
namespace SYS_CONTROL_SAMP {
constexpr uint8_t CAL_SAMP_1_SAMPLE = 0;
constexpr uint8_t CAL_SAMP_4_SAMPLE = 1;
constexpr uint8_t CAL_SAMP_8_SAMPLE = 2;
constexpr uint8_t CAL_SAMP_16_SAMPLE = 3;
} // namespace SYS_CONTROL_SAMP

// SPI timeout enable
namespace SYS_CONTROL_TIMEOUT {
constexpr uint8_t DISABLE = 0;
constexpr uint8_t ENABLE = 1;
} // namespace SYS_CONTROL_TIMEOUT

// CRC enable
namespace SYS_CONTROL_CRC {
constexpr uint8_t DISABLE = 0;
constexpr uint8_t ENABLE = 1;
} // namespace SYS_CONTROL_CRC

// STATUS byte control
namespace SYS_CONTROL_STATUS {
constexpr uint8_t DISABLE = 0;
constexpr uint8_t ENABLE = 1;
} // namespace SYS_CONTROL_STATUS

HAL_StatusTypeDef reg_write(uint8_t reg, uint8_t data);
HAL_StatusTypeDef reg_read(uint8_t reg, uint8_t* data);
HAL_StatusTypeDef data_read(uint16_t* data);
HAL_StatusTypeDef data_read_IT(uint16_t* data);

HAL_StatusTypeDef put_to_sleep();
HAL_StatusTypeDef start_by_command();
HAL_StatusTypeDef stop_by_command();
HAL_StatusTypeDef wake_up_the_device();
HAL_StatusTypeDef reset_device_to_default_settings();
HAL_StatusTypeDef self_offset_calibration();
HAL_StatusTypeDef system_gain_calibration();

int init();
void select_channel(uint8_t muxp, uint8_t muxn);
void start_conversions();

// int read_channel_data(uint32_t* d_status, uint32_t* d_crc, int* i_data);
// int read_link1_back_power_from_antenna(float* power);
// int read_link1_transmitting_power(float* power);
// int read_anlg1_in1(float* power);
// int read_anlg1_in2(float* power);
// int read_link2_back_power_from_antenna(float* power);
// int read_link2_transmitting_power(float* power);
// int read_12v_input_voltage(float* power);
// int read_5v_output_of_5v_dc_dc(float* power);
// int read_3_7v_dc_dc(float* power);
// int read_gbe_switch1_1v_core_voltage(float* power);
// int read_adc_internal_temperature(float* temperature);
// int read_adc_internal_power_supply(float* power);

extern ID id;
extern STATUS status;
extern INPMUX inpmux;
extern PGA pga;
extern DATARATE datarate;
extern REF ref;
extern IDACMAG idacmag;
extern IDACMUX idacmux;
extern VBIAS vbias;
extern SYS sys;
extern OFCAL0 ofcal0;
extern OFCAL1 ofcal1;
extern FSCAL0 fscal0;
extern FSCAL1 fscal1;
extern GPIODAT gpiodat;
extern GPIOCON gpiocon;

} // namespace ADS114S08

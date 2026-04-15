#pragma once

#include "stm32g0xx_hal.h"
#include <cstdint>

namespace ADS114S08 {

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
        uint8_t FL_REF_L1 : 1;
        uint8_t FL_N_RAILN : 1;
        uint8_t FL_N_RAILP : 1;
        uint8_t FL_P_RAILN : 1;
        uint8_t FL_P_RAILP : 1;
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
        uint8_t DELAY : 3;
    } u;
};

// 04h DATARATE
union DATARATE {
    uint8_t Value;
    struct {
        uint8_t DR : 4;
        uint8_t FILTER : 1;
        uint8_t MODE : 1;
        uint8_t CLK : 1;
        uint8_t G_CHOP : 1;
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
        uint8_t FL_REF_EN : 2;
    } u;
};

// 06h IDACMAG
union IDACMAG {
    uint8_t Value;
    struct {
        uint8_t IMAG : 4;
        uint8_t ZERO1 : 1;
        uint8_t ZERO2 : 1;
        uint8_t PSW : 1;
        uint8_t FL_RAIL_EN : 1;
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
        uint8_t VB_LEVEL : 1;
        uint8_t VB_AINC : 1;
        uint8_t VB_AIN5 : 1;
        uint8_t VB_AIN4 : 1;
        uint8_t VB_AIN3 : 1;
        uint8_t VB_AIN2 : 1;
        uint8_t VB_AIN1 : 1;
        uint8_t VB_AIN0 : 1;
    } u;
};

// 09h SYS
union SYS {
    uint8_t Value;
    struct {
        uint8_t SENDSTAT : 1;
        uint8_t CRC_ : 1;
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
        uint8_t ReservedZero : 4;
    } u;
};

// Input Multiplexer(INPMUX) Register Field Descriptions
// Selects the ADC positive input channel.
enum class INPMUX_Field : uint8_t {
    AIN0 = 0,
    AIN1 = 1,
    AIN2 = 2,
    AIN3 = 3,
    AIN4 = 4,
    AIN5 = 5,
    AIN6 = 6,
    AIN7 = 7,
    AIN8 = 8,
    AIN9 = 9,
    AIN10 = 10,
    AIN11 = 11,
    AINCOM = 12
};

// Programmable conversion delay selection
// Sets the programmable conversion delay time for the first conversion after a
// WREG when a configuration change resets of the digital filter and triggers a
// new conversion(1)
enum class PGA_DELAY_Field : uint8_t {
    DELAY_14 = 0,
    DELAY_25 = 1,
    DELAY_64 = 2,
    DELAY_256 = 3,
    DELAY_1024 = 4,
    DELAY_2048 = 5,
    DELAY_4096 = 6,
    DELAY_1 = 7
};

// Enables or bypasses the PGA.
enum class PGA_ENABLE_Field : uint8_t {
    POWERED_DOWN_AND_BYPASSED = 0,
    ENABLED = 1
};

// Configures the PGA gain
enum class PGA_PAGE_SELECTION : uint8_t {
    GAIN_1 = 0,
    GAIN_2 = 1,
    GAIN_4 = 2,
    GAIN_8 = 3,
    GAIN_16 = 4,
    GAIN_32 = 5,
    GAIN_64 = 6,
    GAIN_128 = 7
};

// Enables the global chop function.When enabled, the device automatically
// swaps the inputs and takes the average of two consecutive readings to
// cancel the offset voltage.
enum class DATARATE_SELECTION_G_CHOP : uint8_t { DISABLE = 0, ENABLE = 1 };

// Configures the clock source to use either the internal oscillator or an
// external clock
enum class DATARATE_SELECTION_CLK : uint8_t {
    INTERNAL_4_096MHZ = 0,
    EXTERNAL_CLOCK = 1
};

// Configures the ADC for either continuous conversion or single - shot
// conversion mode
enum class DATARATE_SELECTION_MOD : uint8_t {
    CONTINUOUS_CONVERSION_MODE = 0,
    SINGLE_SHOT_CONVERSION_MODE = 1
};

// Configures the ADC to use either the sinc3 or the low-latency filter
enum class DATARATE_SELECTION_FILTER : uint8_t {
    SINC3 = 0,
    LOW_LATENCY_FILTER = 1
};

// Data rate selection
// Configures the output data rate
enum class DATARATE_SELECTION_DR : uint8_t {
    SEL_2_5_SPS = 0,
    SEL_5_SPS = 1,
    SEL_10_SPS = 2,
    SEL_16_6_SPS = 3,
    SEL_20_SPS = 4,
    SEL_50_SPS = 5,
    SEL_60_SPS = 6,
    SEL_100_SPS = 7,
    SEL_200_SPS = 8,
    SEL_400_SPS = 9,
    SEL_800_SPS = 10,
    SEL_1000_SPS = 11,
    SEL_2000_SPS = 12,
    SEL_4000_SPS = 13,
    SEL_4000_SPS_2 = 14
};

// System Control(SYS) Register Field Descriptions
// Enables a set of system monitor measurements using the ADC.
enum class SYS_CONTROL_SYSMON : uint8_t {
    DISABLE = 0,
    NORMAL = 1,
    INTERNAL_TEMP = 2,
    AVDD_AVSS_4_MEASUREMENT_GAIN_SET_TO_1 = 3,
    DVDD_4_MEASUREMENT_GAIN_SET_TO_1 = 4,
    BURNOUT_CURRENT_SOURCES_ENABLED_0_2_MA = 5,
    BURNOUT_CURRENT_SOURCES_ENABLED_1_MA = 6,
    BURNOUT_CURRENT_SOURCES_ENABLED_10_MA = 7
};

// Calibration sample size selection
// Configures the number of samples averaged for self and system offset and
// system gain calibration.
enum class SYS_CONTROL_SAMP : uint8_t {
    CAL_SAMP_1_SAMPLE = 0,
    CAL_SAMP_4_SAMPLE = 1,
    CAL_SAMP_8_SAMPLE = 2,
    CAL_SAMP_16_SAMPLE = 3
};

// SPI timeout enable
// Enables the SPI timeout function.
enum class SYS_CONTROL_TIMEOUT : uint8_t {
    DISABLE = 0, // default
    ENABLE = 1
};

// CRC enable
// Enables the CRC byte appended to the conversion result.When enabled,
// CRC is calculate
enum class SYS_CONTROL_CRC : uint8_t {
    DISABLE = 0, // default
    ENABLE = 1
};

// Enables the STATUS byte prepended to the conversion result.
enum class SYS_CONTROL_STATUS : uint8_t {
    DISABLE = 0, // default
    ENABLE = 1
};

int init();
HAL_StatusTypeDef put_to_sleep();
HAL_StatusTypeDef start_by_command();
HAL_StatusTypeDef stop_by_command();
HAL_StatusTypeDef wake_up_the_device();
HAL_StatusTypeDef reset_device_to_default_settings();
HAL_StatusTypeDef self_offset_calibration();
HAL_StatusTypeDef system_gain_calibration();
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

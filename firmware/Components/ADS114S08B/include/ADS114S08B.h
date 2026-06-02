#pragma once

#include <cstdint>

#include "gpio.h"
#include "spi.h"

namespace ADS114S08B {

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
constexpr uint8_t ID_DEV_ID_MASK = 0x07;
constexpr uint8_t ID_DEV_ID_SHIFT = 0;
constexpr uint8_t ID_RESERVED_MASK = 0xF8;
constexpr uint8_t ID_RESERVED_SHIFT = 3;

// 01h STATUS 80h
constexpr uint8_t STATUS_FL_REF_L0_MASK = 0x01;
constexpr uint8_t STATUS_FL_REF_L0_SHIFT = 0;
constexpr uint8_t STATUS_RESERVED_MASK = 0x3E;
constexpr uint8_t STATUS_RESERVED_SHIFT = 1;
constexpr uint8_t STATUS_RDY_MASK = 0x40;
constexpr uint8_t STATUS_RDY_SHIFT = 6;
constexpr uint8_t STATUS_FL_POR_MASK = 0x80;
constexpr uint8_t STATUS_FL_POR_SHIFT = 7;

// 02h INPMUX
constexpr uint8_t INPMUX_MUXN_MASK = 0x0F;
constexpr uint8_t INPMUX_MUXN_SHIFT = 0;
constexpr uint8_t INPMUX_MUXP_MASK = 0xF0;
constexpr uint8_t INPMUX_MUXP_SHIFT = 4;

// 03h PGA
constexpr uint8_t PGA_GAIN_MASK = 0x07;
constexpr uint8_t PGA_GAIN_SHIFT = 0;
constexpr uint8_t PGA_EN_MASK = 0x18;
constexpr uint8_t PGA_EN_SHIFT = 3;
constexpr uint8_t PGA_RESERVED_MASK = 0xE0;
constexpr uint8_t PGA_RESERVED_SHIFT = 5;

// 04h DATARATE
constexpr uint8_t DATARATE_DR_MASK = 0x0F;
constexpr uint8_t DATARATE_DR_SHIFT = 0;
constexpr uint8_t DATARATE_RESERVED0_MASK = 0x10;
constexpr uint8_t DATARATE_RESERVED0_SHIFT = 4;
constexpr uint8_t DATARATE_MODE_MASK = 0x20;
constexpr uint8_t DATARATE_MODE_SHIFT = 5;
constexpr uint8_t DATARATE_CLK_MASK = 0x40;
constexpr uint8_t DATARATE_CLK_SHIFT = 6;
constexpr uint8_t DATARATE_RESERVED1_MASK = 0x80;
constexpr uint8_t DATARATE_RESERVED1_SHIFT = 7;

// 05h REF
constexpr uint8_t REF_REFCON_MASK = 0x03;
constexpr uint8_t REF_REFCON_SHIFT = 0;
constexpr uint8_t REF_REFSEL_MASK = 0x0C;
constexpr uint8_t REF_REFSEL_SHIFT = 2;
constexpr uint8_t REF_REFN_BUF_MASK = 0x10;
constexpr uint8_t REF_REFN_BUF_SHIFT = 4;
constexpr uint8_t REF_REFP_BUF_MASK = 0x20;
constexpr uint8_t REF_REFP_BUF_SHIFT = 5;
constexpr uint8_t REF_FL_REF_EN_MASK = 0x40;
constexpr uint8_t REF_FL_REF_EN_SHIFT = 6;
constexpr uint8_t REF_RESERVED_MASK = 0x80;
constexpr uint8_t REF_RESERVED_SHIFT = 7;

// 06h IDACMAG
constexpr uint8_t IDACMAG_IMAG_MASK = 0x0F;
constexpr uint8_t IDACMAG_IMAG_SHIFT = 0;
constexpr uint8_t IDACMAG_RESERVED_MASK = 0xF0;
constexpr uint8_t IDACMAG_RESERVED_SHIFT = 4;

// 07h IDACMUX
constexpr uint8_t IDACMUX_I1MUX_MASK = 0x0F;
constexpr uint8_t IDACMUX_I1MUX_SHIFT = 0;
constexpr uint8_t IDACMUX_I2MUX_MASK = 0xF0;
constexpr uint8_t IDACMUX_I2MUX_SHIFT = 4;

// 08h VBIAS
constexpr uint8_t VBIAS_VB_AIN0_MASK = 0x01;
constexpr uint8_t VBIAS_VB_AIN0_SHIFT = 0;
constexpr uint8_t VBIAS_VB_AIN1_MASK = 0x02;
constexpr uint8_t VBIAS_VB_AIN1_SHIFT = 1;
constexpr uint8_t VBIAS_VB_AIN2_MASK = 0x04;
constexpr uint8_t VBIAS_VB_AIN2_SHIFT = 2;
constexpr uint8_t VBIAS_VB_AIN3_MASK = 0x08;
constexpr uint8_t VBIAS_VB_AIN3_SHIFT = 3;
constexpr uint8_t VBIAS_VB_AIN4_MASK = 0x10;
constexpr uint8_t VBIAS_VB_AIN4_SHIFT = 4;
constexpr uint8_t VBIAS_VB_AIN5_MASK = 0x20;
constexpr uint8_t VBIAS_VB_AIN5_SHIFT = 5;
constexpr uint8_t VBIAS_VB_AINC_MASK = 0x40;
constexpr uint8_t VBIAS_VB_AINC_SHIFT = 6;
constexpr uint8_t VBIAS_RESERVED_MASK = 0x80;
constexpr uint8_t VBIAS_RESERVED_SHIFT = 7;

// 09h SYS
constexpr uint8_t SYS_RESERVED_MASK = 0x03;
constexpr uint8_t SYS_RESERVED_SHIFT = 0;
constexpr uint8_t SYS_TIMEOUT_MASK = 0x04;
constexpr uint8_t SYS_TIMEOUT_SHIFT = 2;
constexpr uint8_t SYS_CAL_SAMP_MASK = 0x18;
constexpr uint8_t SYS_CAL_SAMP_SHIFT = 3;
constexpr uint8_t SYS_SYS_MON_MASK = 0xE0;
constexpr uint8_t SYS_SYS_MON_SHIFT = 5;

// 0Bh OFCAL0
constexpr uint8_t OFCAL0_OFC_MASK = 0xFF;
constexpr uint8_t OFCAL0_OFC_SHIFT = 0;

// 0Ch OFCAL1
constexpr uint8_t OFCAL1_OFC_MASK = 0xFF;
constexpr uint8_t OFCAL1_OFC_SHIFT = 0;

// FSCAL0 0Eh
constexpr uint8_t FSCAL0_FSC_MASK = 0xFF;
constexpr uint8_t FSCAL0_FSC_SHIFT = 0;

// FSCAL1 0Fh
constexpr uint8_t FSCAL1_FSC_MASK = 0xFF;
constexpr uint8_t FSCAL1_FSC_SHIFT = 0;

// GPIODAT 10h
constexpr uint8_t GPIODAT_DAT_MASK = 0x0F;
constexpr uint8_t GPIODAT_DAT_SHIFT = 0;
constexpr uint8_t GPIODAT_DIR_MASK = 0xF0;
constexpr uint8_t GPIODAT_DIR_SHIFT = 4;

// GPIOCON 11h
constexpr uint8_t GPIOCON_CON_MASK = 0x0F;
constexpr uint8_t GPIOCON_CON_SHIFT = 0;
constexpr uint8_t GPIOCON_RESERVED_MASK = 0xF0;
constexpr uint8_t GPIOCON_RESERVED_SHIFT = 4;

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
    AINCOM = 12,
}; // namespace INPMUX_Field

// Enables or bypasses the PGA.
enum class PGA_EN_Field : uint8_t {
    POWERED_DOWN_AND_BYPASSED = 0,
    ENABLED = 1,
};

// Configures the PGA gain
enum class PGA_GAIN_Field : uint8_t {
    GAIN_1 = 0,
    GAIN_2 = 1,
    GAIN_4 = 2,
    GAIN_8 = 3,
    GAIN_16 = 4,
    GAIN_32 = 5,
    GAIN_64 = 6,
    GAIN_128 = 7,
};

// Enables the global chop function.
enum class DATARATE_SELECTION : uint8_t {
    DISABLE = 0,
    ENABLE = 1,
};

// Configures the clock source
enum class DR_CLK_Field : uint8_t {
    INTERNAL_4_096MHZ = 0,
    EXTERNAL_CLOCK = 1,
};

// Configures the ADC for conversion mode
enum class DR_MODE_Field : uint8_t {
    CONTINUOUS_CONVERSION_MODE = 0,
    SINGLE_SHOT_CONVERSION_MODE = 1,
};

// Data rate selection
enum class DR_SEL_Field : uint8_t {
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
    SEL_4000_SPS_2 = 14,
};

// System monitor configuration
enum class SYS_MON : uint8_t {
    DISABLE = 0,
    NORMAL = 1,
    INTERNAL_TEMP = 2,
    AVDD_AVSS_4_MEASUREMENT_GAIN_SET_TO_1 = 3,
    DVDD_4_MEASUREMENT_GAIN_SET_TO_1 = 4,
    BURNOUT_CURRENT_SOURCES_ENABLED_0_2_MA = 5,
    BURNOUT_CURRENT_SOURCES_ENABLED_1_MA = 6,
    BURNOUT_CURRENT_SOURCES_ENABLED_10_MA = 7,
};

// Calibration sample size selection
enum class SYS_CAL_SAMP : uint8_t {
    CAL_SAMP_1_SAMPLE = 0,
    CAL_SAMP_4_SAMPLE = 1,
    CAL_SAMP_8_SAMPLE = 2,
    CAL_SAMP_16_SAMPLE = 3,
};

// SPI timeout enable
enum class SYS_TIMEOUT : uint8_t {
    DISABLE = 0,
    ENABLE = 1,
};

enum class REF_SEL : uint8_t {
    EXTERNAL_0 = 0,
    EXTERNAL_1 = 1,
    INTERNAL = 2,
};

class Driver {
  public:
    enum class State {
        POWER_DOWN,
        STANDBY,
        SINGLE_CONVERSION_MODE,
        CONTINUOUS_CONVERSION_MODE,
    };

    Driver(SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* start_port,
           uint16_t start_pin, GPIO_TypeDef* drdy_port, uint16_t drdy_pin,
           GPIO_TypeDef* rst_port, uint16_t rst_pin);

    State get_state();
    DR_MODE_Field get_mode();
    int write_check(uint8_t reg, uint8_t data_write, uint8_t& data_read);
    int init();
    int select_differential(INPMUX_Field muxp, INPMUX_Field muxn);
    int select_single_ended(INPMUX_Field muxp);
    int config_pga(PGA_EN_Field pga_en, PGA_GAIN_Field gain);
    int config_datarate(DR_SEL_Field dr, DR_MODE_Field mode, DR_CLK_Field clk);
    int start_continous_conversions();
    int start_single_conversion();
    uint16_t decode_data_IT();
    HAL_StatusTypeDef data_read_IT();

  protected:
    State state{State::POWER_DOWN};

    static constexpr uint32_t SPI_TIMEOUT{100};

    SPI_HandleTypeDef* const spi_handle;

    GPIO_TypeDef* const start_port;
    const uint16_t start_pin;

    GPIO_TypeDef* const drdy_port;
    const uint16_t drdy_pin;

    GPIO_TypeDef* const rst_port;
    const uint16_t rst_pin;

    uint8_t tx_buffer[3]{};
    uint8_t rx_buffer[3]{};

    uint8_t id{0};
    uint8_t status{0};
    uint8_t inpmux{0};
    uint8_t pga{0};
    uint8_t datarate{0};
    uint8_t ref{0};
    uint8_t idacmag{0};
    uint8_t idacmux{0};
    uint8_t vbias{0};
    uint8_t sys{0};
    uint8_t ofcal0{0};
    uint8_t ofcal1{0};
    uint8_t fscal0{0};
    uint8_t fscal1{0};
    uint8_t gpiodat{0};
    uint8_t gpiocon{0};

    HAL_StatusTypeDef reg_write(uint8_t reg, uint8_t data);
    HAL_StatusTypeDef reg_read(uint8_t reg, uint8_t& data);
    HAL_StatusTypeDef data_read(uint16_t& data);
    HAL_StatusTypeDef put_to_sleep();
    HAL_StatusTypeDef start_by_command();
    HAL_StatusTypeDef stop_by_command();
    HAL_StatusTypeDef wake_up_the_device();
    HAL_StatusTypeDef reset_device_to_default_settings();
    HAL_StatusTypeDef self_offset_calibration();
    HAL_StatusTypeDef system_gain_calibration();
};

} // namespace ADS114S08B

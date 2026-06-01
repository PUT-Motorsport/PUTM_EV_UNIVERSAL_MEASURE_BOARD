#include "ADS114S08B.h"
#include "stm32g0xx_hal_def.h"

namespace ADS114S08B {

HAL_StatusTypeDef Driver::reg_write(uint8_t reg, uint8_t data) {
    uint8_t tx[3]{static_cast<uint8_t>(WREG_CMD | (reg & 0x1f)), 0x00, data};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::reg_read(uint8_t reg, uint8_t& data) {
    uint8_t txrx[3]{static_cast<uint8_t>(RREG_CMD | (reg & 0x1f)), 0x00, 0x00};
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
        spi_handle, txrx, txrx, sizeof(txrx), SPI_TIMEOUT);
    if(status == HAL_OK) {
        data = txrx[2];
    }
    return status;
}

HAL_StatusTypeDef Driver::data_read(uint16_t& data) {
    uint8_t txrx[3]{RDATA_CMD, 0x00, 0x00};
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
        spi_handle, txrx, txrx, sizeof(txrx), SPI_TIMEOUT);
    if(status == HAL_OK) {
        data = (txrx[1] << 8) | txrx[2];
    }
    return status;
}

HAL_StatusTypeDef Driver::data_read_IT() {
    tx_buffer[0] = RDATA_CMD;
    tx_buffer[1] = 0x00;
    tx_buffer[2] = 0x00;
    rx_buffer[0] = 0x00;
    rx_buffer[1] = 0x00;
    rx_buffer[2] = 0x00;
    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive_IT(spi_handle, tx_buffer, rx_buffer, 3);
    return status;
}

HAL_StatusTypeDef Driver::start_by_command() {
    uint8_t tx[1]{START_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::stop_by_command() {
    uint8_t tx[1]{STOP_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::put_to_sleep() {
    uint8_t tx[1]{POWERDOWN_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::system_gain_calibration() {
    uint8_t tx[1]{SYGCAL_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::wake_up_the_device() {
    uint8_t tx[1]{WAKEUP_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::reset_device_to_default_settings() {
    uint8_t tx[1]{RESET_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::self_offset_calibration() {
    uint8_t tx[1]{SFOCAL_CMD};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

Driver::Driver(SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* start_port,
               uint16_t start_pin, GPIO_TypeDef* drdy_port, uint16_t drdy_pin,
               GPIO_TypeDef* rst_port, uint16_t rst_pin)
    : spi_handle{spi_handle}, start_port{start_port}, start_pin{start_pin},
      drdy_port{drdy_port}, drdy_pin{drdy_pin}, rst_port{rst_port},
      rst_pin{rst_pin} {}

int Driver::write_check(uint8_t reg, uint8_t data_write, uint8_t& data_read) {
    uint8_t data_buffer{};
    if(reg_write(reg, data_write) != HAL_OK)
        return 1;
    if(reg_read(reg, data_buffer) != HAL_OK)
        return 1;
    if(data_write != data_buffer)
        return 1;
    data_read = data_buffer;
    return 0;
}

Driver::State Driver::get_state() { return state; }

DR_MODE_Field Driver::get_mode() {
    return static_cast<DR_MODE_Field>((datarate & DATARATE_MODE_MASK) >>
                                      DATARATE_MODE_SHIFT);
}

int Driver::init() {

    if(state == State::POWER_DOWN) {
        if(spi_handle == nullptr || start_port == nullptr ||
           drdy_port == nullptr || rst_port == nullptr || start_pin == 0 ||
           drdy_pin == 0 || rst_pin == 0) {
            return 1;
        }

        // Bring ADC out of reset
        HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_RESET);
        HAL_Delay(5); // Wait for ADC to power up and initialize
        HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_SET);
        HAL_Delay(10); // td(RSSC) after reset

        // Check if ADC connected
        if(reg_read(ID_ADDR, id) != HAL_OK) {
            return 1;
        }
        if(((id & ID_DEV_ID_MASK) >> ID_DEV_ID_SHIFT) != 0x4) {
            return 1;
        }

        // Reset ADC to default settings
        if(reset_device_to_default_settings() == HAL_ERROR)
            return 1;

        HAL_Delay(4);

        // Check if ADC reset ok
        if(reg_read(STATUS_ADDR, status) != HAL_OK) {
            return 1;
        }
        if(status != REG_STATUS_DEFAULT) {
            return 1;
        }

        // Clear power-on-reset flag
        reg_write(STATUS_ADDR, 0x00);

        // Read all registers for verification
        reg_read(STATUS_ADDR, status);
        reg_read(INPMUX_ADDR, inpmux);
        reg_read(PGA_ADDR, pga);
        reg_read(DATARATE_ADDR, datarate);
        reg_read(REF_ADDR, ref);
        reg_read(IDACMAG_ADDR, idacmag);
        reg_read(IDACMUX_ADDR, idacmux);
        reg_read(VBIAS_ADDR, vbias);
        reg_read(SYS_ADDR, sys);
        reg_read(OFCAL0_ADDR, ofcal0);
        reg_read(OFCAL1_ADDR, ofcal1);
        reg_read(FSCAL0_ADDR, fscal0);
        reg_read(FSCAL1_ADDR, fscal1);
        reg_read(GPIODAT_ADDR, gpiodat);
        reg_read(GPIOCON_ADDR, gpiocon);

        state = State::STANDBY;
        return 0;
    }
    return 1;
}

int Driver::select_differential(INPMUX_Field muxp, INPMUX_Field muxn) {
    if(state != State::POWER_DOWN) {
        uint8_t inpmux_write = inpmux;
        inpmux_write = (inpmux_write & ~INPMUX_MUXP_MASK) |
                       ((static_cast<uint8_t>(muxp) << INPMUX_MUXP_SHIFT) &
                        INPMUX_MUXP_MASK);
        inpmux_write = (inpmux_write & ~INPMUX_MUXN_MASK) |
                       ((static_cast<uint8_t>(muxn) << INPMUX_MUXN_SHIFT) &
                        INPMUX_MUXN_MASK);
        return write_check(INPMUX_ADDR, inpmux_write, inpmux);
    }
    return 1;
}

int Driver::select_single_ended(INPMUX_Field muxp) {
    if(state != State::POWER_DOWN) {
        return select_differential(muxp, INPMUX_Field::AINCOM);
    }
    return 1;
}

int Driver::start_continous_conversions() {
    if(state == State::STANDBY &&
       get_mode() == DR_MODE_Field::CONTINUOUS_CONVERSION_MODE) {
        state = State::CONTINUOUS_CONVERSION_MODE;
        HAL_GPIO_WritePin(start_port, start_pin, GPIO_PIN_SET);
        HAL_Delay(1);
        return 0;
    }
    return 1;
}

int Driver::start_single_conversion() {
    if(state == State::STANDBY &&
       get_mode() == DR_MODE_Field::SINGLE_SHOT_CONVERSION_MODE) {
        state = State::SINGLE_CONVERSION_MODE;
        HAL_GPIO_WritePin(start_port, start_pin, GPIO_PIN_RESET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(start_port, start_pin, GPIO_PIN_SET);
        return 0;
    }
    return 1;
}

int Driver::config_pga(PGA_EN_Field pga_en, PGA_GAIN_Field gain) {
    if(state != State::POWER_DOWN) {
        uint8_t pga_write = pga;
        pga_write =
            (pga_write & ~PGA_GAIN_MASK) |
            ((static_cast<uint8_t>(gain) << PGA_GAIN_SHIFT) & PGA_GAIN_MASK);
        pga_write =
            (pga_write & ~PGA_EN_MASK) |
            ((static_cast<uint8_t>(pga_en) << PGA_EN_SHIFT) & PGA_EN_MASK);
        return write_check(PGA_ADDR, pga_write, pga);
    }
    return 1;
}

int Driver::config_datarate(DR_SEL_Field dr, DR_MODE_Field mode,
                            DR_CLK_Field clk) {
    if(state != State::POWER_DOWN) {
        uint8_t datarate_write = datarate;
        datarate_write = (datarate_write & ~DATARATE_DR_MASK) |
                         ((static_cast<uint8_t>(dr) << DATARATE_DR_SHIFT) &
                          DATARATE_DR_MASK);
        datarate_write = (datarate_write & ~DATARATE_MODE_MASK) |
                         ((static_cast<uint8_t>(mode) << DATARATE_MODE_SHIFT) &
                          DATARATE_MODE_MASK);
        datarate_write = (datarate_write & ~DATARATE_CLK_MASK) |
                         ((static_cast<uint8_t>(clk) << DATARATE_CLK_SHIFT) &
                          DATARATE_CLK_MASK);
        return write_check(DATARATE_ADDR, datarate_write, datarate);
    }
    return 1;
}

uint16_t Driver::decode_data_IT() {
    uint16_t data = (rx_buffer[1] << 8) | rx_buffer[2];
    if(state == State::SINGLE_CONVERSION_MODE) {
        state = State::STANDBY;
    }
    return data;
}

} // namespace ADS114S08B

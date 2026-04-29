#include "ADS114S08.h"
#include "stm32g0xx_hal_def.h"

namespace ADS114S08 {

HAL_StatusTypeDef Driver::reg_write(uint8_t reg, uint8_t data) {
    uint8_t tx[3]{static_cast<uint8_t>(WREG_CMD | (reg & 0x1f)), 0x00, data};
    return HAL_SPI_Transmit(spi_handle, tx, sizeof(tx), SPI_TIMEOUT);
}

HAL_StatusTypeDef Driver::reg_read(uint8_t reg, uint8_t* data) {
    if(data == NULL)
        return HAL_ERROR;
    uint8_t txrx[3]{static_cast<uint8_t>(RREG_CMD | (reg & 0x1f)), 0x00, 0x00};
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
        spi_handle, txrx, txrx, sizeof(txrx), SPI_TIMEOUT);
    if(status == HAL_OK) {
        *data = txrx[2];
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
    rx_buffer[0] = RDATA_CMD;
    rx_buffer[1] = 0x00;
    rx_buffer[2] = 0x00;
    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive_IT(spi_handle, rx_buffer, rx_buffer, 3);
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

} // namespace ADS114S08

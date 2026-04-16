#include "ADS114S08.h"

namespace ADS114S08 {

HAL_StatusTypeDef reg_write(uint8_t reg, uint8_t data) {
    uint8_t tx[3]{static_cast<uint8_t>(WREG_CMD | (reg & 0x1f)), 0x00, data};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef reg_read(uint8_t reg, uint8_t* data) {
    uint8_t txrx[3]{static_cast<uint8_t>(RREG_CMD | (reg & 0x1f)), 0x00, 0x00};
    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(SPI, txrx, txrx, sizeof(txrx), HAL_MAX_DELAY);
    if(status == HAL_OK) {
        *data = txrx[2];
    }
    return status;
}

HAL_StatusTypeDef data_read(uint16_t* data) {
    uint8_t txrx[3]{RDATA_CMD, 0x00, 0x00};
    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(SPI, txrx, txrx, sizeof(txrx), HAL_MAX_DELAY);
    if(status == HAL_OK) {
        *data = (txrx[0] << 8) | txrx[1];
    }
    return status;
}

HAL_StatusTypeDef data_read_IT(uint16_t* data) {
    uint8_t txrx[3]{RDATA_CMD, 0x00, 0x00};
    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive_IT(SPI, txrx, txrx, sizeof(txrx));
    if(status == HAL_OK) {
        *data = (txrx[0] << 8) | txrx[1];
    }
    return status;
}

HAL_StatusTypeDef start_by_command() {
    uint8_t tx[1]{START_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef stop_by_command() {
    uint8_t tx[1]{STOP_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef put_to_sleep() {
    uint8_t tx[1]{POWERDOWN_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef system_gain_calibration() {
    uint8_t tx[1]{SYGCAL_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef wake_up_the_device() {
    uint8_t tx[1]{WAKEUP_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef reset_device_to_default_settings() {
    uint8_t tx[1]{RESET_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef self_offset_calibration() {
    uint8_t tx[1]{SFOCAL_CMD};
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

} // namespace ADS114S08

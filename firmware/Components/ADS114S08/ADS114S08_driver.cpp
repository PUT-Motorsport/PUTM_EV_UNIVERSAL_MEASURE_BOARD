#include "ADS114S08.h"
#include "config.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"

namespace ADS114S08 {

ID id;
STATUS status;
INPMUX inpmux;
PGA pga;
DATARATE datarate;
REF ref;
IDACMAG idacmag;
IDACMUX idacmux;
VBIAS vbias;
SYS sys;
OFCAL0 ofcal0;
OFCAL1 ofcal1;
FSCAL0 fscal0;
FSCAL1 fscal1;
GPIODAT gpiodat;
GPIOCON gpiocon;

HAL_StatusTypeDef reg_write(uint8_t reg, uint8_t data) {
    uint8_t tx[4];
    tx[0] = WREG_CMD | (reg & 0x1f);
    tx[1] = 0x00;
    tx[2] = data;

    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef reg_read(uint8_t reg, uint8_t* data) {
    uint8_t txrx[4];
    txrx[0] = RREG_CMD | (reg & 0x1f);
    txrx[1] = 0x00;
    txrx[2] = 0x00;

    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(SPI, txrx, txrx, sizeof(txrx), HAL_MAX_DELAY);
    if(status == HAL_OK) {
        *data = txrx[2];
    }
    return status;
}

HAL_StatusTypeDef data_read(uint16_t* data) {
    uint8_t txrx[3];
    txrx[0] = RDATA_CMD;
    txrx[1] = 0x00;
    txrx[2] = 0x00;

    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(SPI, txrx, txrx, sizeof(txrx), HAL_MAX_DELAY);
    if(status == HAL_OK) {
        *data = (txrx[0] << 8) | txrx[1];
    }
    return status;
}

HAL_StatusTypeDef data_read_IT(uint16_t* data) {
    uint8_t txrx[3];
    txrx[0] = RDATA_CMD;
    txrx[1] = 0x00;
    txrx[2] = 0x00;

    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive_IT(SPI, txrx, txrx, sizeof(txrx));
    if(status == HAL_OK) {
        *data = (txrx[0] << 8) | txrx[1];
    }
    return status;
}

HAL_StatusTypeDef start_by_command() {
    uint8_t tx[1];
    tx[0] = START_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef stop_by_command() {
    uint8_t tx[1];
    tx[0] = STOP_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef put_to_sleep() {
    uint8_t tx[1];
    tx[0] = POWERDOWN_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef system_gain_calibration() {
    uint8_t tx[1];
    tx[0] = SYGCAL_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef wake_up_the_device() {
    uint8_t tx[1];
    tx[0] = WAKEUP_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef reset_device_to_default_settings() {
    uint8_t tx[1];
    tx[0] = RESET_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

HAL_StatusTypeDef self_offset_calibration() {
    uint8_t tx[1];
    tx[0] = SFOCAL_CMD;
    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

} // namespace ADS114S08

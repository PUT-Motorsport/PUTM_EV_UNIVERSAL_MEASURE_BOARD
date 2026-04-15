#include "ADS114S08.h"
#include "gpio.h"
#include "spi.h"
#include "stm32g0xx_hal_spi.h"
#include "tim.h"

namespace ADS114S08 {

const auto SPI = reinterpret_cast<SPI_HandleTypeDef*>(SPI1);

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

static int data_read(uint32_t* d_status, uint32_t* d_crc, int* i_data);
static HAL_StatusTypeDef reg_write(uint8_t reg, uint8_t data);
static HAL_StatusTypeDef reg_read(uint8_t reg, uint8_t* data);

int init() {
    reg_read(ID_ADDR, &id.Value);
    if(id.u.DEV_ID != 0x4) {
        return 1;
    }

    reg_read(STATUS_ADDR, &status.Value);
    if(status.Value != REG_STATUS_DEFAULT) {
        return 1;
    }

    if(reset_device_to_default_settings() == HAL_ERROR)
        return 1;

    // reg_read(INPMUX_ADDR, &inpmux.Value);
    // reg_read(PGA_ADDR, &pga.Value);
    // reg_read(DATARATE_ADDR, &datarate.Value);
    // reg_read(REF_ADDR, &ref.Value);
    // reg_read(IDACMAG_ADDR, &idacmag.Value);
    // reg_read(IDACMUX_ADDR, &idacmux.Value);
    // reg_read(VBIAS_ADDR, &vbias.Value);
    // reg_read(SYS_ADDR, &sys.Value);
    // reg_read(OFCAL0_ADDR, &ofcal0.Value);
    // reg_read(OFCAL1_ADDR, &ofcal1.Value);
    // reg_read(FSCAL0_ADDR, &fscal0.Value);
    // reg_read(FSCAL1_ADDR, &fscal1.Value);
    // reg_read(GPIODAT_ADDR, &gpiodat.Value);
    // reg_read(GPIOCON_ADDR, &gpiocon.Value);

    HAL_GPIO_WritePin(ADC_START_GPIO_Port, ADC_START_Pin, GPIO_PIN_SET);

    // Set DATARATE register (ADR = 04h).The recommended sample rate is 200SPS
    // and single sample. This yield the following register value: 38h
    datarate.u.DR = (uint8_t)DATARATE_SELECTION_DR::SEL_200_SPS;
    reg_write(DATARATE_ADDR, datarate.Value);

    // Select channel to sampled by setting negative input mux to AINCOM and
    // channel will be the positive mux
    inpmux.u.MUXN = (uint8_t)INPMUX_Field::AINCOM;
    reg_write(INPMUX_ADDR, inpmux.Value);

    // Set appropriate PGA gain and if in bypass mode by write to register
    // PGA(ADR = 03h).
    pga.u.GAIN = (uint8_t)PGA_PAGE_SELECTION::GAIN_1;
    pga.u.PGA_EN = (uint8_t)PGA_ENABLE_Field::ENABLED;
    reg_write(PGA_ADDR, pga.Value);

    sys.u.CRC_ = (uint8_t)SYS_CONTROL_CRC::ENABLE;
    sys.u.SENDSTAT = (uint8_t)SYS_CONTROL_STATUS::ENABLE;
    sys.u.SYS_MON = (uint8_t)SYS_CONTROL_SYSMON::NORMAL;
    reg_write(SYS_ADDR, sys.Value);

    // Set reference control register (REF ADR = 05h) to internal 2.5v reference
    // always on by write value 3Ah
    ref.Value = 0x3A;
    reg_write(REF_ADDR, ref.Value);

    wake_up_the_device();
    return 1;
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

static HAL_StatusTypeDef reg_write(uint8_t reg, uint8_t data) {
    uint8_t tx[3];
    tx[0] = WREG_CMD | (reg & 0x1f);
    tx[1] = 0x00;
    tx[2] = data;

    return HAL_SPI_Transmit(SPI, tx, sizeof(tx), HAL_MAX_DELAY);
}

static HAL_StatusTypeDef reg_read(uint8_t reg, uint8_t* data) {
    uint8_t txrx[3];
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

// int read_channel_data(uint32_t* d_status, uint32_t* d_crc, int* d_data) {
//     *d_status = 0;
//     *d_data = 0;
//     *d_crc = 0;

//     GPIO_Write(GPIO_ADC_STRT);
//     usleep(2);

//     /* wait for nDRDY_REG to deassert as a known valid data */
//     while(GPIO_Read(GPIO_ADC_RDYn) == 1) {
//         usleep(2000);
//     }

//     /* Now read out the results as conversion is completed */
//     return data_read(d_status, d_crc, d_data);
// }

// static int data_read(uint32_t* d_status, uint32_t* d_crc, int* i_data) {
//     int i_data0;
//     int i_data1;

//     if(sys.u.SENDSTAT == (uint8_t)SysControlStatusEnum::ENABLE) {
//         if(SPI_ReadByte(adcSpiFD, d_status) == 0)
//             return 0;
//     }

//     // get the conversion data
//     if(SPI_ReadByte(adcSpiFD, &i_data0) == 0)
//         return 0;
//     if(SPI_ReadByte(adcSpiFD, &i_data1) == 0)
//         return 0;
//     *i_data = (i_data1 << 8) + i_data0;

//     if(sys.u.CRC_ == (uint8_t)SysControlCrcEnum::ENABLE) {
//         if(SPI_ReadByte(adcSpiFD, d_crc) == 0)
//             return 0;
//     }

//     return 1;
// }

} // namespace ADS114S08

#include "ADS114S08.h"

namespace ADS114S08 {

int Driver::init(SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* start_port,
                 uint16_t start_pin, GPIO_TypeDef* drdy_port, uint16_t drdy_pin,
                 GPIO_TypeDef* rst_port, uint16_t rst_pin) {

    if(spi_handle == nullptr || start_port == nullptr || drdy_port == nullptr ||
       rst_port == nullptr || start_pin == 0 || drdy_pin == 0 || rst_pin == 0) {
        return 1;
    }

    this->spi_handle = spi_handle;
    this->start_port = start_port;
    this->start_pin = start_pin;
    this->drdy_port = drdy_port;
    this->drdy_pin = drdy_pin;
    this->rst_port = rst_port;
    this->rst_pin = rst_pin;

    // Bring ADC out of reset
    HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_RESET);
    HAL_Delay(5); // Wait for ADC to power up and initialize
    HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_SET);
    HAL_Delay(10); // td(RSSC) after reset

    // Check if ADC connected
    if(reg_read(ID_ADDR, &id.value) != HAL_OK) {
        return 1;
    }
    if(id.u.DEV_ID != 0x4) {
        return 1;
    }

    // Reset ADC to default settings
    if(reset_device_to_default_settings() == HAL_ERROR)
        return 1;

    HAL_Delay(4);

    // Check if ADC reset ok
    if(reg_read(STATUS_ADDR, &status.value) != HAL_OK) {
        return 1;
    }
    if(status.value != REG_STATUS_DEFAULT) {
        return 1;
    }

    // Clear power-on-reset flag
    reg_write(STATUS_ADDR, 0x00);

    // Set internal voltage reference
    reg_write(REF_ADDR, 0x19);

    // Read all registers for verification
    reg_read(STATUS_ADDR, &status.value);
    reg_read(INPMUX_ADDR, &inpmux.value);
    reg_read(PGA_ADDR, &pga.value);
    reg_read(DATARATE_ADDR, &datarate.value);
    reg_read(REF_ADDR, &ref.value);
    reg_read(IDACMAG_ADDR, &idacmag.value);
    reg_read(IDACMUX_ADDR, &idacmux.value);
    reg_read(VBIAS_ADDR, &vbias.value);
    reg_read(SYS_ADDR, &sys.value);
    reg_read(OFCAL0_ADDR, &ofcal0.value);
    reg_read(OFCAL1_ADDR, &ofcal1.value);
    reg_read(FSCAL0_ADDR, &fscal0.value);
    reg_read(FSCAL1_ADDR, &fscal1.value);
    reg_read(GPIODAT_ADDR, &gpiodat.value);
    reg_read(GPIOCON_ADDR, &gpiocon.value);

    state = STANDBY_MODE;
    return 0;
}

void Driver::select_differential(INPMUX_Field muxp, INPMUX_Field muxn) {
    inpmux.u.MUXP = static_cast<uint8_t>(muxp);
    inpmux.u.MUXN = static_cast<uint8_t>(muxn);
    reg_write(INPMUX_ADDR, inpmux.value);
    reg_read(INPMUX_ADDR, &inpmux.value);
}

void Driver::select_single_ended(INPMUX_Field muxp) {
    select_differential(muxp, INPMUX_Field::AINCOM);
}

void Driver::start_conversions() {
    HAL_GPIO_WritePin(start_port, start_pin, GPIO_PIN_SET);
    HAL_Delay(1);
}

void Driver::config_pga(PGA_EN_Field pga_en, PGA_GAIN_Field gain) {
    pga.u.GAIN = static_cast<uint8_t>(gain);
    pga.u.PGA_EN = static_cast<uint8_t>(pga_en);
    reg_write(PGA_ADDR, pga.value);
    reg_read(PGA_ADDR, &pga.value);
}

void Driver::config_datarate(DR_SEL_Field dr, DR_MODE_Field mode,
                             DR_CLK_Field clk) {
    datarate.u.DR = static_cast<uint8_t>(dr);
    datarate.u.MODE = static_cast<uint8_t>(mode);
    datarate.u.CLK = static_cast<uint8_t>(clk);
    reg_write(DATARATE_ADDR, datarate.value);
    reg_read(DATARATE_ADDR, &datarate.value);
}

uint16_t Driver::data_decode_IT() {
    uint16_t data = (rx_buffer[1] << 8) | rx_buffer[2];
    return data;
}

} // namespace ADS114S08
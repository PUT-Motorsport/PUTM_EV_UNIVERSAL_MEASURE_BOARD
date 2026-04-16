#include "ADS114S08.h"
#include <cstdint>

namespace ADS114S08 {

int Instance::init() {
    // Check if ADC connected
    reg_read(ID_ADDR, &this->id.value);
    if(this->id.u.DEV_ID != 0x4) {
        return 1;
    }

    // Reset ADC to default settings
    if(reset_device_to_default_settings() == HAL_ERROR)
        return 1;

    HAL_TIM_Base_Start(TIM);
    while(__HAL_TIM_GET_COUNTER(TIM) < 4096) {
        // Reset command delay
    }

    // Check if ADC reset ok
    reg_read(STATUS_ADDR, &this->status.value);
    if(this->status.value != REG_STATUS_DEFAULT) {
        return 1;
    }

    // Clear power-on-reset flag
    reg_write(STATUS_ADDR, 0x00);

    // Read all registers for verification
    reg_read(INPMUX_ADDR, &this->inpmux.value);
    reg_read(PGA_ADDR, &this->pga.value);
    reg_read(DATARATE_ADDR, &this->datarate.value);
    reg_read(REF_ADDR, &this->ref.value);
    reg_read(IDACMAG_ADDR, &this->idacmag.value);
    reg_read(IDACMUX_ADDR, &this->idacmux.value);
    reg_read(VBIAS_ADDR, &this->vbias.value);
    reg_read(SYS_ADDR, &this->sys.value);
    reg_read(OFCAL0_ADDR, &this->ofcal0.value);
    reg_read(OFCAL1_ADDR, &this->ofcal1.value);
    reg_read(FSCAL0_ADDR, &this->fscal0.value);
    reg_read(FSCAL1_ADDR, &this->fscal1.value);
    reg_read(GPIODAT_ADDR, &this->gpiodat.value);
    reg_read(GPIOCON_ADDR, &this->gpiocon.value);

    return 1;
}

void Instance::select_channel(INPMUX_Field muxp, INPMUX_Field muxn) {
    this->inpmux.u.MUXN = static_cast<uint8_t>(muxn);
    this->inpmux.u.MUXP = static_cast<uint8_t>(muxp);
    reg_write(INPMUX_ADDR, this->inpmux.value);
}

void start_conversions() {
    HAL_GPIO_WritePin(START_PORT, START_PIN, GPIO_PIN_SET);
    HAL_TIM_Base_Start(TIM);
    while(__HAL_TIM_GET_COUNTER(TIM) < 4) {
    }
    HAL_GPIO_WritePin(START_PORT, START_PIN, GPIO_PIN_RESET);
}

void Instance::config_pga(PGA_EN_Field pga_en, PGA_GAIN_Field gain) {
    this->pga.u.GAIN = static_cast<uint8_t>(gain);
    this->pga.u.PGA_EN = static_cast<uint8_t>(pga_en);
    reg_write(PGA_ADDR, pga.value);
}

void Instance::config_datarate(DR_SEL_Field dr, DR_MODE_Field mode,
                               DR_CLK_Field clk) {
    datarate.u.DR = static_cast<uint8_t>(dr);
    datarate.u.MODE = static_cast<uint8_t>(mode);
    datarate.u.CLK = static_cast<uint8_t>(clk);
    reg_write(DATARATE_ADDR, datarate.value);
}

} // namespace ADS114S08
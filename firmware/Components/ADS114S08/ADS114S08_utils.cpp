#include "ADS114S08.h"
#include "config.h"

namespace ADS114S08 {

int init() {

    // Check if ADC connected
    reg_read(ID_ADDR, &id.Value);
    if(id.u.DEV_ID != 0x4) {
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
    reg_read(STATUS_ADDR, &status.Value);
    if(status.Value != REG_STATUS_DEFAULT) {
        return 1;
    }

    // Clear power-on-reset flag
    reg_write(STATUS_ADDR, 0x00);

    // Read all registers for verification
    reg_read(INPMUX_ADDR, &inpmux.Value);
    reg_read(PGA_ADDR, &pga.Value);
    reg_read(DATARATE_ADDR, &datarate.Value);
    reg_read(REF_ADDR, &ref.Value);
    reg_read(IDACMAG_ADDR, &idacmag.Value);
    reg_read(IDACMUX_ADDR, &idacmux.Value);
    reg_read(VBIAS_ADDR, &vbias.Value);
    reg_read(SYS_ADDR, &sys.Value);
    reg_read(OFCAL0_ADDR, &ofcal0.Value);
    reg_read(OFCAL1_ADDR, &ofcal1.Value);
    reg_read(FSCAL0_ADDR, &fscal0.Value);
    reg_read(FSCAL1_ADDR, &fscal1.Value);
    reg_read(GPIODAT_ADDR, &gpiodat.Value);
    reg_read(GPIOCON_ADDR, &gpiocon.Value);

    // HAL_GPIO_WritePin(ADC_START_GPIO_Port, ADC_START_Pin, GPIO_PIN_SET);

    // // Set DATARATE register (ADR = 04h).The recommended sample rate is
    // 200SPS
    // // and single sample. This yield the following register value: 38h
    // datarate.u.DR = (uint8_t)DATARATE_SELECTION_DR::SEL_200_SPS;
    // reg_write(DATARATE_ADDR, datarate.Value);

    // // Select channel to sampled by setting negative input mux to AINCOM and
    // // channel will be the positive mux
    // inpmux.u.MUXN = (uint8_t)INPMUX_Field::AINCOM;
    // reg_write(INPMUX_ADDR, inpmux.Value);

    // // Set appropriate PGA gain and if in bypass mode by write to register
    // // PGA(ADR = 03h).
    // pga.u.GAIN = (uint8_t)PGA_PAGE_SELECTION::GAIN_1;
    // pga.u.PGA_EN = (uint8_t)PGA_ENABLE_Field::ENABLED;
    // reg_write(PGA_ADDR, pga.Value);

    // sys.u.CRC_ = (uint8_t)SYS_CONTROL_CRC::ENABLE;
    // sys.u.SENDSTAT = (uint8_t)SYS_CONTROL_STATUS::ENABLE;
    // sys.u.SYS_MON = (uint8_t)SYS_CONTROL_SYSMON::NORMAL;
    // reg_write(SYS_ADDR, sys.Value);

    // // Set reference control register (REF ADR = 05h) to internal 2.5v
    // reference
    // // always on by write value 3Ah
    // ref.Value = 0x3A;
    // reg_write(REF_ADDR, ref.Value);

    // wake_up_the_device();
    return 1;
}

void select_channel(uint8_t muxp, uint8_t muxn) {
    inpmux.u.MUXP = muxp;
    inpmux.u.MUXN = muxn;
    reg_write(INPMUX_ADDR, inpmux.Value);
}

void start_conversions() {
    HAL_GPIO_WritePin(START_PORT, START_PIN, GPIO_PIN_SET);
    HAL_TIM_Base_Start(TIM);
    while(__HAL_TIM_GET_COUNTER(TIM) < 4) {
    }
    HAL_GPIO_WritePin(START_PORT, START_PIN, GPIO_PIN_RESET);
}

} // namespace ADS114S08
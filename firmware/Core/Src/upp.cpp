#include "upp.hpp"

int Adc::config_channel(ADS114S08B::INPMUX_Field input,
                        Channel::Voltage voltage, Channel::Gain gain) {
    if(input == ADS114S08B::INPMUX_Field::AINCOM ||
       voltage == Channel::Voltage::OFF || gain == Channel::Gain::OFF)
        return 1;
    for(auto& ch : channels) {
        if(ch.input == input) {
            if(((voltage == Channel::Voltage::U_5V ||
                 voltage == Channel::Voltage::U_12V) &&
                ch.type == Channel::Type::SINGLE_ENDED))
                return 1;
            else if(voltage == Channel::Voltage::U_3V3 &&
                    ch.type == Channel::Type::DIFFERENTIAL)
                return 1;
            else {
                ch.voltage = voltage;
                ch.gain = gain;
                if(!ch.status) {
                    ch.status = true;
                    active_channels_count++;
                }
                return 0;
            }
        }
    }
    return 1;
}

int Adc::deactivate_channel(ADS114S08B::INPMUX_Field input) {
    for(auto& ch : channels) {
        if(ch.input == input && ch.status == true) {
            ch.status = false;
            ch.gain = Channel::Gain::OFF;
            ch.voltage = Channel::Voltage::OFF;
            active_channels_count--;
            if(active_channels_count <= 0) {
                status = Status::STOP;
            }
            return 0;
        }
    }
    return 1;
}

int Adc::start() {
    if(active_channels_count <= 0) {
        return 1;
    }
    if(status == Status::STOP) {
        current_channel = 0;
        for(size_t i{}; i < channels.max_size(); i++) {
            if(channels[i].status == true) {
                current_channel = i;
                break;
            }
        }
        select_single_ended(channels[current_channel].input);

        switch(get_mode()) {
        case ADS114S08B::DR_MODE_Field::CONTINUOUS_CONVERSION_MODE:
            start_continous_conversions();
            break;
        case ADS114S08B::DR_MODE_Field::SINGLE_SHOT_CONVERSION_MODE:
            start_single_conversion();
            break;
        }
        status = Status::WAIT_FOR_DRDY;
        return 0;
    }
    return 1;
}

[[nodiscard]] std::optional<Adc::Channel> Adc::step() {
    switch(get_state()) {
    case Adc::State::CONTINUOUS_CONVERSION_MODE: {
        switch(status) {
        case Status::IDLE: {
            next();
            return std::nullopt;
        }
        case Status::DATA_READY: {
            read();
            return std::nullopt;
        }
        case Status::DECODE: {
            return update();
        }
        default:
            return std::nullopt;
        }
    }
    default:
        return std::nullopt;
    }
}

int Adc::next() {
    if(status == Status::IDLE) {
        if(active_channels_count <= 0)
            return 1;
        do {
            current_channel = (current_channel + 1) % channels.size();
        } while(channels[current_channel].status == false);
        select_single_ended(channels[current_channel].input);

        switch(get_state()) {
        case State::CONTINUOUS_CONVERSION_MODE: {
            break;
        }
        case State::STANDBY: {
            start_single_conversion();
            break;
        }
        default:
            return 1;
        }
        status = Status::WAIT_FOR_DRDY;
        return 0;
    }
    return 1;
}

int Adc::read() {
    if(status == Status::DATA_READY) {
        status = Status::WAIT_FOR_SPI;
        if(data_read_IT() != HAL_OK)
            return 1;
        else
            return 0;
    }
    return 1;
}

Adc::Channel Adc::update() {
    if(status == Status::DECODE) {
        switch(get_state()) {
        case State::CONTINUOUS_CONVERSION_MODE: {
            channels[current_channel].raw_value = decode_data_IT();
            break;
        }
        case State::SINGLE_CONVERSION_MODE: {
            channels[current_channel].raw_value = decode_data_IT();
            break;
        }
        default:
            break;
        }
        status = Status::IDLE;
    }
    return channels[current_channel];
}
#pragma once

#include "ADS114S08B.h"
#include <array>
#include <optional>

class Upp : public ADS114S08B::Driver {
  public:
    enum class Status {
        STOP,
        IDLE,
        WAIT_FOR_DRDY,
        DATA_READY,
        WAIT_FOR_SPI,
        DECODE,
    } volatile status{Status::STOP};

    class Channel {
      public:
        enum class Input : uint8_t {
            SINGLE_ENDED_1,
            SINGLE_ENDED_2,
            SINGLE_ENDED_3,
            DIFFERENTIAL_1,
            DIFFERENTIAL_5,
            DIFFERENTIAL_3,
            DIFFERENTIAL_4,
            DIFFERENTIAL_6,
            SINGLE_ENDED_6,
            DIFFERENTIAL_2,
            SINGLE_ENDED_5,
            SINGLE_ENDED_4,
        };

        const ADS114S08B::INPMUX_Field ads_input_id;

        enum class Type : uint8_t {
            SINGLE_ENDED,
            DIFFERENTIAL,
        } const type;

        enum class Voltage : int32_t {
            OFF,
            U_3V3 = 3300,
            U_5V = 5000,
            U_12V = 12000,
        } voltage{Voltage::OFF};

        enum class Gain : int32_t {
            OFF,
            G_1 = 1,
            G_5 = 5,
            G_25 = 25,
            G_100 = 100,
            G_1000 = 1000,
        } gain{Gain::OFF};

        int32_t get_voltage_mv() const {
            if(status == false) {
                return 0;
            }
            constexpr int32_t MAX_VAL_12B{32767};
            constexpr int32_t INA2128_OFFSET_VOLTAGE{6000};
            constexpr int32_t VOLTAGE_DIVIDER{3};
            int32_t value{MAX_VAL_12B + 1};
            switch(type) {
            case Type::SINGLE_ENDED: {
                value = (static_cast<int32_t>(raw_value) * ADC_VOLTAGE_REF) /
                        (MAX_VAL_12B * static_cast<int32_t>(gain));
                break;
            }
            case Type::DIFFERENTIAL: {
                value = (((static_cast<int32_t>(raw_value) * ADC_VOLTAGE_REF *
                           VOLTAGE_DIVIDER) /
                          MAX_VAL_12B) -
                         INA2128_OFFSET_VOLTAGE) /
                        static_cast<int32_t>(gain);
                break;
            }
            }
            return value;
        }

        bool status{false};
        int16_t raw_value{0};
    };

    static constexpr uint32_t CHANNEL_COUNT{12};
    static constexpr int32_t ADC_VOLTAGE_REF{5000};

    Upp(ADS114S08B::Driver driver_config) : ADS114S08B::Driver{driver_config} {}

    int config_channel(Channel::Input upp_input, Channel::Voltage voltage,
                       Channel::Gain gain);

    int deactivate_channel(Channel::Input upp_input);

    int start();

    void drdy_callback(uint16_t GPIO_Pin) {
        if(GPIO_Pin == drdy_pin) {
            if(status == Status::WAIT_FOR_DRDY) {
                status = Status::DATA_READY;
            };
        }
    }

    void spi_callback(SPI_HandleTypeDef* hspi) {
        if(hspi == &hspi1) {
            if(status == Status::WAIT_FOR_SPI)
                status = Status::DECODE;
        }
    }

    [[nodiscard]] std::optional<const Channel*> step();

  private:
    std::array<Channel, CHANNEL_COUNT> channels{
        {{ADS114S08B::INPMUX_Field::AIN0, Channel::Type::SINGLE_ENDED},
         {ADS114S08B::INPMUX_Field::AIN1, Channel::Type::SINGLE_ENDED},
         {ADS114S08B::INPMUX_Field::AIN2, Channel::Type::SINGLE_ENDED},
         {ADS114S08B::INPMUX_Field::AIN3, Channel::Type::DIFFERENTIAL},
         {ADS114S08B::INPMUX_Field::AIN4, Channel::Type::DIFFERENTIAL},
         {ADS114S08B::INPMUX_Field::AIN5, Channel::Type::DIFFERENTIAL},
         {ADS114S08B::INPMUX_Field::AIN6, Channel::Type::DIFFERENTIAL},
         {ADS114S08B::INPMUX_Field::AIN7, Channel::Type::DIFFERENTIAL},
         {ADS114S08B::INPMUX_Field::AIN8, Channel::Type::SINGLE_ENDED},
         {ADS114S08B::INPMUX_Field::AIN9, Channel::Type::DIFFERENTIAL},
         {ADS114S08B::INPMUX_Field::AIN10, Channel::Type::SINGLE_ENDED},
         {ADS114S08B::INPMUX_Field::AIN11, Channel::Type::SINGLE_ENDED}}};
    uint8_t current_channel{0};
    int8_t active_channels_count{0};

    int next();
    int read();
    const Channel* update();
};
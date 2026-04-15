cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_library(ADS114S08)

target_sources(ADS114S08
    PUBLIC
        ADS114S08_driver.cpp
        ADS114S08_utils.cpp
)

target_include_directories(ADS114S08
    PUBLIC
        include

)

target_link_libraries(ADS114S08
        stm32cubemx
)
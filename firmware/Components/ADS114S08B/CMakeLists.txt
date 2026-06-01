cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_library(ADS114S08B)

target_sources(ADS114S08B
    PUBLIC
        ADS114S08B.cpp
)

target_include_directories(ADS114S08B
    PUBLIC
        include

)

target_link_libraries(ADS114S08B
        stm32cubemx
)
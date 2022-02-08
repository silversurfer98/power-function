cmake_minimum_required(VERSION 3.2)
project(Fractional_Power_Function VERSION 1.0.2)

add_subdirectory(power_funcs)

add_executable(output main.cpp)

target_link_libraries(output PRIVATE power)
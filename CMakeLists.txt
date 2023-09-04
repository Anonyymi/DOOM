cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(DOOM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

FILE(GLOB DOOM_LINUX_IWADS "resources/iwad/*.wad")
FILE(GLOB DOOM_LINUX_PWADS "resources/pwad/*.wad")

add_subdirectory(bin-doom)

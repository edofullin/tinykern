cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER "riscv64-linux-gnu-gcc-14")
set(CMAKE_LINKER "riscv64-linux-gnu-ld")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tinykern VERSION 1.0
                  DESCRIPTION "Very nice kernel"
                  LANGUAGES C)

add_executable(image kernel/main.c)
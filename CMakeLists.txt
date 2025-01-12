cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tinykern VERSION 1.0
                  DESCRIPTION "Very nice kernel"
                  LANGUAGES C ASM)

add_subdirectory(kernel)
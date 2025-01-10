cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER "riscv64-unknown-linux-gnu-gcc")
set(CMAKE_LINKER "riscv64-unknown-linux-gnu-ld")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(K_SRCS kernel/entry.S kernel/centry.c)
set(K_LINK_O entry.S.o centry.o)
set(K_LINK_SCRIPT ${CMAKE_SOURCE_DIR}/kernel/kernel.ld)
set(K_LINK_OPT "-nostdlib")

project(tinykern VERSION 1.0
                  DESCRIPTION "Very nice kernel"
                  LANGUAGES C ASM)

add_executable(image ${K_SRCS})

target_link_libraries(image -T ${K_LINK_SCRIPT} ${K_LINK_OPT})
set_target_properties(image PROPERTIES LINK_DEPENDS ${K_LINK_SCRIPT})

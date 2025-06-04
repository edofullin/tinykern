set(K_LINK_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
set(K_LINK_OPTS "-nostdlib -lc -static")
set(K_C_FLAGS "-mcmodel=medany")

file(GLOB_RECURSE k_srcs
    "*.c"
    "*.S"
)
message(SOURCES=${k_srcs})
add_executable(kImage ${k_srcs})

target_link_libraries(kImage -T${K_LINK_SCRIPT} ${K_LINK_OPTS})
set_target_properties(kImage PROPERTIES LINK_DEPENDS ${K_LINK_SCRIPT} COMPILE_FLAGS ${K_C_FLAGS})
target_include_directories(kImage PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_target(qemu
                  DEPENDS kImage
                  COMMAND "qemu-system-riscv64" "-s" "-S" "-machine" "virt" "-serial" "stdio" "-bios" "none" "-kernel" "kImage" "-smp" "1" "-m" "128M")
set(K_LINK_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
set(K_LINK_OPTS "-nostdlib")

file(GLOB k_srcs
    "*.c"
    "*.S"
)

add_executable(kImage ${k_srcs})

target_link_libraries(kImage -T${K_LINK_SCRIPT} ${K_LINK_OPTS})
set_target_properties(kImage PROPERTIES LINK_DEPENDS ${K_LINK_SCRIPT})

add_custom_target(qemu
                  DEPENDS kImage
                  COMMAND "qemu-system-riscv64" "-s" "-S" "-machine" "virt" "-bios" "none" "-kernel" "kImage" "-smp" "1" "-m" "128M")
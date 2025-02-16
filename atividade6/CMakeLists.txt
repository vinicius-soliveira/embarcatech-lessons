add_executable(atividade6 main.c)

target_link_libraries(atividade6 pico_stdlib hardware_adc)

# enable usb output, disable uart output
pico_enable_stdio_uart(atividade6 0)
pico_enable_stdio_usb(atividade6 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(atividade6)

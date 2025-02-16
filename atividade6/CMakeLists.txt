add_executable (atividade6 main.c)

target_link_libraries(atividade6 pico_stdlib hardware_adc)

if (PICO_CYW43_SUPPORTED)
	target_link_libraries(atividade6 pico_cyw43_arch_none)
endif()

pico_enable_stdio_uart(atividade6 0)
pico_enable_stdio_usb(atividade6 1)

pico_add_extra_outputs(atividade6)

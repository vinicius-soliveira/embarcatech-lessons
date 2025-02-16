add_executable (atividade1 main.c)

target_link_libraries(atividade1 pico_stdlib hardware_timer hardware_gpio hardware_sync)

if (PICO_CYW43_SUPPORTED)
	target_link_libraries(atividade1 pico_cyw43_arch_none)
endif()

pico_enable_stdio_uart(atividade1 0)
pico_enable_stdio_usb(atividade1 1)

pico_add_extra_outputs(atividade1)

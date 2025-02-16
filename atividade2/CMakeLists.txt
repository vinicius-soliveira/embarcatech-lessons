add_executable (atividade2 main.c)

target_link_libraries(atividade2 pico_stdlib hardware_timer hardware_gpio)

if (PICO_CYW43_SUPPORTED)
	target_link_libraries(atividade2 pico_cyw43_arch_none)
endif()

pico_enable_stdio_uart(atividade2 0)
pico_enable_stdio_usb(atividade2 1)

pico_add_extra_outputs(atividade2)

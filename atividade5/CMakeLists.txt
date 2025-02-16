add_executable (atividade5 main.c)

target_link_libraries(atividade5 pico_stdlib hardware_pwm)

if (PICO_CYW43_SUPPORTED)
	target_link_libraries(atividade5 pico_cyw43_arch_none)
endif()

pico_enable_stdio_uart(atividade5 0)
pico_enable_stdio_usb(atividade5 1)

pico_add_extra_outputs(atividade5)

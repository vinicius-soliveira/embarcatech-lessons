add_executable (atividade3 main.c)

target_link_libraries(atividade3 pico_stdlib)

if (PICO_CYW43_SUPPORTED)
	target_link_libraries(atividade3 pico_cyw43_arch_none)
endif()

pico_enable_stdio_uart(atividade3 0)
pico_enable_stdio_usb(atividade3 1)

pico_add_extra_outputs(atividade3)

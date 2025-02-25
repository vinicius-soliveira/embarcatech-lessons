add_library(DS1307 STATIC ds1307.h)

target_include_directories(DS1307 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(DS1307 PUBLIC pico_stdlib hardware_i2c)

add_executable(atividade4 main.c ds1307.c)

target_link_libraries(atividade4 PRIVATE DS1307 pico_stdlib hardware_i2c)

pico_enable_stdio_uart(atividade4 0)
pico_enable_stdio_usb(atividade4 1)

pico_add_extra_outputs(atividade4)

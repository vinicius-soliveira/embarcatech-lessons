/*
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Leitura de data e hora de um RTC via comunicação I²C
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h"
#include "ds1307.h"

#define DS1307_ADDR       0x68
#define I2C_PORT          i2c0
#define I2C_FREQ          100*1000
#define SDA_PIN           4
#define SCL_PIN           5

#define REF_DAY_ON_WEEK   3
#define REF_DAY           24
#define REF_MONTH         9
#define REF_YEAR          24
#define REF_HOUR          13
#define REF_MIN           27
#define REF_SEC           0

uint8_t ref_datetime[] = {REF_SEC, REF_MIN, REF_HOUR,
                          REF_DAY_ON_WEEK, REF_DAY,
                          REF_MONTH, REF_YEAR};

uint8_t datetime[7];

int main() {
    stdio_init_all();

    // Inicializa e configura I2C
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Configura o RTC com um valor de referência
    while (!i2c_config_ref_datetime(I2C_PORT, DS1307_ADDR, ref_datetime, sizeof(ref_datetime))) {
        sleep_ms(100);
    }

    printf("RTC configurado com sucesso!\n");

    while (true) {
        // Lê a data/hora do RTC
        if (!i2c_read_datetime(I2C_PORT, DS1307_ADDR, datetime, sizeof(datetime))) {
            sleep_ms(500);
            continue;
        }

        // Exibe a data/hora formatada
        printf("Data: %02d/%02d/20%02d - Hora: %02d:%02d:%02d\n", 
               datetime[DAY], datetime[MONTH], datetime[YEAR], 
               datetime[HOUR], datetime[MIN], datetime[SEC]);

        sleep_ms(5000);  // Aguarda 5 segundos 
    }
}
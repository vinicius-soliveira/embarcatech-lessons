/*
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Acionar LED com botões (sem interrupções)
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_PIN     12
#define BTN_A_PIN   5
#define DEBOUNCE_DELAY_MS 50
#define BLINKING_TIME 10000 // 10 segundos

int count = 0;
int period = 100; // 100 ms = 1/10Hz
bool stop_blinking = false;

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    stdio_init_all();

    uint32_t start_time = 0;

    while (1) {
        if (!gpio_get(BTN_A_PIN)) { // Botão pressionado (ativo em LOW)
            sleep_ms(DEBOUNCE_DELAY_MS);
            if (!gpio_get(BTN_A_PIN)) { // Confirmação do debounce
                count++;
                count = count <= 5 ? count : 5;
                while (!gpio_get(BTN_A_PIN)); // Espera o botão ser solto
                sleep_ms(DEBOUNCE_DELAY_MS);
            }
        }

        if (count == 5 && !stop_blinking) {
            start_time = to_ms_since_boot(get_absolute_time());
            while (to_ms_since_boot(get_absolute_time()) - start_time < BLINKING_TIME) {
                gpio_put(LED_PIN, 1);
                sleep_ms(period);
                gpio_put(LED_PIN, 0);
                sleep_ms(period);
            }
            stop_blinking = true;
        }
    }

    printf("Fim de Programa!\n");
}


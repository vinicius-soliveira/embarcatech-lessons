/*
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Acionar LED com botões (com interrupção)
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/sync.h"

#define LED_PIN     12
#define BTN_A_PIN   5
#define DEBOUNCE_DELAY_MS 50
#define BLINKING_TIME 10000 // 10 segundos

volatile int count = 0;
int period = 100; // 100 ms = 1/10Hz
volatile bool stop_blinking = false;
volatile bool button_pressed = false;

static void gpio_callback(uint gpio, uint32_t events);
static int64_t timer_callback(alarm_id_t id, void *user_data);

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    stdio_init_all();

    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
   
    while (1) {
        if (button_pressed) {
            button_pressed = false; // Reset da flag após processar

            if (count == 5) {
                stop_blinking = false;
                add_alarm_in_ms(BLINKING_TIME, timer_callback, NULL, false);
                
                while (!stop_blinking) {
                    gpio_put(LED_PIN, 1);
                    sleep_ms(period);
                    gpio_put(LED_PIN, 0);
                    sleep_ms(period);
                }
            	break;
	    }
        }
    }

    printf("Fim de Programa!\n");
}

// Callback da interrupção do botão
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_A_PIN) {
        static uint32_t last_interrupt_time = 0;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (current_time - last_interrupt_time > DEBOUNCE_DELAY_MS) {
            last_interrupt_time = current_time;             
            count = count < 5 ? count + 1 : 5;
            button_pressed = true; // Flag para iniciar processo do LED piscar
        }
    }
}

// Callback do timer (executado após 10 segundos)
static int64_t timer_callback(alarm_id_t id, void *user_data) {
    stop_blinking = true;
    return 0; // Retorna 0 para não repetir
}


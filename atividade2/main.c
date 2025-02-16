/* 
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Acionar e controlar LED com botões
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/timer.h"

#define LED_PIN             11
#define BTN_A_PIN           5
#define BTN_B_PIN           6
#define DEBOUNCE_DELAY_MS   50  // Delay para evitar bounce
#define BLINKING_TIME       10000 // 10 segundos

volatile int count = 0;
volatile int period = 100; // 10 Hz
volatile bool stop_blinking = false; // Flag para parar o loop
volatile bool button_pressed = false; // Flag para iniciar o timer apenas uma vez

static void gpio_callback(uint gpio, uint32_t events);
static int64_t timer_callback(alarm_id_t id, void* user_data);
static bool debounce(uint gpio);

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);

    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  
    stdio_init_all();

    while(1) {
        if (button_pressed) {
            button_pressed = false;

            if (count == 5) {
                stop_blinking = false;
                add_alarm_in_ms(BLINKING_TIME, timer_callback, NULL, false);

                while (!stop_blinking) {
                    gpio_put(LED_PIN, 1);
                    sleep_ms(period);
                    gpio_put(LED_PIN, 0);
                    sleep_ms(period);
                }

                printf("Fim de Programa!\n");
                break; // Encerra o programa após o tempo de piscar
            }
        }
    }

    return 0;
}

// Função para debounce dos botões
static bool debounce(uint gpio) {
    static uint32_t last_interrupt_time_A = 0;
    static uint32_t last_interrupt_time_B = 0;

    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    uint32_t *last_time = (gpio == BTN_A_PIN) ? &last_interrupt_time_A : &last_interrupt_time_B;

    if (current_time - *last_time > DEBOUNCE_DELAY_MS) {
        *last_time = current_time;
        return true;
    }
    return false;
}

// Callback do timer
static int64_t timer_callback(alarm_id_t id, void *user_data) {
    stop_blinking = true; 
    return 0;
}

// Callback da interrupção dos botões
static void gpio_callback(uint gpio, uint32_t events) {
    if (!debounce(gpio)) return;

    if (gpio == BTN_A_PIN) {
        count = (count < 5) ? count + 1 : 5;
        period = 100;  
        button_pressed = true; // Sinaliza para o loop principal processar
    } 
    else if (gpio == BTN_B_PIN && count == 5) {
        period = 1000; // Reduz frequência para 1 Hz
    }
}


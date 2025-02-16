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
#define BLINKING_TIME       10000

volatile int count = 0;
volatile int period = 100; // 10 Hz
volatile bool stop_blinking = false; // Flag para parar o loop
volatile bool timer_started = false; // Flag para iniciar o timer apenas uma vez

static void isr_handler(uint gpio, uint32_t events);
static bool timer_callback(struct repeating_timer *t);

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_RISE, true, &isr_handler);
    gpio_set_irq_enabled_with_callback(BTN_B_PIN, GPIO_IRQ_EDGE_RISE, true, &isr_handler);
  
    stdio_init_all();
    
    struct repeating_timer timer;

    while (!stop_blinking) {  
        if (count == 5) {
            if (!timer_started) {
                // Inicia o timer apenas uma vez quando count for 5
                add_repeating_timer_ms(10000, timer_callback, NULL, &timer);
                timer_started = true;
            }

            // Pisca o LED com a frequência definida
            gpio_put(LED_PIN, 1);
            sleep_ms(period);
            gpio_put(LED_PIN, 0);
            sleep_ms(period);
        }
    }

    printf("Fim de Programa!\n");
}

// Função de callback do timer 
static bool timer_callback(struct repeating_timer *t) {
    stop_blinking = true; // Sinaliza que o tempo acabou
    return false; // Retorna false para parar o timer
}

// Função de interrupção dos botões
void isr_handler(uint gpio, uint32_t events) {
    gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_RISE, false);  // Desativa interrupção temporariamente
    sleep_ms(DEBOUNCE_DELAY_MS);  // Espera o tempo de debounce

    if (gpio == BTN_A_PIN) {
        count++;
        count = count <= 5 ? count : 5;
        period = 100;  
    } 
    else if (gpio == BTN_B_PIN && count == 5) {
        period = 1000; // Reduz frequência para 1 Hz
    }

    gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_RISE, true);  // Reativa interrupção
}

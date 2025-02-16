/*
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Acionar LED com botões
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/timer.h"

#define LED_PIN     12
#define BTN_A_PIN   5
#define DEBOUNCE_DELAY_MS 50
#define BLINKING_TIME 10000 // 10 segundos

volatile int count = 0;
int period = 100; // 100 ms = 1/10Hz
volatile bool stop_blinking = false; // Flag para parar o loop
volatile bool timer_started = false; // Flag para evitar reiniciar o timer

static void click_counter(uint gpio, uint32_t events);
static bool timer_callback(struct repeating_timer *t);

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &click_counter);
    
    stdio_init_all();

    struct repeating_timer timer;

    while (!stop_blinking) {  
        if (count == 5) {
            if (!timer_started) {
                add_repeating_timer_ms(BLINKING_TIME, timer_callback, NULL, &timer);
                timer_started = true;
            }

            // Pisca o LED
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

// Função de callback do botão
void click_counter(uint gpio, uint32_t events) {
    gpio_set_irq_enabled(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, false);  // Desativa interrupção temporariamente
    sleep_ms(DEBOUNCE_DELAY_MS);  // Espera o tempo de debounce

    count++;
    count = count <= 5 ? count : 5;

    gpio_set_irq_enabled(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true);  // Reativa interrupção
}

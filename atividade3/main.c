/*
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Validar comunicação UART com loopback
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_TX_PIN 4
#define UART1_RX_PIN 5

#define SIZE 100

char uart0_buf[SIZE] = {0};
char uart1_buf[SIZE] = {0};

int main() {
  stdio_init_all();
  
  // Configuração da UART
  uart_init(uart0, 115200);
  uart_init(uart1, 115200);
  gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);
  uart_set_fifo_enabled(uart0, true);
  uart_set_fifo_enabled(uart1, true);

  while (true) {
      printf("Digite uma mensagem: ");
      scanf(" %99[^\n]", &uart0_buf);


    while (uart_is_readable(uart1)){
        uart_getc(uart1);
    }

    uart_puts(uart0, uart0_buf);
    uart_putc(uart0, '\n');  // Caractere de terminação

    printf("Mensagem enviada pela UART0: %s\n", uart0_buf);
    int index = 0;
     
    while (index < SIZE -1) {
        if (uart_is_readable(uart1)) {
            char c = uart_getc(uart1);
            if (c == '\n') break;  // Finaliza leitura, quando encontra caractere de terminação
            uart1_buf[index++] = c;
        }
    }
      uart1_buf[index] = '\0'; 
      printf("Mensagem recebida pela UART1: %s\n", uart1_buf);
      sleep_ms(500);
    
      printf("----------------------------------------------------\n\n");
  }
  
}



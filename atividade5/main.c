/*
 * Copyright (c) 2025 Embarcatech 
 * Author: Vinicius Oliveira <vinsoliveira@gmail.com>
 * Function: Controle de luminosidade de LED, utilizando PWM
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"


#define SYSTEM_CLOCK_FREQ    125*1000*1000
#define LED_VD_PIN           11
#define LED_AZ_PIN           12
#define LED_VM_PIN           13

#define LED_VM_FREQ          1000
#define LED_VD_FREQ          10000

#define PWM_REFRESH_LEVEL    22000
#define DUTY_MIN             5
#define DUTY_STEP            5
#define DUTY_MAX             100

volatile uint8_t duty_cycle = DUTY_MIN;
volatile bool up_down = true; // up = true, down = false
static uint32_t count = 0; 

typedef struct {
    uint8_t gpio;
    uint8_t slice;
    int freq;
    float clkdiv;      
    uint16_t wrap;     
    
} pwm_config_t;


// Instâncias das configurações de PWM para os dois LEDs
pwm_config_t pwm_led_vm = {LED_VM_PIN, 0, LED_VM_FREQ, 0, 0}; 
pwm_config_t pwm_led_vd = {LED_VD_PIN, 0, LED_VD_FREQ, 0, 0}; 


void pwm_irq_handler(){
    pwm_clear_irq(pwm_led_vm.slice); 
    pwm_clear_irq(pwm_led_vd.slice); 
    
    if (count++ < PWM_REFRESH_LEVEL) return;
    count = 0; 
    
    if (up_down){
      duty_cycle += DUTY_STEP; 
      if (duty_cycle >= DUTY_MAX){
        duty_cycle = DUTY_MAX;
        up_down = 0;
      }
    }else{
      duty_cycle -= DUTY_STEP; 
      if (duty_cycle <= DUTY_MIN){
        duty_cycle = DUTY_MIN;
        up_down = 1; 
      }
        
    }

    pwm_set_gpio_level(pwm_led_vm.gpio, (duty_cycle*pwm_led_vm.wrap)/100); 
    pwm_set_gpio_level(pwm_led_vd.gpio, (duty_cycle*pwm_led_vd.wrap)/100);

}

void calc_parameters(pwm_config_t* pwm) {
    
    float clkdiv = 1.0;
    uint32_t wrap = SYSTEM_CLOCK_FREQ / pwm->freq - 1;

    if (wrap > 0xFFFF){
        clkdiv = (float)SYSTEM_CLOCK_FREQ / (pwm->freq * (0xFFFF + 1));
        wrap = 0xFFFF;
    }

    pwm->clkdiv = clkdiv;
    pwm->wrap = wrap;
}

void setup_pwm(pwm_config_t* pwm){
  
  gpio_set_function(pwm->gpio, GPIO_FUNC_PWM);
  pwm->slice = pwm_gpio_to_slice_num(pwm->gpio);

  calc_parameters(pwm);

  pwm_set_clkdiv(pwm->slice, pwm->clkdiv); 
  pwm_set_wrap(pwm->slice, pwm->wrap); 
  pwm_set_gpio_level(pwm->gpio, duty_cycle); 
  pwm_set_enabled (pwm->slice, true);

  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_handler);
  pwm_clear_irq (pwm->slice);
  pwm_set_irq_enabled (pwm->slice, true);
  irq_set_enabled (PWM_IRQ_WRAP, true);

}

int main() {
  stdio_init_all();

  setup_pwm(&pwm_led_vm);
  setup_pwm(&pwm_led_vd);

  while (true) {
     printf("Duty Cycle atualizado: %d%%\n", duty_cycle);
     sleep_ms(2000);
    
  }
}
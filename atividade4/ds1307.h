#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h"

enum index{
  SEC, MIN, HOUR, DAY_WEEK, DAY, MONTH, YEAR    
}; 

bool i2c_read_datetime(i2c_inst_t*, uint8_t, uint8_t*, uint8_t);
bool i2c_config_ref_datetime(i2c_inst_t*, uint8_t,  uint8_t*, uint8_t);
uint8_t dec_to_bcd(uint8_t);
uint8_t bcd_to_dec(uint8_t);
#include "ds1307.h"


uint8_t dec_to_bcd(uint8_t dec){
  uint8_t bcd = ((dec / 10) << 4 ) | (dec % 10);
  return bcd;
}

uint8_t bcd_to_dec(uint8_t bcd){
  uint8_t dec = (10*(bcd >> 4)) + (bcd & 0x0F);
  return dec;
}

bool i2c_read_datetime(i2c_inst_t *i2c, uint8_t address, uint8_t* data, uint8_t size){
  
   if (size < 7) return false;  
    uint8_t cmd = 0x00;  

    if (i2c_write_blocking(i2c, address, &cmd, 1, true) < 0) {
        return false;
    }

    if (i2c_read_blocking(i2c, address, data, size, false) != size) {
        return false;
    }

    data[SEC]  = bcd_to_dec(data[SEC]  & 0x7F); 
    data[MIN]  = bcd_to_dec(data[MIN]);
    data[HOUR] = bcd_to_dec(data[HOUR] & 0x3F); 
    data[DAY_WEEK] = bcd_to_dec(data[DAY_WEEK]);
    data[DAY]  = bcd_to_dec(data[DAY]);
    data[MONTH] = bcd_to_dec(data[MONTH]);
    data[YEAR] = bcd_to_dec(data[YEAR]);

  return true;
}

bool i2c_config_ref_datetime(i2c_inst_t *i2c, uint8_t address, uint8_t* data, uint8_t size) {
    if (size < 7) return false;  

    uint8_t buffer[size + 1]; 
    buffer[0] = 0x00;  

    for (int i = 0; i < size; i++) {
        buffer[i + 1] = dec_to_bcd(data[i]);
    }

    buffer[1] &= 0x7F;  // Ativa clock

    int ret = i2c_write_blocking(i2c, address, buffer, size + 1, false);
    if (ret < 0) return false;  

    return ret == size + 1;  
}
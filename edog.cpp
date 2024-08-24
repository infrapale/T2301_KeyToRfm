#include "Arduino.h"
#include <Wire.h>
#include "edog.h"
#include "helper.h"

/*
uint8_t tarr1[I2C_TX_BUFF_SIZE];
uint8_t tarr2[I2C_TX_BUFF_SIZE];

i2c_st i2c;


void edog_put_tx_buff_uint32( uint16_t offs, uint32_t u32)
{
    i2c.tx_buff[offs + 0 ] = (u32 >> 24) & 0x000000FF;
    i2c.tx_buff[offs + 1 ] = (u32 >> 16) & 0x000000FF;
    i2c.tx_buff[offs + 2 ] = (u32 >> 8) & 0x000000FF;
    i2c.tx_buff[offs + 3 ] = (u32 >> 0) & 0x000000FF;
}

void edog_put_tx_buff_uint16( uint16_t offs, uint16_t u16)
{
    i2c.tx_buff[offs + 0 ] = (u16 >> 8) & 0x000000FF;
    i2c.tx_buff[offs + 1 ] = (u16 >> 0) & 0x000000FF;
}
void edog_put_tx_buff_uint8( uint16_t offs, uint8_t u8)
{
    i2c.tx_buff[offs + 0 ] = (u8 >> 0) & 0x000000FF;
}

uint32_t edog_get_rx_buff_uint32(uint16_t offs)
{
    uint32_t u32 = 0;
    u32 |= ((uint32_t)i2c.rx_buff[offs + 0 ] << 24) & 0x0000000FFUL;
    u32 |= ((uint32_t)i2c.rx_buff[offs + 1 ] << 16) & 0x0000000FFUL;
    u32 |= ((uint32_t)i2c.rx_buff[offs + 2 ] << 8)  & 0x0000000FFUL;
    u32 |= ((uint32_t)i2c.rx_buff[offs + 3 ] << 0)  & 0x0000000FFUL;
    return u32;
}

uint16_t edog_get_rx_buff_uint16(uint16_t offs)
{
    uint16_t u16 = 0;
    u16 |= ((uint16_t)i2c.rx_buff[offs + 0 ] << 8)  & 0x0000000FFUL;
    u16 |= ((uint16_t)i2c.rx_buff[offs + 1 ] << 0)  & 0x0000000FFUL;
    return u16;
}

uint8_t edog_get_rx_buff_uint8(uint16_t offs)
{
    uint8_t u8 = 0;
    u8 = ((uint8_t)i2c.rx_buff[offs + 0 ] << 0)  & 0x0000000FFUL;
    return u8;
}


void edog_build_test_data(void)
{
  for( uint8_t i = 0; i < 8; i++)
  {
    tarr1[i] = i*2;
    tarr2[i] = i*3;
  }
}

  

void edog_initialize(uint8_t i2c_addr)
{
  i2c.addr = i2c_addr;
  edog_build_test_data();
}

void edog_print_rx_buff(void)
{
  Serial.print("i2c.rx: ");
  helper_print_hex_arr(i2c.rx_buff,9);
}

void edog_print_tx_buff(void)
{
  Serial.print("i2c.tx: ");
  helper_print_hex_arr(i2c.tx_buff,9);
}

void edog_receive_i2c(void)
{
  if(i2c.reg_m2s > 0)
  {
    Wire.beginTransmission(i2c.addr); 
    Wire.write( i2c.tx_buff, i2c.reg_m2s + 1)  ;      
    Wire.endTransmission();
  }  
}

void edog_read_i2c(uint8_t bytes)
{
    Wire.requestFrom(i2c.addr, bytes);   
    uint8_t i = 0; 
    while(Wire.available())    
    { 
      int c = Wire.read();
      i2c.rx_buff[i++] = (uint8_t) c;
    }
}

void edog_send_i2c(void)
{
  {
    Wire.beginTransmission(i2c.addr); 
    Wire.write( i2c.tx_buff, i2c.reg_m2s + 1)  ;      
    Wire.endTransmission();
  }  
}

void edog_set_read_pos(uint8_t pos)
{
  Serial.println("Set read pos");
  edog_build_uint_msg(REG_ADDR_SET_RD_POS, pos, 1, 0);
  edog_send_i2c();
}

void edog_rd_reg(uint8_t pos, uint8_t len)
{
  edog_set_read_pos(pos);
  delay(1);
  Wire.requestFrom(i2c.addr, len); 
  Wire.requestFrom(i2c.addr, i2c.reg_s2m);   
  uint8_t i = 0; 
  while(Wire.available())    
  { 
    int c = Wire.read();
    i2c.rx_buff[i++] = (uint8_t) c;
  }
  edog_print_rx_buff();
}
 

void edog_send_receive(void)
{

  Serial.print("send_receive: "); 
  // Serial.print(i2c.addr, HEX); 
  Serial.print(" reg_m2s="); Serial.print(i2c.reg_m2s);
  Serial.print(" reg_s2m="); Serial.print(i2c.reg_s2m);
  Serial.println(" ");
  Serial.flush();

  Wire.beginTransmission(i2c.addr); 
  Wire.write( i2c.tx_buff, i2c.reg_m2s + 1)  ;      
  Wire.endTransmission();

  if(i2c.reg_s2m > 0)
  {
    // delay(1);
    //Wire.beginTransmission(i2c.addr); 
    Wire.requestFrom(i2c.addr, i2c.reg_s2m);   
    uint8_t i = 0; 
    while(Wire.available())    
    { 
      int c = Wire.read();
      i2c.rx_buff[i++] = (uint8_t) c;
    }

    //Wire.endTransmission();
  }
}

void edog_build_uint_msg(uint8_t raddr, uint32_t value, uint8_t m2s, uint8_t s2m)
{
  uint8_t aindx = 0;    //[0] is reserved for register address
  i2c.reg_addr = raddr;
  i2c.reg_data = value;
  i2c.reg_m2s = m2s;
  i2c.reg_s2m = s2m;

  i2c.tx_buff[aindx++] = i2c.reg_addr;
  if (i2c.reg_m2s == 4)
  {
    i2c.tx_buff[aindx++] = (i2c.reg_data >> 24) & 0x000000FF;
    i2c.tx_buff[aindx++] = (i2c.reg_data >> 16) & 0x000000FF;
  }
  if (i2c.reg_m2s > 1)
  {
    i2c.tx_buff[aindx++] = (i2c.reg_data >> 8) & 0x000000FF;
  }
  i2c.tx_buff[aindx] = (i2c.reg_data >> 0) & 0x000000FF;
}

void edog_build_array_msg(uint8_t raddr, uint8_t *arr, uint8_t m2s, uint8_t s2m)
{
  uint8_t aindx = 0;    //[0] is reserved for register address
  i2c.reg_addr = raddr;
  i2c.reg_m2s = m2s;
  i2c.reg_s2m = s2m;
  // Serial.printf("i2c.reg_addr %02X %02X\n", i2c.reg_addr, i2c.tx_buff[0]); Serial.flush();
  i2c.tx_buff[aindx++] = i2c.reg_addr;
  for (uint8_t i = 0; i < m2s; i++)
  {
    i2c.tx_buff[aindx++] = arr[i];
  }
}


void edog_set_wd_timeout(uint32_t wd_timeout)
{
  Serial.printf("Watchdog timeout = %d\n\r", wd_timeout);
  edog_build_uint_msg(REG_ADDR_WD_INTERVAL, wd_timeout, 4, 0);
  edog_send_receive();
}

void edog_set_sleep_time(uint32_t sleep_time)
{
  Serial.printf("Sleep time = %d\n\r",sleep_time);
  edog_build_uint_msg(REG_ADDR_SLEEP_TIME, sleep_time, 4, 0);
  edog_send_receive();
}

void edog_clear_watchdog(void)
{
  // Serial.println("Clear watchdogSleep time = ");
  edog_build_uint_msg(REG_ADDR_CLEAR_WATCHDOG, 1, 1, 0);
  edog_send_receive();
}

void edog_switch_off(void)
{
  Serial.println("Goto Sleep");
  edog_build_uint_msg(REG_ADDR_POWER_OFF_0, 1, 1, 0);
  edog_send_receive();
}

// void edog_switch_off_1(uint8_t value)
// {
//   Serial.println("Switch off 1");
//   edog_build_uint_msg(REG_ADDR_SWITCH_OFF_1, value, 1, 0);
//   edog_send_receive();
// }
void edog_ext_reset(uint8_t value)
{
  Serial.println("External Reset");
  edog_build_uint_msg(REG_ADDR_EXT_RESET, value, 1, 0);
  edog_send_receive();
}
void edog_load_eeprom(void)
{
  Serial.println("Load EEPROM Data");
  edog_build_uint_msg(REG_ADDR_EEPROM_LOAD, 1, 1, 0);
  edog_send_i2c();
}

void edog_save_eeprom(void)
{
  Serial.println("Save EEPROM Data");
  edog_build_uint_msg(REG_ADDR_EEPROM_SAVE, 1, 1, 0);
  edog_send_i2c();
}


void edog_read_eeprom(uint16_t addr)
{
  Serial.printf("Read EEPROM @ %04X\n\r", addr);
  delay(5);
  edog_build_uint_msg(REG_ADDR_EEPROM_ADDR, (uint32_t) addr, 2, 0);
  edog_send_i2c();
  delay(5);

  edog_build_uint_msg(REG_ADDR_EEPROM_LOAD, 0, 1, 0);
  edog_send_i2c();
  delay(10);

  edog_build_uint_msg(REG_ADDR_EEPROM_READ, 0, 0, 8);
  // edog_receive_i2c();
  edog_send_receive();  
  edog_print_rx_buff();
}

void edog_write_eeprom(uint16_t addr, uint8_t *arr)
{
  Serial.printf("Write EEPROM @ %04X", addr);

  edog_build_uint_msg(REG_ADDR_EEPROM_ADDR, (uint32_t) addr, 2, 0);
  delay(5);
  edog_send_i2c();
  
  delay(5);
  edog_build_array_msg(REG_ADDR_EEPROM_WRITE, arr, 8, 0);
  edog_send_i2c();
  delay(10);

  edog_build_uint_msg(REG_ADDR_EEPROM_SAVE, 0, 1, 0);
  edog_send_i2c();
  edog_print_tx_buff();
}

void edog_write_eeprom_buff(uint16_t addr)
{
  Serial.printf("Write EEPROM @ %04X", addr);
 
  edog_build_uint_msg(REG_ADDR_EEPROM_ADDR, (uint32_t) addr, 2, 0);
  edog_send_i2c();
  
  delay(1);
  i2c.reg_addr = REG_ADDR_EEPROM_WRITE;
  i2c.reg_m2s = 9;
  i2c.reg_s2m = 0;
  i2c.tx_buff[0] = i2c.reg_addr;
  edog_send_i2c();
  delay(1);

  edog_build_uint_msg(REG_ADDR_EEPROM_SAVE, 0, 1, 0);
  edog_send_i2c();
  delay(10);
  edog_print_tx_buff();
}


void edog_test_eeprom_write_read(void)
{
  edog_build_test_data();
  edog_write_eeprom(0x0010, tarr1);
  delay(20);
  edog_read_eeprom(0x0010);
  delay(20);
}
*/

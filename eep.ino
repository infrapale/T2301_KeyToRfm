#include "main.h"
#include <EEPROM.h>

typedef struct 
{
    uint16_t  addr;
    uint16_t  size;
} eep_st;


eep_st eep;

void eep_initialize(uint16_t size)
{
  EEPROM.begin(size);
  eep.addr = 0;
  eep.size = size;
}

bool eep_set_addr(uint16_t addr)
{
  bool is_ok = fasle;
  if (addr < eep.size)
  {
    eep.addr = addr;
    is_ok = true;
  }
  return is_ok;
}

bool eep_next(void)
{
  bool is_ok = fasle;
  if (eep.addr < eep.size)
  {
    eep_addr++;
    is_ok = true;
  }
  return is_ok;
}

bool eep_commit()
{
  return EEPROM.commit();
}

void eep_write_u8(uint8_t u8)
{
    EEPROM.write(eep.addr, u8);
    eep.addr++;
}

void eep_write_u16(uint16t u16)
{
    uint8_t u8;
    u8 = (uint8_t) ((u16 >> 8) & 0xFF);
    EEPROM.write(eep.addr, u8);
    eep.addr++;
    u8 = (uint8_t) ((u16 >> 0) & 0xFF);
    EEPROM.write(eep.addr, u8);
    eep.addr++;
}

uint8_t eep_read_u8(void)
{
  uint8_t u8 = EEPROM.read(eep.addr);
  eep.addr++;
  return (u8);
}

uint8_t eep_read_u16(void)
{
  uint16_t u16;
  uint8_t u8 = EEPROM.read(eep.addr);
  u16 = (uint16_t) (u8 << 8);
  eep.addr++;
  u8 = EEPROM.read(eep.addr);
  u16 |= u8; 

  return (u8);
}


value = EEPROM.read(address);
#ifndef __EDOG_H__
#define __EDOG_H__


#define REG_ARRAY_LEN 20


#define I2C_RX_BUFF_SIZE   (16)
#define I2C_TX_BUFF_SIZE   (16)

#define I2C_ADDR    13

typedef struct
{
  char      mode;
  uint8_t   addr;
  uint8_t   reg_addr;
  uint32_t  reg_data;
  uint8_t   reg_m2s;
  uint8_t   reg_s2m;
  uint8_t   tx_buff[I2C_TX_BUFF_SIZE];
  uint8_t   rx_buff[I2C_RX_BUFF_SIZE];

} i2c_st;

typedef enum 
{
  MASTER_TO_SLAVE = 0,
  SLAVE_TO_MASTER 
} master_slave_direction_et;

typedef enum
{
  REG_ADDR_SET_RD_POS       = 0x00,
  REG_ADDR_CLEAR_WATCHDOG   = 0x08,
  REG_ADDR_SWITCH_OFF       = 0x09,
  REG_ADDR_EEPROM_ADDR      = 0x0A,
  REG_ADDR_EEPROM_LOAD      = 0x0C,
  REG_ADDR_EEPROM_SAVE      = 0x0D,
  REG_ADDR_POWER_OFF_0      = 0x0E,
  REG_ADDR_EXT_RESET        = 0x0F,
  REG_ADDR_WD_INTERVAL      = 0x10,
  REG_ADDR_SLEEP_TIME       = 0x14,
  REG_ADDR_EEPROM_READ      = 0x20,
  REG_ADDR_EEPROM_WRITE     = 0x30,

  
  EEPROM_ADDR_APP_DATA      = 0x0000,
  EEPROM_ADDR_USER_END      = 0x017F,
  EEPROM_ADDR_WD_INTERVAL   = 0x0180,
  EEPROM_ADDR_SLEEP_TIME    = 0x0184,

  REG_ADDR_3 = 0x03,
  REG_ADDR_4 = 0x04,
  REG_ADDR_5 = 0x05,
} reg_addr_et;

typedef enum
{
  EEPROM_FUNC_READ        = 0x01,
  EEPROM_FUNC_WRITE       = 0x02,
} eeprom_func_et;

typedef enum
{
  EEPROM_STATUS_RD_READY    = 0x01,
  EEPROM_STATUS_WR_READY    = 0x02,
} eeprom_status_et;

void edog_initialize(uint8_t i2c_addr);

uint32_t edog_get_rx_buff_uint32(uint16_t offs);

uint16_t edog_get_rx_buff_uint16(uint16_t offs);

uint8_t edog_get_rx_buff_uint8(uint16_t offs);

void edog_put_tx_buff_uint32( uint16_t offs, uint32_t u32);

void edog_put_tx_buff_uint16( uint16_t offs, uint16_t u16);

void edog_put_tx_buff_uint8( uint16_t offs, uint8_t u8);

void edog_rd_reg(uint8_t pos, uint8_t len);

void edog_test_eeprom_write_read(void);

void edog_build_uint_msg(uint8_t raddr, uint32_t value, uint8_t m2s, uint8_t s2m);


void edog_build_array_msg(uint8_t raddr, uint8_t *arr, uint8_t m2s, uint8_t s2m);

void edog_build_test_data(void);

void edog_read_i2c(uint8_t bytes);

void edog_test_eeprom_write_read(void);

void edog_set_wd_timeout(uint32_t wd_timeout);

void edog_set_sleep_time(uint32_t sleep_time);

void edog_clear_watchdog(void);

void edog_switch_off(void);

void edog_switch_off_1(uint8_t value);

void edog_ext_reset(uint8_t value);

void edog_load_eeprom(void);

void edog_save_eeprom(void);


void edog_read_eeprom(uint16_t addr);

void edog_write_eeprom(uint16_t addr, uint8_t *arr);

void edog_write_eeprom_buff(uint16_t addr);

void edog_print_rx_buff(void);

void edog_print_tx_buff(void);


#endif
#ifndef __EEP_H__
#define __EEP_H__

#define EEP_SIZE              256
#define EEP_ADDR_MAIN_STATE   0x0000
#define EEP_ADDR_RESTART_CNTR 0x0002
#define EEP_ADDR_2
#define EEP_ADDR_3

void eep_initialize(uint16_t size);

bool eep_set_addr(uint16_t addr);

bool eep_next(void);

bool eep_commit(void);

void eep_request_commit(uint16_t commit_in_sec);

void eep_write_u8(uint8_t u8);

void eep_write_u16(uint16_t u16);

uint8_t eep_read_u8(void);

uint16_t eep_read_u16(void);

void eep_time_machine(void);;

#endif
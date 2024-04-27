#ifndef __SUPERVISOR_H__
#define __SUPERVISOR_H__

void supervisor_initialize(void);

void supervisor_task(void);

uint8_t supervisor_get_pir(void);

uint16_t supervisor_get_ldr(void);


#endif

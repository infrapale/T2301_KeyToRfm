#ifndef __SUPERVISOR_H__
#define __SUPERVISOR_H__

typedef enum
{
  SUPER_ERR_GET_TIME = 0,
  SUPER_ERR_1,
  SUPER_ERR_NBR_OF
} super_err_et;


void supervisor_initialize(void);

void supervisor_inc_cntr(super_err_et cntr_indx);

void supervisor_clr_cntr(super_err_et cntr_indx);

void supervisor_debug_print(void);

bool supervisor_pwr_is_on(void);

void supervisor_task(void);

uint8_t supervisor_get_pir(void);

uint16_t supervisor_get_ldr(void);


#endif

#include "main.h"
#include "signal.h"
#include "task.h"
#include "edog.h"
#include "supervisor.h"

typedef struct
{
  uint16_t err_cntr[SUPER_ERR_NBR_OF];
  uint16_t ldr_val;
  uint8_t  pir_val;
  task_st  *sm; 
  uint8_t  wd_pin_state;
} supervisor_st;

uint16_t err_limit[SUPER_ERR_NBR_OF] =
{
  [SUPER_ERR_GET_TIME] = 4,
  [SUPER_ERR_1]        = 1,
};

supervisor_st super;

void supervisor_initialize(void)
{
  super.sm = task_get_task(TASK_SUPERVISOR);
  super.sm->state = 0;
  super.ldr_val = 0;
  super.wd_pin_state = 0;
  for (uint8_t i = 0; i < SUPER_ERR_NBR_OF; i++) super.err_cntr[i] = 0;

  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_PIR, INPUT);
}

void supervisor_inc_cntr(super_err_et cntr_indx)
{
  super.err_cntr[cntr_indx]++;
}

void supervisor_clr_cntr(super_err_et cntr_indx)
{
  super.err_cntr[cntr_indx] = 0;
}

uint8_t supervisor_get_pir(void)
{
  return super.pir_val;
}

uint16_t supervisor_get_ldr(void)
{
  return super.ldr_val;
}

void supervisor_debug_print(void)
{
  Serial.print("Supervisor err_cntr: ");
    for (uint8_t i = 0; i < SUPER_ERR_NBR_OF; i++) 
    {
      Serial.printf("%d ", super.err_cntr[i]);
    }
  Serial.println();
}

void supervisor_task(void)
{
  uint8_t tindx;
  super_err_et err_cntr_at_limit = SUPER_ERR_NBR_OF;
  bool clr_edog = false;

  // WD DEBUG super.err_cntr[SUPER_ERR_GET_TIME]++;  // DEBUG -REMOVE

  switch(super.sm->state)
  {
    case 0:
      super.ldr_val = analogRead(PIN_LDR);
      super.pir_val = digitalRead(PIN_PIR);
      super.sm->state = 10;
      for (uint8_t i = 0; i < SUPER_ERR_NBR_OF; i++) 
      {
        if (super.err_cntr[i] >= err_limit[i])
        {
          err_cntr_at_limit = (super_err_et)i;
          break;
        } 
      }

      if (err_cntr_at_limit < SUPER_ERR_NBR_OF)
      {
        Serial.printf("Error limit exceeded: index = %d cntr = %d\n", 
          err_cntr_at_limit, super.err_cntr[err_cntr_at_limit]);
          super.sm->state = 100;
          clr_edog = false;
      }
      else
      {
         clr_edog = true;
      }
      break;
    case 10:
      super.sm->state = 0;
      tindx = task_check_all();
      if (tindx < TASK_NBR_OF) 
      {
        task_st  *tptr = task_get_task(tindx);
        Serial.printf("!!! Task counter overflow: %s %d\n", tptr->name, tptr->wd_cntr);
        task_print_status(true);       
        super.sm->state = 100;
      } 
      else 
      {
        clr_edog = true;
      }
      break;
    case 3:
      super.sm->state++;
      break;
    case 4:
      super.sm->state++;
      break;
    case 5:
      super.sm->state++;
      break;
    case 6:
      super.sm->state++;
      break;
    case 7:
      super.sm->state++;
      break;
    case 100:
      super.sm->state++;
      Serial.printf("!!! Waiting for Watchdog Reset\n\r");
      break;
    case 101:
      break;
    default:
      super.sm->state = 0;
      break;

  }
  // if (clr_edog)  edog_clear_watchdog();
  if (clr_edog ) 
  {
    if (super.wd_pin_state == 0) super.wd_pin_state = 1;
    else super.wd_pin_state = 0;
    digitalWrite(PIN_WD_RESET,super.wd_pin_state);
  }
}
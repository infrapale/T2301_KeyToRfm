
#include "sema.h"


typedef struct
{
    bool reserved;
} sema_st;

sema_st sema[SEMA_NBR_OF];


bool sema_reserve( uint8_t sema_indx)
{
   if (sema[sema_indx].reserved)
   {
      return false;
   } 
   else 
   {
      sema[sema_indx].reserved = true;
      return true;
   }
}

void sema_release(uint8_t sema_indx)
{
    sema[sema_indx].reserved  = false;
}
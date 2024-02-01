#ifndef __TASK_H__
#define __TASK_H__

#define TASK_RESERVED_NBR  8

typedef void (*task_cb)(void);

typedef struct 
{
    char      name[16];
    uint32_t  interval;
    uint32_t  next_run;
    uint16_t  state;
    uint16_t  prev_state;
    task_cb   cb;
} task_st;


void task_initialize(uint8_t active_tasks);

void task_set_task(uint8_t task_indx, task_st *task_p) ;

void task_run(void);

void task_print_status(void);



#endif
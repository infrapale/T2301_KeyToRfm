#ifndef __KBD_UART_H__
#define __KBD_UART_H__

#define UART_MAX_BLOCK_LEN  8
#include "Arduino.h"


#define  KBD_RX_BUF_LEN   80
#define  KBD_RX_RING_BUFF_LEN   16

typedef struct 
{
    char    module;
    char    key;
    char    value;
} kbd_data_st;

typedef struct
{
    char        buff[KBD_RX_BUF_LEN];
    bool        rx_avail;
    uint8_t     rx_len; 
    kbd_data_st data;
    uint8_t     head;
    uint8_t     tail;
    uint8_t     cntr;
    // char    module;
    // char    key;
    // char    value;
} kbd_uart_st;


/// @brief Clear rx available
/// @param  -
/// @return -
void kbd_uart_initialize(void);

/// @brief  Get pointer to module data
/// @param
/// @return data pointer
//uart_msg_st *uart_get_data_ptr(void);

void kbd_ring_add_key(kbd_data_st *pkey);

bool kbd_ring_get_key(kbd_data_st *pkey);


/// @brief  Read uart
/// @note   Save mesage in uart.rx.str
/// @param  -
/// @return -
bool kbd_uart_read(void);

/// @brief  Parse Rx frame, 
/// @note   check that the frame is valid and addressed to me
/// @param  -
/// @return -
void kbd_uart_parse_rx(void);


void kbd_print_module_key_value(kbd_data_st *pkey);

void kbd_print_mesage(void);
/// @brief  Print rx metadat for debugging
/// @param  -
/// @return -
// void kbd_uart_print_rx_metadata(void);

void run_read_key_commands(void);
void run_send_key_commands(void);


#endif
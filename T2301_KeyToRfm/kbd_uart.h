#ifndef __KBD_UART_H__
#define __KBD_UART_H__

#define UART_MAX_BLOCK_LEN  8
#include "Arduino.h"

#define SerialKbd  Serial1
#define SerialRfm  Serial2


/// @brief Clear rx available
/// @param  -
/// @return -
void kbd_uart_initialize(void);

/// @brief  Get pointer to module data
/// @param
/// @return data pointer
//uart_msg_st *uart_get_data_ptr(void);

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

void kbd_print_mesage(void);
/// @brief  Print rx metadat for debugging
/// @param  -
/// @return -
// void kbd_uart_print_rx_metadata(void);

#endif
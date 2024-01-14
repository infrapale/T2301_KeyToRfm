#include <stdint.h>
#include "main.h"
#include "kbd_uart.h"
//#include "json.h"



kbd_uart_st  kbd_uart;
kbd_data_st kbd_rx_ring[KBD_RX_RING_BUFF_LEN];

// uart_msg_st *kbd_uart_get_data_ptr(void)
// {
//     return &uart;
// }

void kbd_uart_initialize(void)
{
    memset(&kbd_uart,0x00, sizeof(kbd_uart));
    kbd_uart.rx_avail = false;
    SerialKbd.setTimeout(10000);

}

void kbd_ring_add_key(kbd_data_st *pkey)
{
    kbd_rx_ring[kbd_uart.head].module = pkey->module;
    kbd_rx_ring[kbd_uart.head].key = pkey->key;
    kbd_rx_ring[kbd_uart.head].value = pkey->value;
    if (++kbd_uart.head >= KBD_RX_RING_BUFF_LEN)
    {
        kbd_uart.head = 0;
    }
    if (kbd_uart.cntr < KBD_RX_RING_BUFF_LEN) kbd_uart.cntr++;
}


bool kbd_ring_get_key(kbd_data_st *pkey)
{   
    bool res = false;
    if(kbd_uart.cntr > 0 )
    {
        kbd_uart.cntr--;
        pkey->module  = kbd_rx_ring[kbd_uart.tail].module;
        pkey->key     = kbd_rx_ring[kbd_uart.tail].key;
        pkey->value   = kbd_rx_ring[kbd_uart.tail].value;
        if (++kbd_uart.tail >= KBD_RX_RING_BUFF_LEN)
        {
            kbd_uart.tail = 0;
        }
        res = true;
    }
    return res;
}


void kbd_ring_test()
{
    for (uint8_t i = 0; i < 20; i++)
    {

    }
    
}

bool kbd_uart_read(void)
{
    if (SerialKbd.available())
    {
        String  rx_str;
        rx_str = SerialKbd.readStringUntil('\n');
        rx_str.trim();
        //Serial.println(rx_str);
        kbd_uart.rx_len = rx_str.length();
        if (kbd_uart.rx_len > 0)
        {
          rx_str.toCharArray(kbd_uart.buff, KBD_RX_BUF_LEN);
          //Serial.print("rx is available:"); Serial.println(kbd_uart.buff);
          kbd_uart.rx_avail = true;
        }
    }
    else 
    {
        kbd_uart.rx_avail = false;
    }
    return kbd_uart.rx_avail;
}


void kbd_uart_parse_rx(void)
{
    //rfm_send_msg_st *rx_msg = &send_msg; 
    bool do_continue = true;
    uint8_t len;
    //   <KP2:1=1>
    if ((kbd_uart.buff[0] != '<') || 
        (kbd_uart.buff[1] != 'K') || 
        (kbd_uart.buff[2] != 'P') || 
        (kbd_uart.buff[4] != ':') || 
        (kbd_uart.buff[6] != '=') || 
        (kbd_uart.buff[8] != '>') 
      )  do_continue = false;


    if (do_continue)
    {   
        kbd_uart.data.module = kbd_uart.buff[3];
        kbd_uart.data.key    = kbd_uart.buff[5];
        kbd_uart.data.value  = kbd_uart.buff[7];

        if ((kbd_uart.data.module < '1') || (kbd_uart.data.module > '2')) do_continue = false; 
        if ((kbd_uart.data.key < '1') || (kbd_uart.data.key > '8')) do_continue = false;
        if ((kbd_uart.data.value != '0') && (kbd_uart.data.value != '1')) do_continue = false;
    }
    if (do_continue)
    {
        Serial.print("Buffer frame is OK\n");    
        kbd_ring_add_key(&kbd_uart.data);   
    }
    kbd_print_mesage();
}

void kbd_print_module_key_value(kbd_data_st *pkey)
{
    Serial.printf("module: %c key: %c value: %c\n", pkey->module ,pkey->key, pkey->value);
}

void kbd_print_mesage(void)
{
    Serial.printf("\n%s ",kbd_uart.buff);
    kbd_print_module_key_value(&kbd_uart.data);
    //Serial.printf("module: %c key: %c value: %c\n", kbd_uart.data.module ,kbd_uart.data.key, kbd_uart.data.value);
}

// void uart_build_node_from_rx_str(void)
// {
//     uint8_t indx1;
//     uint8_t indx2;
//     indx1 = 0;  //uart.rx.str.indexOf(':')
//     indx2 = uart.rx.str.indexOf(';');
//     uart.node.zone = uart.rx.str.substring(indx1,indx2);
//     indx1 = indx2+1;
//     indx2 = uart.rx.str.indexOf(';',indx1+1);
//     uart.node.name = uart.rx.str.substring(indx1,indx2);
//     indx1 = indx2+1;
//     indx2 = uart.rx.str.indexOf(';',indx1+1);
//     uart.node.value = uart.rx.str.substring(indx1,indx2);
//     indx1 = indx2+1;
//     indx2 = uart.rx.str.indexOf(';',indx1+1);
//     uart.node.remark = uart.rx.str.substring(indx1,indx2);
//     indx1 = indx2+1;
//     indx2 = uart.rx.str.indexOf(';',indx1+1);
    
// }





// void uart_build_node_tx_str(void)
// {
//     rfm_receive_msg_st *receive_p = rfm_receive_get_data_ptr();
//     // <#X1N:RMH1;RKOK1;T;->\n   -> {"Z":"MH1","S":"RKOK1","V":"T","R":"-"}
//     uart.rx.str = (char*) receive_p->radio_msg;  
//     uart.tx.str = "<#X1a:";
//     json_pick_data_from_rx(&uart);
//     #ifdef DEBUG_PRINT
//     Serial.print("radio_msg: ");
//     Serial.println(uart.rx.str);  
//     Serial.println(uart.node.zone);
//     Serial.println(uart.node.name);
//     Serial.println(uart.node.value);
//     Serial.println(uart.node.remark);
//     #endif
//     uart.tx.str += uart.node.zone;
//     uart.tx.str += ';';
//     uart.tx.str += uart.node.name;
//     uart.tx.str += ';';
//     uart.tx.str += uart.node.value;
//     uart.tx.str += ';';
//     uart.tx.str += uart.node.remark;
//     uart.tx.str += '>';
// }

// void uart_build_raw_tx_str(void)
// {
//     rfm_receive_msg_st *receive_p = rfm_receive_get_data_ptr();
//     uart.tx.str = "<#X1r:";
//     uart.tx.str += (char*) receive_p->radio_msg;
//     uart.tx.str += '>';
// }

// void uart_rx_send_rfm_from_raw(void)
// {
//     String payload = uart.rx.str.substring(6,uart.rx.len - 1);
//     payload.toCharArray(uart.rx.radio_msg, MAX_MESSAGE_LEN);
//     rfm_send_radiate_msg(uart.rx.radio_msg);
// }

// void uart_rx_send_rfm_from_node(void)
// {
//     uart.rx.str = uart.rx.str.substring(6,uart.rx.len - 1);
//     uart_build_node_from_rx_str();
//     rfm_send_msg_st *send_p = rfm_send_get_data_ptr();
//     json_convert_uart_node_to_json(send_p->radio_msg, &uart);
//     rfm_send_radiate_msg(send_p->radio_msg);
// }


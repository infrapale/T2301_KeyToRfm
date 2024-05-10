#include <stdint.h>
#include "main.h"
#include "func.h"
#include "menu4x2.h"
#include "relay.h"
#include "kbd_uart.h"
#include "signal.h"
//#include "json.h"
#include "task.h"
#include "sema.h"


extern main_ctrl_st main_ctrl;

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

void run_read_key_commands(void)
{
  if (kbd_uart_read())
  {
      kbd_uart_parse_rx();
  }
  task_clear_cntr(TASK_READ_KEY);
}


void run_send_key_commands(void)
{
  static uint8_t state = 0;
  static uint8_t prev_state = 99;
  static uint32_t next_send_ms = 0;
  static uint8_t  relay_indx = 0;
  static kbd_data_st key_data;
  static key_function_st func_data;

  if (state != prev_state) 
  {
    Serial.printf("State %d -> %d\n", prev_state, state);
    prev_state = state;
  }
  switch(state)
  {
    case 0:   // wait for key to be pressed
      if (kbd_ring_get_key(&key_data))
      {
          Serial.print("Got: ");
          kbd_print_module_key_value(&key_data);

          if (func_get_key(&key_data, &func_data))
          {
            // Serial.printf("type: %d index: %d\n", func_data.type, func_data.indx);

            if ((key_data.module == '1') && menu4x2_key_do_menu(key_data.key))
            {
              Serial.print("Menu command: "); Serial.println(key_data.key);
              menu4x2_key_pressed(key_data.key);
            }
            else
            {
              switch (func_data.type)
              {
                case FUNC_RELAY:
                  state = 10;
                  break;
                case FUNC_RELAY_GROUP:
                  state = 20;
                  break;
                case FUNC_OPTION:
                  //state = 30;
                  break;
                default:
                  Serial.println("Incorect function type"); 
                  break;
              }

            }
          }
          else Serial.println("func_get_key failed");
      }
      break;

    case 10:
      if (sema_reserve(SEMA_SERIAL2)) state++;
      break;  
    case 11:  // single relay function
      signal_set_event(SIGNAL_EVENT_SENDING);
      relay_send_one((va_relays_et)func_data.indx, key_data.value );
      next_send_ms = millis() + RFM_SEND_INTERVAL;
      state++;
      break;
    case 12:
      if (millis() > next_send_ms)
      {
        sema_release(SEMA_SERIAL2);
        state = 0;
        //signal_return_state();
      } 
      break;  
    case 20:  // relay group
      signal_set_event(SIGNAL_EVENT_SENDING);
      relay_indx = 0;
      state++;
      break;  
    case 21:
      while (relay_indx < VA_RELAY_NBR_OF)
      {
        if (relay_get_is_relay_in_group((va_relays_et)relay_indx, func_data.indx )) 
        {
          state++;  // send realy message
          break;
        }  
        else relay_indx++;  // check next relay
      }
      if (relay_indx >= VA_RELAY_NBR_OF) 
      {
        state = 0;
        //signal_return_state();
      }
      break;  
    case 22: 
      if (sema_reserve(SEMA_SERIAL2)) state++;
      break;
    case 23:
      relay_send_one((va_relays_et)relay_indx, key_data.value );
      next_send_ms = millis() + RFM_SEND_INTERVAL;
      relay_indx++;
      state++;
      break;
    case 24:
      if (millis() > next_send_ms) 
      {
        sema_release(SEMA_SERIAL2);
        state = 21;
      }  
      break;  
    case 30:
      if (kbd_ring_get_key(&key_data))
      {
          Serial.print("Option: ");
          if(key_data.module == '1')
          {
            menu4x2_key_pressed(key_data.key);
          }
      }
      break;  
    case 40:
      if (kbd_ring_get_key(&key_data))
      {
        if(key_data.module == '1')
        {
          switch (key_data.key)
          {
            case '5':
              // main_ctrl.status = STATUS_AT_HOME;
              //clock24_set_state(CLOCK_STATE_AT_HOME);
               
              break;
            case '6':
              //main_ctrl.status = STATUS_AWAY;
              //clock24_clear_state(CLOCK_STATE_AT_HOME);
              break;
          }
        }
        // clock24_clear_state(CLOCK_STATE_OPTION);
        state = 0;
      }  
      break;
  }
}


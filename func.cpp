/*******************************************************************************
Key Press to Relay Functions
********************************************************************************
Keypad 1
  --------------------------------------------
  |5:                   |1:                  |
  --------------------------------------------
  |6:                   |2:                  |
  --------------------------------------------
  |7:                   |3:                  |
  --------------------------------------------
  |8: HH:MM             |4: Valitse/Alkkun   |
  --------------------------------------------

Keypad 2
  --------------------------------------------
  |5: KHH+PSH+Sauna     |1: MH2              |
  --------------------------------------------
  |6: Tupa              |2: Eteinen          |
  --------------------------------------------
  |7: Keittio           |3: MH1              |
  --------------------------------------------
  |8: Ranta             |4: Ulko             |
  --------------------------------------------


********************************************************************************
*/

#include "relay.h"
#include "func.h"

key_function_st key_func[NBR_KEY_PADS][NBR_KEYS_PER_PAD] =
{
  {
    {FUNC_RELAY, VA_RELAY_TK},
    {FUNC_RELAY, VA_RELAY_ET},
    {FUNC_RELAY, VA_RELAY_WC_1},
    {FUNC_OPTION,VA_RELAY_UNDEF},
    {FUNC_RELAY, VA_RELAY_MH1_1},
    {FUNC_RELAY, VA_RELAY_KEITTIO_1},
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_ALL},
    {FUNC_RELAY_GROUP,VA_RELAY_GROUP_KHH},
  },
  {
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_MH2},
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_WC},
    {FUNC_RELAY, VA_RELAY_ET},
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_MH1},
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_KHH},
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_TUPA},
    {FUNC_RELAY_GROUP,VA_RELAY_GROUP_K},
    {FUNC_RELAY, VA_RELAY_POLKU},
  }
};

bool func_get_key(kbd_data_st *pkey, key_function_st *pfunc)
{
    uint8_t   module;
    uint8_t   key;
    bool      res = false;

    if  (((pkey->module == '1') || (pkey->module == '2')) && 
        ((pkey->key >= '1') && (pkey->key <= '8')))
    {
        module = pkey->module - '1';
        key    = pkey->key -'1';
        pfunc->type = key_func[module][key].type;
        pfunc->indx  = key_func[module][key].indx;
        res = true;
        Serial.printf("Correct!  type= %d and index= %d\n", pfunc->type, pfunc->indx );
    }
    else
    {
        Serial.printf("Incorrect module %c or key %c\n", pkey->module, pkey->key );
    }
    return res;
}

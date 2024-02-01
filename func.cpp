
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
    {FUNC_RELAY, VA_RELAY_KHH_1},
    {FUNC_RELAY, VA_RELAY_POLKU},
    {FUNC_RELAY, VA_RELAY_ET},
    {FUNC_RELAY, VA_RELAY_ET},
    {FUNC_RELAY, VA_RELAY_SAUNA},
    {FUNC_RELAY, VA_RELAY_POLKU},
    {FUNC_RELAY_GROUP, VA_RELAY_GROUP_TUPA},
    {FUNC_RELAY_GROUP,VA_RELAY_GROUP_KHH},
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

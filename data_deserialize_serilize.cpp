#include "data_deserialize_serilize.h"
#include <stdio.h>
#include <ESP8266WiFi.h>

const char separator_sym = '#';
const char end_sym = '$';
const char null_sym = '_';



uint8_t is_num(char num)
{
	return (num >= '0' && num <= '9') ? 1 : 0;
}



uint8_t ch_to_num(char ch)
{
	char nums[11] = "0123456789";
	uint8_t res = 0;
	for (uint8_t i = 0; i < 10; i++) {
		if (ch == nums[i]) {
			res = i;
		}
	}
	return res;
}



uint8_t get_field_data(const char raw_str[], const char field_name[], char field_data[], uint8_t ind, uint8_t len, uint8_t data_size)
{
	uint8_t res = 1;  //0 ok,  !0 err
	uint8_t i;
	uint8_t x = 0;
	uint8_t index_start = ind;
	//проверка совпадения имени поля
	for (i = index_start; i < len; i++) {
		if (raw_str[i] == '=') {
			i++;
			res = 0;
			break;
		} else if (raw_str[i] != field_name[i-ind] || raw_str[i] == '\0' || field_name[i-ind] == '\0') {
			res = 1;
			break;
		}
	}
	if (res == 0) {
		for (; i < len; i++) {
			
			if (raw_str[i] != separator_sym && raw_str[i] != end_sym && raw_str[i] != '\0') {
				if (x < data_size-1) { 
					field_data[x] = raw_str[i];
					x++;
				} else {
					res = 1;
					break;
				}
				
			} else {
				if (x > 0) {
					field_data[x] = '\0';
					res = 0;
				} else {
					res = 1;
				}
				break;
			}
		}
	}
	return res;
}



uint8_t net_var_data_deserialize(char raw_str[], uint8_t size, net_var_struct *net_var)
{
	uint8_t res; //0 ok,  !0 err
	uint8_t start_index = 0;
	char buff_id[8] = {0};
	uint8_t id_num = 0;
	res = get_field_data(raw_str, "var",  net_var->var_name, start_index, size, size);
	if (res == 1) return res;
	for(; start_index < size; start_index++){
		if(raw_str[start_index] == separator_sym) {
			start_index++;
			break;
		}
	}
	res = get_field_data(raw_str, "val",  net_var->data, start_index, size, NET_VAR_DATA_SIZE);
	if (res == 1) return res;
	if (net_var->data[0] != null_sym) {
		for(; start_index < size; start_index++){
			if(raw_str[start_index] == separator_sym) {
				start_index++;
				break;
			}
		}
		res = get_field_data(raw_str, "id",  buff_id, start_index, size, sizeof(buff_id));
		if (res == 1) return res;
		for(uint8_t i = 0; i < sizeof(buff_id); i++){
      
			if (buff_id[i] != end_sym) {
				id_num *= 10;
				id_num += ch_to_num(buff_id[i]);
			}
		}
		net_var->id = id_num;
	}
	return res;
}



uint8_t net_var_data_serialize(char raw_str[], uint8_t size, net_var_struct *net_var)
{
  uint8_t res = 1;

  if (snprintf(raw_str, size, "var=%s%cval=%s%c", net_var->var_name, separator_sym, net_var->data, end_sym) <= size) {
    res = 0;
  }

  return res;
}

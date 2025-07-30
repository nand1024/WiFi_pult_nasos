#ifndef __DATA_DESERIALIZE_SERIALIZE_H__
#define __DATA_DESERIALIZE_SERIALIZE_H__

#include <stdint.h>

#define NET_VAR_DATA_SIZE 64

typedef struct{
	char *var_name;
	char data[NET_VAR_DATA_SIZE];
	uint16_t id;
}net_var_struct;

uint8_t net_var_data_deserialize(char raw_str[], uint8_t size, net_var_struct *net_var);
uint8_t net_var_data_serialize(char raw_str[], uint8_t size, net_var_struct *net_var);

#endif

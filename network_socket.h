#ifndef __NETWORK_SOCKET_H__
#define __NETWORK_SOCKET_H__

#include <stdint.h>
#include "data_deserialize_serilize.h"

#define MAX_TRY_REQUEST   5

#define SERVER_PORT       7890
#define SERVER_IP         "192.168.1.13"

int16_t send_data(net_var_struct *net_var);

int16_t get_data(net_var_struct *net_var);

#endif

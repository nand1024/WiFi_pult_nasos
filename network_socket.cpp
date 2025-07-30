
#include <stdio.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include "network_socket.h"

int16_t request_server(char req_tx[], char req_rx[], uint8_t size_rx) {
  int16_t res = -1;
  //req_tx нуль терминированая строка запроса
  const uint16_t port = SERVER_PORT;
  const char *serverIP = SERVER_IP;  // IP адреса сервера
  WiFiClient client;
  if (client.connect(serverIP, port)) {
    res = 0;
#ifdef DEBUG_ON
    Serial.println("Connected to server");
#endif
    client.print(req_tx);  // Відправка повідомлення на сервер
    delay(50);
    uint8_t i;
    for (i = 0; i < size_rx && client.available(); i++) {
      req_rx[i] = client.read();
#ifdef DEBUG_ON
      Serial.write(req_rx[i]);  // Виведення відповіді від сервера
#endif
    }
    res = i;
    client.stop();
#ifdef DEBUG_ON
    Serial.println("\nDisconnected from server");
#endif
  } else {
    Serial.println("connect to server fail");
  }
  return res;
}

//встановлює значення мережевої змінної
int16_t send_data(net_var_struct *net_var) {
  int16_t res_func;
  char data_tx[NET_VAR_DATA_SIZE]={0};
  char null_var = 0;
  if (net_var_data_serialize(data_tx, NET_VAR_DATA_SIZE, net_var) == 0) {
    for (uint8_t i = 0; i < MAX_TRY_REQUEST; i++) {
      res_func = request_server(data_tx, &null_var, 0);
      if (res_func >= 0) {
        Serial.println("send ok");
        break;
      } else {
        delay(100);
      }
    }
  } else {
    res_func = -2;
  }
  return res_func;
}

//повертає значення мережевої змінної
int16_t get_data(net_var_struct *net_var) {
  net_var_struct net_var_buffer;

  int16_t res_func = 1;
  char data_tx[NET_VAR_DATA_SIZE]={0};
  char data_rx[NET_VAR_DATA_SIZE]={0};

  //uint8_t len_var_name = strlen(net_var->var_name);
  net_var_buffer.var_name = net_var->var_name;
  net_var_buffer.data[0] = '?';
  net_var_buffer.data[1] = '\0';

  if (net_var_data_serialize(data_tx, NET_VAR_DATA_SIZE, &net_var_buffer) == 0) {
    for (uint8_t i = 0; i < MAX_TRY_REQUEST; i++) {
      res_func = request_server(data_tx, data_rx, NET_VAR_DATA_SIZE);
      if (res_func > 0) {
        Serial.println("recive ok: ");
        break;
      } else {
        delay(100);
      }
    }
    if (res_func > 0) {
      if (net_var_data_deserialize(data_rx, NET_VAR_DATA_SIZE, &net_var_buffer) == 0){
        memcpy(net_var->data, net_var_buffer.data, NET_VAR_DATA_SIZE);
        net_var->id = net_var_buffer.id;
        Serial.print("recieve data ");
        Serial.println(net_var->data);
      } else {
        
        res_func = -2;
        Serial.println("err deserialize");
      }
    }
  } else {
    res_func = -2;
  }
  return res_func;
}

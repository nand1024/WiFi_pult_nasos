#include <stdio.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "data_deserialize_serilize.h"
#include "led_indication.h"
#include "network_socket.h"
#include "button_module.h"


#define MAX_NET_ERR           5



uint16_t net_cmd_flag = 0;
#define NET_STATUS_RELAY_A_ON   0x0001
#define NET_STATUS_RELAY_A_OFF  0x0002
#define NET_ERR                 0x8000

const char* ssid = "logovo";
const char* password = "silent1234567";

Ticker periph_proc;



struct {
  uint16_t wifi_err;
  uint16_t status_rel_a;
  uint16_t status_rel_b;
}net_err_cnt;



struct{
  bool net_cmd_rel_A_ON;
  bool net_status_rel_A_ON;
}cmd_status_struct;



void periph_prc() {
  bool is_click_A = is_button_A_click();//true якщо був клік //увімкнення реле
  bool is_click_B = is_button_B_click();//true якщо був клік //сигнал

  if (is_click_A) {
    cmd_status_struct.net_cmd_rel_A_ON = true;
  }

  if (is_click_B) {
    cmd_status_struct.net_cmd_rel_A_ON = false;
  }

  if (cmd_status_struct.net_cmd_rel_A_ON == true && (net_cmd_flag & NET_STATUS_RELAY_A_ON)) {         //команда на включення відправленна і відпрацьовується
    set_led_rel(IND_RELAY_STATE_CMD_ON);
  } else if (cmd_status_struct.net_cmd_rel_A_ON == true && (net_cmd_flag & NET_STATUS_RELAY_A_OFF)) { //команда на включення відправленна і не відпрацьовується
    set_led_rel(IND_RELAY_STATE_CMD_ON_LOCAL_OFF);
  } else if (cmd_status_struct.net_cmd_rel_A_ON == false && (net_cmd_flag & NET_STATUS_RELAY_A_ON)) { //команди на включення не було і відпрацьовується команда на включення по місцю
    set_led_rel(IND_RELAY_STATE_CMD_OFF_LOCAL_ON);
  } else {                                                                                            //команд на включення не було реле вимкнене
    set_led_rel(IND_RELAY_STATE_CMD_OFF);
  }
}


void set_state_flag_net_cmd (net_var_struct *net_var, uint16_t *net_flag, uint16_t flag_on, uint16_t flag_off, uint16_t *net_err_cnt, uint8_t *server_err) {
  uint8_t prew_id = net_var->id;
  if (get_data(net_var) > 0) {
    Serial.println(net_var->data);
    if (net_var->id == prew_id) {
      if (net_err_cnt != NULL && *net_err_cnt < MAX_NET_ERR) {
        (*net_err_cnt)++;
      }
    } else {
      if (net_err_cnt != NULL) {
        *net_err_cnt = 0;
      }
    }  
    if (net_err_cnt != NULL && *net_err_cnt == MAX_NET_ERR) {
      *net_flag &= (~flag_on);
      *net_flag |= flag_off;
    } else {
      if (net_var->data[0] == 'Y') {//Y on
        *net_flag |= flag_on;
        *net_flag &= (~flag_off);
      } else if (net_var->data[0] == 'N') {//N off
        *net_flag |= flag_off;
        *net_flag &= (~flag_on);
      }
    }
    *server_err = 0;
  } else {
    *net_flag &= (~flag_on);
    *net_flag |= flag_off;
    *server_err = 1;
  }
}

net_var_struct net_var_cmd_relay_a, net_var_status_relay_a;

void setup() {
  net_var_cmd_relay_a.var_name = (char*)"cmd_relay_A";
  net_var_status_relay_a.var_name = (char*)"status_relay_A";
  net_err_cnt.status_rel_a = 0;
  net_err_cnt.wifi_err = 0;
  Serial.begin(115200);
  cmd_status_struct.net_cmd_rel_A_ON = false;
  cmd_status_struct.net_status_rel_A_ON = false;

  button_init();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  periph_proc.attach(0.1, periph_prc);
  led_ind_init();
}



void loop() {
  uint8_t server_error_fl;
  int16_t res_req = 0;
  //char data_tx_buffer[8] = {0};
  if (WiFi.status() == WL_CONNECTED) {
    net_err_cnt.wifi_err = 0;
    Serial.println("WiFi connected");
    net_cmd_flag &= ~NET_ERR;
    set_state_flag_net_cmd(&net_var_status_relay_a, &net_cmd_flag, NET_STATUS_RELAY_A_ON, NET_STATUS_RELAY_A_OFF, &net_err_cnt.status_rel_a, &server_error_fl);
    delay(10);

    snprintf(net_var_cmd_relay_a.data, NET_VAR_DATA_SIZE, "%c", cmd_status_struct.net_cmd_rel_A_ON ? 'Y' : 'N');
    res_req = send_data(&net_var_cmd_relay_a);
    delay(10);
  
    if (server_error_fl == 0 && res_req >= 0) {
      set_led_net_status(NET_STATUS_CONECT);
    } else {
      set_led_net_status(NET_STATUS_SERVER_ERR);
      Serial.println("server error");
    }
  } else {
    if(net_err_cnt.wifi_err < MAX_NET_ERR)net_err_cnt.wifi_err++;
    if (net_err_cnt.wifi_err == MAX_NET_ERR){
      net_cmd_flag |= NET_ERR;
      set_led_net_status(NET_STATUS_DISCONECT);
    }
#ifdef DEBUG_ON
    Serial.println("WiFi disconected");
#endif
  }
  delay(300);
}

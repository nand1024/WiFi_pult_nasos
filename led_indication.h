#ifndef __LED_INDICATION_H__
#define __LED_INDICATION_H__

#define PIN_LED_GREEN_STATUS    D2   //on
#define PIN_LED_RED_STATUS      D5   //off
#define PIN_LED_NET_STATUS      D1


typedef enum {
  IND_RELAY_STATE_CMD_OFF,
  IND_RELAY_STATE_CMD_ON,
  IND_RELAY_STATE_CMD_ON_LOCAL_OFF,
  IND_RELAY_STATE_CMD_OFF_LOCAL_ON,
  IND_RELAY_STATE_CNT,
}led_relay_state_type;



typedef enum {
  NET_STATUS_CONECT,
  NET_STATUS_DISCONECT,
  NET_STATUS_SERVER_ERR,
  NET_STATUS_CNT,
}led_net_status_type;


void set_led_net_status(led_net_status_type val);
void set_led_rel(led_relay_state_type val);
void led_ind_init();

#endif

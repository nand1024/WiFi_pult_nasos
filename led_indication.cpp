#include <stdint.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "led_indication.h"

Ticker led_ind_process;

static led_net_status_type net_status;
static led_relay_state_type led_rel_state;
static uint8_t timer_signal;



void set_led_net_status(led_net_status_type val) {
  if (NET_STATUS_CNT > val) {
    net_status = val;
  }
}



void set_led_rel(led_relay_state_type val) {
  if(IND_RELAY_STATE_CNT > val) {
    led_rel_state = val;
  }
}



void led_ind_prc() {

  static uint8_t led_signal_cnt = 0;
  static uint8_t cnt_led_status_rel_A = 0;
  static uint8_t cnt_led_net_status = 0;

  if(cnt_led_status_rel_A++ >= 3)cnt_led_status_rel_A = 0;
  if(led_signal_cnt++ >= 19)led_signal_cnt = 0;
  if(cnt_led_net_status++ >= 5)cnt_led_net_status = 0;

  switch(led_rel_state) {
    case IND_RELAY_STATE_CMD_OFF:
      digitalWrite(PIN_LED_RED_STATUS, HIGH);
      digitalWrite(PIN_LED_GREEN_STATUS, LOW);
    break;

    case IND_RELAY_STATE_CMD_ON:
      digitalWrite(PIN_LED_GREEN_STATUS, HIGH);
      digitalWrite(PIN_LED_RED_STATUS, LOW);
    break;

    case IND_RELAY_STATE_CMD_ON_LOCAL_OFF:
      digitalWrite(PIN_LED_RED_STATUS, HIGH);
      if (cnt_led_status_rel_A % 4 == 0) {
        digitalWrite(PIN_LED_GREEN_STATUS, HIGH);
      } else {
        digitalWrite(PIN_LED_GREEN_STATUS, LOW);
      }
    break;

    case IND_RELAY_STATE_CMD_OFF_LOCAL_ON:
      digitalWrite(PIN_LED_GREEN_STATUS, HIGH);
      if (cnt_led_status_rel_A % 4 == 0) {
        digitalWrite(PIN_LED_RED_STATUS, HIGH);
      } else {
        digitalWrite(PIN_LED_RED_STATUS, LOW);
      }
    break;

    default:
    break;
  }

  switch(net_status) {
    case NET_STATUS_CONECT:
      if (cnt_led_net_status % 6 == 0) {
        digitalWrite(PIN_LED_NET_STATUS, HIGH);
      } else {
        digitalWrite(PIN_LED_NET_STATUS, LOW);
      }
    break;

    case NET_STATUS_DISCONECT:
      digitalWrite(PIN_LED_NET_STATUS, HIGH);
    break;

    case NET_STATUS_SERVER_ERR:
      if (cnt_led_net_status % 2 == 0) {
        digitalWrite(PIN_LED_NET_STATUS, HIGH);
      } else {
        digitalWrite(PIN_LED_NET_STATUS, LOW);
      }
    break;

    default:
    break;
  }
}



void led_ind_init() {

  net_status = NET_STATUS_DISCONECT;
  led_rel_state = IND_RELAY_STATE_CMD_OFF;
  timer_signal = 0;

  pinMode(PIN_LED_GREEN_STATUS, OUTPUT);
  pinMode(PIN_LED_RED_STATUS, OUTPUT);
  pinMode(PIN_LED_NET_STATUS, OUTPUT);

  digitalWrite(PIN_LED_GREEN_STATUS, LOW);
  digitalWrite(PIN_LED_RED_STATUS, LOW);
  digitalWrite(PIN_LED_NET_STATUS, LOW);

  led_ind_process.attach(0.3, led_ind_prc);
}

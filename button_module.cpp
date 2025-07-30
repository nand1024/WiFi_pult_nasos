
#include <ESP8266WiFi.h>
#include "button_module.h"

typedef enum {
  BTN_DOWN,
  BTN_UP,
  BTN_CNT
} btn_state;

bool is_button_A_click() {
  bool res = false;
  static btn_state in_state = BTN_UP;
  if (in_state == BTN_UP && digitalRead(PIN_DIG_IN_1) == LOW) {
    in_state = BTN_DOWN;
    res = true;
  } else if (in_state == BTN_DOWN && digitalRead(PIN_DIG_IN_1) == HIGH) {
    in_state = BTN_UP;
  }
  return res;
}

bool is_button_B_click() {
  bool res = false;
  static btn_state in_state = BTN_UP;
  if (in_state == BTN_UP && digitalRead(PIN_DIG_IN_2) == LOW) {
    in_state = BTN_DOWN;
    res = true;
  } else if (in_state == BTN_DOWN && digitalRead(PIN_DIG_IN_2) == HIGH) {
    in_state = BTN_UP;
  }
  return res;
}

void button_init()
{
  pinMode(PIN_DIG_IN_1, INPUT);
  pinMode(PIN_DIG_IN_2, INPUT);
}

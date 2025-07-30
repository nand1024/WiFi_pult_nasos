#ifndef __BUTTON_MODULE_H__
#define __BUTTON_MODULE_H__

#define PIN_DIG_IN_1 D6           //pump run
#define PIN_DIG_IN_2 D7           //pump stop

bool is_button_A_click();
bool is_button_B_click();
void button_init();

#endif

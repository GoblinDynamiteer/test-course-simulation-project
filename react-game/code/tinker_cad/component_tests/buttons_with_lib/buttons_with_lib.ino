/*  Project simulation
    TinkerCad button test code

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-06

    Testing switch buttons with Button library
    One button turns onboard led on, other off
 */

#define PIN_BTN_LED_ON 1
#define PIN_BTN_LED_OFF 2
#define PIN_ONBOARD_LED 13

#include <Button.h>

/* Button object */
Button btn_led_on(PIN_BTN_LED_ON);
Button btn_led_off(PIN_BTN_LED_OFF);

void setup(void)
{
    btn_led_on.begin();
    btn_led_off.begin();
    pinMode(PIN_ONBOARD_LED, OUTPUT);
}

void loop(void)
{
    static bool led_state = false;
    static bool led_state_last = false;

    if(btn_led_on.pressed())
    {
        led_state = true;
    }

    if(btn_led_off.pressed())
    {
        led_state = false;
    }

    if(led_state != led_state_last)
    {
        digitalWrite(PIN_ONBOARD_LED, led_state);
        led_state_last = led_state;
    }

    delay(1);
}

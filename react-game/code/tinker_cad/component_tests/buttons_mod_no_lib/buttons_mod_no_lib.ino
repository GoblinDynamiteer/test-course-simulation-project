/*  Project simulation
    TinkerCad button test code

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-06

    Testing switch buttons without Button library
    One button turns onboard led on, other off
 */

#define PIN_BTN_LED_ON 1
#define PIN_BTN_LED_OFF 2
#define PIN_ONBOARD_LED 13
#define BUTTON_PRESSED 0

void setup(void)
{
    pinMode(PIN_BTN_LED_ON, INPUT_PULLUP);
    pinMode(PIN_BTN_LED_OFF, INPUT_PULLUP);
    pinMode(PIN_ONBOARD_LED, OUTPUT);
}

void loop(void)
{
    static bool led_state = false;
    static bool led_state_last = false;

    if(digitalRead(PIN_BTN_LED_ON) == BUTTON_PRESSED)
    {
        led_state = true;
    }

    if(digitalRead(PIN_BTN_LED_OFF) == BUTTON_PRESSED)
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

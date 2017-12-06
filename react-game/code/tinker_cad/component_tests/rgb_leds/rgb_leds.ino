/*  Project simulation
    TinkerCad button test code

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-06

    Testing RGB-leds & resistor

    Connect common gnd w/ resistor for RGB LED 1
    Connect common gnd w/o resistor for RGB LED 2
        ( Should break at high pwm)
 */

const byte pins_rgbled_1[] = { 3,  5, 6 };
const byte pins_rgbled_2[] = { 9, 10, 11 };

enum{ RED, BLUE, GREEN, COLORS_MAX };

#define PWM_MAX 255

void setup(void)
{
    for(int i = 0; i < 3; i++)
    {
        pinMode(pins_rgbled_1[i], OUTPUT);
        pinMode(pins_rgbled_2[i], OUTPUT);
    }
}

void loop(void)
{
    for(int color = RED; color < COLORS_MAX; color++)
    {
        for(int pwm = PWM_MAX; pwm >= 0; pwm--)
        {
            analogWrite(pins_rgbled_1[color], pwm);
            analogWrite(pins_rgbled_2[color], pwm);
            delay(30);
        }
    }
}

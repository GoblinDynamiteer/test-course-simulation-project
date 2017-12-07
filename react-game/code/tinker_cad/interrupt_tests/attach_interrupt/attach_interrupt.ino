#define INT_PIN 3
#define BUTTON_PRESSED 0
#define ERROR -1

enum { RED, GREEN, BLUE, COLOR_MAX };
const int rgb_led_pin[] = { 10, 11, 12 };
bool button_pressed;

#define CYCLE_COLOR(c) (c + 1 == COLOR_MAX ? RED : c + 1)

void setup()
{
    for(int i = 0; i < 3; i++)
    {
        pinMode(rgb_led_pin[i], OUTPUT);
    }

    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(
        digitalPinToInterrupt(INT_PIN), isr_button_click, FALLING);

    button_pressed = false;
    Serial.begin(9600);
}

void loop()
{
    int color = RED;

    if(button_pressed)
    {
        Serial.println("Setting color: " + String(color));
        set_rgb_led(color);
        color = CYCLE_COLOR(color);
        button_pressed = false;
    }

    delay(1);
}

void isr_button_click(void)
{
    button_pressed = true;
}

void set_rgb_led(int color)
{
    for(int i = 0; i < 3; i++)
    {
        digitalWrite(rgb_led_pin[i], LOW);
    }

    digitalWrite(rgb_led_pin[color], HIGH);
}

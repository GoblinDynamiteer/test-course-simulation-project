#define INT_PIN 3
#define BUTTON_STATUS_DOWN 0
#define ERROR -1

enum { RED, GREEN, BLUE, COLOR_MAX, BTN_MAX = COLOR_MAX };
const int rgb_led_pin[] = { 10, 11, 12 };
const int rgb_btn_pin[] = { 4, 5, 6 };

bool button_pressed;

void setup()
{
    for(int i = 0; i < BTN_MAX; i++)
    {
        pinMode(rgb_led_pin[i], OUTPUT);
        pinMode(rgb_btn_pin[i], INPUT_PULLUP);
    }

    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(
        digitalPinToInterrupt(INT_PIN), isr_button_click, FALLING);

    button_pressed = false;
    Serial.begin(9600);
}

void loop()
{
    if(button_pressed)
    {
        int color = get_pressed_button();
        Serial.println("Setting color: " + String(color));
        set_rgb_led(color);
        button_pressed = false;
    }

    delay(1);
}

void isr_button_click(void)
{
    Serial.println("Int!");
    button_pressed = true;
}

int get_pressed_button(void)
{
    for(int i = 0; i < BTN_MAX; i++)
    {
        if(digitalRead(rgb_btn_pin[i]) == BUTTON_STATUS_DOWN)
        {
            Serial.println("Btn press: " + String(i));
            return i;
        }
    }

    return ERROR;
}

void set_rgb_led(int color)
{
    for(int i = 0; i < COLOR_MAX; i++)
    {
        digitalWrite(rgb_led_pin[i], LOW);
    }

    digitalWrite(rgb_led_pin[color], HIGH);
}

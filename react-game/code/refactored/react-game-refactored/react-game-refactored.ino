/*  Project simulation
TinkerCad

Mjukvaruutveckare Inbyggda System
Johan Kampe
2017-12-09

Reaction game refactored.
*/

#include <Arduino.h>
#include <LiquidCrystal.h>
//#include <Player.h>

enum{ RED, GREEN, BLUE, MAX_COLORS };
enum{ BTN_PL1_RED, BTN_PL1_GREEN, BTN_PL1_BLUE,
      BTN_PL2_RED, BTN_PL2_GREEN, BTN_PL2_BLUE,
      MAX_BTN
   };

enum{ PLAYER_1, PLAYER_2 };

enum
{
    LCD_RS = A0,
    LCD_EN = A1,
    LCD_D4 = A2,
    LCD_D5 = A3,
    LCD_D6 = A4,
    LCD_D7 = A5
};

enum
{
    GAME_MODE_SET_NAMES,
    GAME_MODE_IDLE,
    GAME_MODE_COUNTDOWN,
    GAME_MODE_PLAY,
    GAME_MODE_TEST
};

#define INT_PIN 3
#define NO_BUTTON_PRESSED -1
#define BTN_TO_COLOR(b) (b >= MAX_COLORS ? b - 3 : b)

const int rgb_led_pins[] = { 11, 13, 12 };
const int btn_pins[] = { 5, 6, 7, 8, 9, 10 };
bool button_pressed;
int last_button_pressed;

bool (*game_mode[5])(void);

LiquidCrystal lcd(
    LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

//Player player[2];

/* Function prototypes */
bool game_mode_set_names(void);
bool game_mode_idle(void);
bool game_mode_countdown(void);
bool game_mode_play(void);
bool game_mode_test(void);
void rgb_led_off(void);
void rgb_led_set_color(int);
void isr_button_click(void);
void lcd_print_string(String, int, int, bool);
int button_get_which_pressed(void);

void setup()
{
    randomSeed(analogRead(0));

    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(
        digitalPinToInterrupt(INT_PIN), isr_button_click, FALLING);

    game_mode[GAME_MODE_SET_NAMES] =    game_mode_set_names;
    game_mode[GAME_MODE_IDLE] =         game_mode_idle;
    game_mode[GAME_MODE_COUNTDOWN] =    game_mode_countdown;
    game_mode[GAME_MODE_PLAY] =         game_mode_play;
    game_mode[GAME_MODE_TEST] =         game_mode_test;

    for(int i = 0; i < MAX_BTN; i++)
    {
        if(i < MAX_COLORS)
        {
            pinMode(rgb_led_pins[i], OUTPUT);
        }

        pinMode(btn_pins[i], INPUT_PULLUP);
    }

    lcd.begin(16, 2);
    rgb_led_off();
    button_pressed = false;

    //game_mode[GAME_MODE_SET_NAMES];
}

void loop()
{
    /* Run game mode with with pointer */
    game_mode[GAME_MODE_TEST]();

    delay(1);
}

bool game_mode_test(void)
{
    const String button_names[] =
    {
        "P1 RED", "P1 GREEN", "P1 BLUE",
        "P2 RED", "P2 GREEN", "P2 BLUE"
    };

    if(button_pressed)
    {
        if(last_button_pressed != NO_BUTTON_PRESSED)
        {
            lcd_print_string(
                "Button down: " + String(last_button_pressed), 0, 0, true);
            lcd_print_string(button_names[last_button_pressed], 0, 1, false);
            rgb_led_set_color(BTN_TO_COLOR(last_button_pressed));
        }

        button_pressed = false;
    }

}

/* ISR triggers on any button press */
void isr_button_click(void)
{
    if(!button_pressed)
    {
        last_button_pressed = button_get_which_pressed();
        button_pressed = true;
    }

}

bool game_mode_set_names(void)
{
    return false;
}

bool game_mode_idle(void)
{
    return false;
}

bool game_mode_countdown(void)
{
    return false;
}

bool game_mode_play(void)
{
    return false;
}

int button_get_status(int button)
{
    return !digitalRead(btn_pins[button]);
}

int button_get_which_pressed(void)
{
    for(int i = 0; i < MAX_BTN; i++)
    {
        if(button_get_status(i))
        {
            return i;
        }
    }

    return NO_BUTTON_PRESSED;
}

/* Set RGB-LED color off */
void rgb_led_off(void)
{
    analogWrite(rgb_led_pins[BLUE], 0);
    analogWrite(rgb_led_pins[RED], 0);
    analogWrite(rgb_led_pins[GREEN], 0);
}

/* Set RGB-LED color */
void rgb_led_set_color(int c)
{
    rgb_led_off();
    analogWrite(rgb_led_pins[c], 255);
}

int get_random_color(void)
{
    return random(RED, MAX_COLORS);
}

/* Print string to LCD */
void lcd_print_string(String s, int col, int row, bool clear)
{
    if(clear)
    {
        lcd.clear();
    }

    lcd.setCursor(col, row);
    lcd.print(s);
}

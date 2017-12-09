/*  Project simulation
TinkerCad

Mjukvaruutveckare Inbyggda System
Johan Kampe
2017-12-09

Reaction game refactored.
*/

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Player.h>

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

LiquidCrystal lcd(
    LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

enum
{
    GAME_MODE_SET_NAMES,
    GAME_MODE_IDLE,
    GAME_MODE_COUNTDOWN,
    GAME_MODE_PLAY
};

const int rgb_led_pins[] = { 9, 10, 11 };
const int btn_pins[] = { 2, 3, 4, 5, 6, 7 };

bool (*game_mode[4])(void);

Player player[2];

void rgb_led_off(void);
bool game_mode_set_names(void);
bool game_mode_idle(void);
bool game_mode_countdown(void);
bool game_mode_play(void);

void setup()
{
    randomSeed(analogRead(0));

    game_mode[GAME_MODE_SET_NAMES] =    game_mode_set_names;
    game_mode[GAME_MODE_IDLE] =         game_mode_idle;
    game_mode[GAME_MODE_COUNTDOWN] =    game_mode_countdown;
    game_mode[GAME_MODE_PLAY] =         game_mode_play;

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
}

void loop()
{
    /* Run game mode with with pointer */

    delay(1000);
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

/*  Project simulation
TinkerCad

Mjukvaruutveckare Inbyggda System
Johan Kampe
2017-12-10

Reaction game refactored.
*/

#include <Arduino.h>
#include <LiquidCrystal.h>

enum{ RED, GREEN, BLUE, MAX_COLORS };
enum{ BTN_PL1_RED, BTN_PL1_GREEN, BTN_PL1_BLUE,
      BTN_PL2_RED, BTN_PL2_GREEN, BTN_PL2_BLUE,
      MAX_BTN
   };

enum{ PLAYER_1, PLAYER_2, MAX_PLAYERS };

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
#define BTN_TO_PLAYER(b) (b > 2 ? PLAYER_2 : PLAYER_1 )
#define INC_ONE_MAX(v, m) (v + 1 > m ? m : v + 1)
#define UP true
#define DOWN false

const int rgb_led_pins[] = { 11, 13, 12 };
const int btn_pins[] = { 5, 6, 7, 8, 9, 10 };
bool button_pressed;
int last_button_pressed;

int player_score[MAX_PLAYERS];
String player_name[MAX_PLAYERS];

LiquidCrystal lcd(
    LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/* Function prototypes */
void game_mode_set_names(void);
bool game_mode_idle(void);
bool game_mode_countdown(void);
bool game_mode_play(void);
bool game_mode_test(void);
void rgb_led_off(void);
void rgb_led_set_color(int);
void isr_button_click(void);
void lcd_print_string(String, int, int, bool);
int button_get_which_pressed(void);
void player_name_cycle_char(int, bool, int);

void setup()
{
    randomSeed(analogRead(0));

    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(
        digitalPinToInterrupt(INT_PIN), isr_button_click, FALLING);

    for(int i = 0; i < MAX_BTN; i++)
    {
        if(i < MAX_COLORS)
        {
            pinMode(rgb_led_pins[i], OUTPUT);
        }

        pinMode(btn_pins[i], INPUT_PULLUP);
    }

    player_name[PLAYER_1].reserve(2);
    player_name[PLAYER_2].reserve(2);
    player_score[PLAYER_1] = 0;
    player_score[PLAYER_2] = 0;

    lcd.begin(16, 2);
    rgb_led_off();
    button_pressed = false;

    Serial.begin(9600);
    game_mode_set_names();
}

void loop()
{
    while(!game_mode_idle());

    game_mode_countdown();

    game_mode_test();
    delay(1);
}

bool game_mode_test(void)
{
    const String button_names[] =
    {
        "P1 RED", "P1 GREEN", "P1 BLUE",
        "P2 RED", "P2 GREEN", "P2 BLUE"
    };

    while(true)
    {
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

/* Set player names (2 char initials).  */
void game_mode_set_names(void)
{
    player_name[PLAYER_1] = player_name[PLAYER_2] = "AA";
    int char_index[MAX_PLAYERS] = { 0, 0 };
    bool done = false;

    lcd_print_string("SET NAME P1: " + player_name[PLAYER_1], 0, 0, true);
    lcd_print_string("SET NAME P2: " + player_name[PLAYER_2], 0, 1, false);

    while(!done)
    {
        if(button_pressed)
        {
            int p = BTN_TO_PLAYER(last_button_pressed);

            switch(BTN_TO_COLOR(last_button_pressed))
            {
                case RED:
                    player_name_cycle_char(p, DOWN, char_index[p]);
                    break;

                case GREEN:
                    char_index[p] = INC_ONE_MAX(char_index[p], 2);
                    break;

                case BLUE:
                    player_name_cycle_char(p, UP, char_index[p]);
                    break;

                default:
                    break;
            }

            lcd_print_string(
                "SET NAME P1: " + player_name[PLAYER_1], 0, 0, true);
            lcd_print_string(
                "SET NAME P2: " + player_name[PLAYER_2], 0, 1, false);

            button_pressed = false;
            done = (char_index[PLAYER_1] == 2 && char_index[PLAYER_2] == 2);
        }
    }
}

/* Idle mode, display score and wait for any button press */
bool game_mode_idle(void)
{
    static bool dots = false;
    lcd_print_string(
        player_name[PLAYER_1] + ": " + player_score[PLAYER_1] + " | " +
        player_name[PLAYER_2] + ": " + player_score[PLAYER_2], 0, 0, true);

    lcd_print_string(dots ? "*PRESS ANY KEY!*" : " PRESS ANY KEY! ",
        0, 1, false);

    dots = !dots;

    if(button_pressed)
    {
        button_pressed = false;
        return true;
    }

    delay(500);

    return false;
}

/* Countdown */
bool game_mode_countdown(void)
{
    return false;
}

bool game_mode_play(void)
{
    return false;
}

/* Cycle a players initial, A-Z */
void player_name_cycle_char(int player, bool direction, int index)
{
    char c = player_name[player][index];

    if(direction == UP)
    {
        c = c + 1 > 'Z' ? 'A' : c + 1;
    }

    else
    {
        c = c - 1 < 'A' ? 'Z' : c - 1;
    }

    player_name[player][index] = c;
}

/* Get a buttons status */
int button_get_status(int button)
{
    return !digitalRead(btn_pins[button]);
}

/* Gets first found pressed button */
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

/* Get a random color, RED, GREEN or BLUE */
int get_random_color(void)
{
    return random(RED, MAX_COLORS);
}


void lcd_print_string(String s, int col, int row, bool clear)
{
    if(clear)
    {
        lcd.clear();
    }

    lcd.setCursor(col, row);
    lcd.print(s);
}

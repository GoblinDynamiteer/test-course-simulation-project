/*  PROJECT: SIMULATION REACT-GAME

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-10

    Two player reaction game
    Code for TinkerCad Simulation
*/

#include <Arduino.h>
#include <LiquidCrystal.h>

/* Color IDs */
enum{ RED, GREEN, BLUE, MAX_COLORS };

/* Button IDs */
enum{ BTN_PL1_RED, BTN_PL1_GREEN, BTN_PL1_BLUE,
      BTN_PL2_RED, BTN_PL2_GREEN, BTN_PL2_BLUE,
      MAX_BUTTONS };

/* Player IDs */
enum{ PLAYER_1, PLAYER_2, MAX_PLAYERS };

/* Pins for LCD hookup */
enum
{
    LCD_RS = A0,
    LCD_EN = A1,
    LCD_D4 = A2,
    LCD_D5 = A3,
    LCD_D6 = A4,
    LCD_D7 = A5
};

#define INT_PIN             3           // Iterrupt pin
#define NO_BUTTON_PRESSED   -1          // Used when checking all buttons
#define UP                  true        // For cycling player name letters
#define DOWN                false       // For cycling player name letters
#define LCD_COLS            16          // LCD column size
#define LCD_ROWS            2           // LCD row size
#define COUNTDOWN_NUMBER    3           // Countdown for game

/* Determine button color */
#define BTN_TO_COLOR(b)     (b >= MAX_COLORS ? b - 3 : b)
/* Determine which player the button belongs to */
#define BTN_TO_PLAYER(b)    (b > 2 ? PLAYER_2 : PLAYER_1 )
/* Increase value (v) by 1, stop at max (m) */
#define INC_ONE_MAX(v, m)   (v + 1 > m ? m : v + 1)
/* Calculate value for placing a string with lenght s_len on LCD middle */
#define LCD_MIDDLE(s_len)   (LCD_COLS / 2 - s_len / 2)

/* Pins */
const int rgb_led_pins[] =  { 11, 13, 12 };
const int btn_pins[] =      { 5, 6, 7, 8, 9, 10 };

/* Button variables */
bool button_pressed;
int last_button_pressed;

/* Player variables */
int player_score[MAX_PLAYERS];
String player_name[MAX_PLAYERS];

/* LCD Object */
LiquidCrystal lcd(
    LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/* Function prototypes */
void game_mode_set_names(void);
bool game_mode_idle(void);
void game_mode_countdown(void);
int  game_mode_play(void);
void rgb_led_off(void);
void rgb_led_set_color(int);
int  rgb_get_random_color(void);
void isr_button_click(void);
void lcd_print_string(String, int, int, bool);
int  button_get_which_pressed(void);
void player_name_cycle_char(int, bool, int);

/**
 * Program setup
 */
void setup()
{
    randomSeed(analogRead(0));

    /* Set pin modes for interrupt pin, buttons and rgb-led */
    pinMode(INT_PIN, INPUT_PULLUP);

    for(int i = 0; i < MAX_BUTTONS; i++)
    {
        if(i < MAX_COLORS)
        {
            pinMode(rgb_led_pins[i], OUTPUT);
        }

        pinMode(btn_pins[i], INPUT_PULLUP);
    }

    /* Set ISR for buttons */
    attachInterrupt(
        digitalPinToInterrupt(INT_PIN), isr_button_click, FALLING);

    /* Players init */
    player_name[PLAYER_1].reserve(2);
    player_name[PLAYER_2].reserve(2);
    player_score[PLAYER_1] = 0;
    player_score[PLAYER_2] = 0;

    lcd.begin(LCD_COLS, LCD_ROWS);

    rgb_led_off();
    button_pressed = false;

    /* Game starts by setting player names (once) */
    game_mode_set_names();
}

/**
 * Program main loop
 */
void loop()
{
    while(!game_mode_idle()); // Until any button press

    game_mode_countdown();

    int winner = game_mode_play();

    lcd_print_string("WINNER: ", 0, 0, true);
    lcd_print_string(player_name[winner] + "!", 0, 1, false);

    player_score[winner]++;

    delay(2000);
    rgb_led_off();
    button_pressed = false;
}

/**
 * ISR, triggers on any button press
 */
void isr_button_click(void)
{
    if(!button_pressed)
    {
        /* Determine what button was pressed */
        last_button_pressed = button_get_which_pressed();
        button_pressed = true;
    }
}

/**
 * Set player names (2 letter initials)
 *      Red button:     Prev letter (Eg CA -> BA)
 *      Blue button:    Next letter (Eg AA -> BA)
 *      Green button:   Confirm
 *
 * Mode ends when both players have set both their initials
 */
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

/**
 * Idle mode, display score on LCD and wait for any button press
 * @return  true : Any button pressed, false: No button pressed
 */
bool game_mode_idle(void)
{
    static bool dots = false;

    lcd_print_string(
        player_name[PLAYER_1] + ": " + player_score[PLAYER_1] + " | " +
        player_name[PLAYER_2] + ": " + player_score[PLAYER_2], 1, 0, true);

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

/**
 * Display message and countdown from COUNTDOWN_NUMBER on LCD
 */
void game_mode_countdown(void)
{
    static const String message = "GET READY!";
    int count = COUNTDOWN_NUMBER;

    lcd_print_string(message, LCD_MIDDLE(message.length()), 0, true);
    delay(1000);

    while(count)
    {
        lcd_print_string(String(count--), LCD_COLS/2, 1, false);
        delay(1000);
    }

    button_pressed = false;
}

/**
 * Waits for any player to press a button, then determines winner by checking
 *      if pressed button color matches the RGB-LED color.
 * @return  Winner, PLAYER_1 or PLAYER_2
 */
int game_mode_play(void)
{
    static const String color_name[MAX_COLORS] =
    {
        "RED", "GREEN", "BLUE"
    };

    int c = rgb_get_random_color();
    int winner;

    rgb_led_set_color(c);

    lcd_print_string(
        color_name[c] + "!", LCD_MIDDLE(color_name[c].length()), 0, true);
    lcd_print_string(
        color_name[c] + "!", LCD_MIDDLE(color_name[c].length()), 1, false);

    while(!button_pressed);

    int player_to_press_first = BTN_TO_PLAYER(last_button_pressed);
    int button_color_pressed =  BTN_TO_COLOR(last_button_pressed);

    return (button_color_pressed == c ?
        player_to_press_first : !player_to_press_first);
}

/**
 * Cycle letter in player name (initials)
 * @param player    PLAYER_1 or PLAYER_2
 * @param direction UP or DOWN (UP: A->B, DOWN: C->B)
 * @param index     Letter to change, 0 or 1
 */
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

/**
 * Get status of a button
 * @param  button Button index, eg BTN_PL1_RED or BTN_PL2_BLUE
 * @return        Button status: 1 (pressed) 0 (not pressed)
 */
int button_get_status(int button)
{
    return !digitalRead(btn_pins[button]);
}

/**
 * Gets first detected pressed button
 * @return  Button index, eg BTN_PL1_RED or BTN_PL2_BLUE
 */
int button_get_which_pressed(void)
{
    for(int i = 0; i < MAX_BUTTONS; i++)
    {
        if(button_get_status(i))
        {
            return i;
        }
    }

    return NO_BUTTON_PRESSED;
}

/**
 * Set RGB-LED color off
 */
void rgb_led_off(void)
{
    analogWrite(rgb_led_pins[BLUE], 0);
    analogWrite(rgb_led_pins[RED], 0);
    analogWrite(rgb_led_pins[GREEN], 0);
}

/**
 * Set RGB-LED color
 * @param c Color (RED, GREEN or BLUE)
 */
void rgb_led_set_color(int c)
{
    rgb_led_off();
    analogWrite(rgb_led_pins[c], 255);
}

/**
 * Get a random color, RED, GREEN or BLUE
 * @return  Color
 */
int rgb_get_random_color(void)
{
    return random(RED, MAX_COLORS);
}

/**
 * Print a string variable to LCD
 * @param s     String to print
 * @param col   Start column
 * @param row   Start row
 * @param clear Clear before print?
 */
void lcd_print_string(String s, int col, int row, bool clear)
{
    if(clear)
    {
        lcd.clear();
    }

    lcd.setCursor(col, row);
    lcd.print(s);
}

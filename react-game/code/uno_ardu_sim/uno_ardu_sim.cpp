/*  PROJECT: SIMULATION REACT-GAME

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-10

    Two player reaction game
    Code for UnoArduSim Simulation

    Modified from TinkerCad simulation code
*/

/* Color IDs */
enum{ RED, GREEN, BLUE, MAX_COLORS };

/* Button IDs */
enum{ BTN_PL1_RED, BTN_PL1_GREEN, BTN_PL1_BLUE,
      BTN_PL2_RED, BTN_PL2_GREEN, BTN_PL2_BLUE,
      MAX_BUTTONS };

/* Player IDs */
enum{ PLAYER_1, PLAYER_2, MAX_PLAYERS };

#define INT_PIN             3           // Iterrupt pin
#define NO_BUTTON_PRESSED   -1          // Used when checking all buttons
#define UP                  true        // For cycling player name letters
#define DOWN                false       // For cycling player name letters

/* Rewrite function like macros to functions: */
/* Determine button color */
int BTN_TO_COLOR(int b)
{
    if(b > 2)
    {
        return b - 3;
    }

    return b;
}
/* Determine which player the button belongs to */
int BTN_TO_PLAYER(int b)
{
    return (b > 2 ? PLAYER_2 : PLAYER_1);
}
/* Increase value (v) by 1, stop at max (m) */
int INC_ONE_MAX(int v, int m)
{
    return (v + 1 > m ? m : v + 1);
}
/* Calculate value for placing a string with lenght s_len on LCD middle */
#define LCD_MIDDLE(s_len)   (LCD_COLS / 2 - s_len / 2)

/* Pins */
const int rgb_led_pins[] =  { 11, 13, 12 };
const int btn_pins[] =      { 5, 6, 7, 8, 9, 10 };

/* Button variables */
bool button_pressed;
int last_button_pressed;

/* Player variables */
int player_score[2];
String player_name[2];

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

    /* Set ISR for buttons -- Not supported in UnoArduSim */
    //attachInterrupt(
    //    digitalPinToInterrupt(INT_PIN), isr_button_click, FALLING);

    /* Players init */
    player_name[PLAYER_1].reserve(2);
    player_name[PLAYER_2].reserve(2);
    player_score[PLAYER_1] = 0;
    player_score[PLAYER_2] = 0;

    Serial.begin(9600);

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
    while(!game_mode_idle())
    {
        // Empty
    }

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

        if(last_button_pressed != NO_BUTTON_PRESSED)
        {
            button_pressed = true;
        }
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
    player_name[PLAYER_1] = "JK";
    player_name[PLAYER_2] = "TB";
}

/**
 * Idle mode, display score on LCD and wait for any button press
 * @return  true : Any button pressed, false: No button pressed
 */
bool game_mode_idle(void)
{
    lcd_print_string(" PRESS ANY KEY! ", 0, 1, false);
    lcd_print_string(player_name[PLAYER_1], 1, 0, true);
    lcd_print_string(String(player_score[PLAYER_1]), 1, 0, true);
    lcd_print_string(player_name[PLAYER_2], 1, 0, true);
    lcd_print_string(String(player_score[PLAYER_2]), 1, 0, true);

    while(!button_pressed)
    {
        isr_button_click();
    }

    if(button_pressed)
    {
        button_pressed = false;
        return true;
    }

    return false;
}

/**
 * Display message and countdown from 3 on LCD
 */
void game_mode_countdown(void)
{
    String message = "GET READY!";
    int count = 3;

    lcd_print_string(message, 0, 0, true);
    delay(1000);

    while(count)
    {
        lcd_print_string(String(count--), 0, 1, false);
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
    String color_name[3] =
    {
        "RED", "GREEN", "BLUE"
    };

    int c = rgb_get_random_color();
    int winner;

    rgb_led_set_color(c);

    lcd_print_string(color_name[c], 0, 0, true);

    while(!button_pressed)
    {
        isr_button_click();
        delay(100);
    }

    int player_to_press_first = BTN_TO_PLAYER(last_button_pressed);
    int button_color_pressed =  BTN_TO_COLOR(last_button_pressed);

    if(button_color_pressed == c)
    {
        return player_to_press_first;
    }

    return !player_to_press_first;
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
    String * s = &player_name[player];

    if(direction == UP)
    {
        c = c + 1 > 'Z' ? 'A' : c + 1;
    }

    else
    {
        c = c - 1 < 'A' ? 'Z' : c - 1;
    }

    s[index] = c;
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
    digitalWrite(rgb_led_pins[BLUE], 0);
    digitalWrite(rgb_led_pins[RED], 0);
    digitalWrite(rgb_led_pins[GREEN], 0);
}

/**
 * Set RGB-LED color
 * @param c Color (RED, GREEN or BLUE)
 */
void rgb_led_set_color(int c)
{
    rgb_led_off();
    digitalWrite(rgb_led_pins[c], HIGH);
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
 * **MOCK** Print a string variable to LCD
 * @param s     String to print
 * @param col   Start column
 * @param row   Start row
 * @param clear Clear before print?
 */
void lcd_print_string(String s, int col, int row, bool clear)
{
    Serial.println(s);
}

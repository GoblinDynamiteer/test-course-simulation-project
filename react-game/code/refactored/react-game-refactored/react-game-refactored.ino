/*  Project simulation
TinkerCad

Mjukvaruutveckare Inbyggda System
Johan Kampe
2017-12-07

Reaction game refactored.
*/

#include <U8glib.h>

enum{ RED, GREEN, BLUE, MAX_COLORS };
enum{ BTN_PL1_RED, BTN_PL1_GREEN, BTN_PL1_BLUE,
      BTN_PL2_RED, BTN_PL2_GREEN, BTN_PL2_BLUE,
      MAX_BTN
   };

enum{ PLAYER_1, PLAYER_2 };

const int rgb_led_pins[] = { 9, 10, 11 };
const int btn_pins[] = { 2, 3, 4, 5, 6, 7 };

//Constants
#define ledBrightness 100

bool player1set = 0;
bool player2set = 0;
bool letter1set = 0;
bool letter3set = 0;
char player1name[3] = "AA";
char player2name[3] = "BB";
char letter1 = 'A';
char letter2 = 'A';
char letter3 = 'A';
char letter4 = 'A';
byte scorePlayer1 = 0;
byte scorePlayer2 = 0;


//OLED object
U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);

//Function declarations
void ledOff(void);
void ledOn(byte PinNo);
void ledBlink(byte PinNo);
void drawDisplay(byte mode, byte num);
void setName(void);
void genNames(void);
void changeLetterP1(bool upDown);
void changeLetterP2(bool upDown);
byte ledRandom(void);
void countDown(byte n);
int game_get_winner(int led_color);

void setup()
{
    randomSeed(analogRead(0));

    for(int i = 0; i < MAX_BTN; i++)
    {
        if(i < MAX_COLORS)
        {
            pinMode(rgb_led_pins[i], OUTPUT);
        }

        pinMode(btn_pins[i], INPUT_PULLUP);
    }

    ledOff();
}

void loop()
{
    if(!player1set)
    {
        setName();
    }

    display.firstPage();

    do{
        drawDisplay(1,0);
    }while(display.nextPage());

    ledOff();

    while(1)
    {
        if(button_get_status(BTN_PL2_GREEN) ||
           button_get_status(BTN_PL2_GREEN))
        {
           break;
        }
    }

    countDown(3);
    int winner = game_get_winner(ledRandom());

    display.firstPage();

    do{
        drawDisplay(4, winner);
    }while(display.nextPage());

    delay(1000);
}

int button_get_status(int button)
{
    return !digitalRead(btn_pins[button]);
}

int game_get_winner(int led_color)
{
    int button_pressed_first, player_pressed;

    while(1)
    {
        for(int i = 0; i < MAX_BTN; i++)
        {
            if(button_get_status(i))
            {
                button_pressed_first = i;
                break;
            }
        }
    }

    if(button_pressed_first > BTN_PL1_BLUE)
    {
        player_pressed = PLAYER_2;
        button_pressed_first -= 3;
    }

    else
    {
        player_pressed = PLAYER_1;
    }

    return (button_pressed_first == led_color ?
        player_pressed : !player_pressed);
}

void countDown(byte n)
{
    while(n)
    {
        display.firstPage();
        do{
            drawDisplay(3,n);
        }while(display.nextPage());

        n--;
        delay(900);
    }
}

//Turns off LED
void ledOff(void)
{
    analogWrite(rgb_led_pins[BLUE], 0);
    analogWrite(rgb_led_pins[RED], 0);
    analogWrite(rgb_led_pins[GREEN], 0);
}

//Turns on LED
void ledOn(byte PinNo)
{
    ledOff();
    delay(10);
    analogWrite(PinNo, ledBrightness);
}

//Blinks led
void ledBlink(byte PinNo)
{
    for(int i = 0; i<5; i++)
    {
        ledOff();
        delay(200);
        ledOn(PinNo);
        delay(200);
    }

    ledOff();
}

byte ledRandom(void)
{
    int randomPin = random(rgb_led_pins[RED], rgb_led_pins[BLUE] + 1);
    ledOn(randomPin);
    return randomPin;
}

//Changes letters for player 1 name input
void changeLetterP1(bool upDown)
{
    if(upDown)
    {
        if(!letter1set)
        {
            letter1++;
        }

        else
        {
            letter2++;
        }
    }

    else
    {
        if(!letter1set)
        {
            letter1--;
        }

        else
        {
            letter2--;
        }
    }
}

//Changes letters for player 2 name input
void changeLetterP2(bool upDown)
{
    if(upDown)
    {
        if(!letter3set)
        {
            letter3++;
        }

        else
        {
            letter4++;
        }
    }

    else
    {
        if(!letter3set)
        {
            letter3--;
        }

        else
        {
            letter4--;
        }
    }
}

//Allows players to set names (initials)
void setName(void)
{
    while(1)
    {
        display.firstPage();

        do{
            drawDisplay(2,0);
        }while(display.nextPage());

        if(button_get_status(BTN_PL1_RED))
        {
            changeLetterP1(1);
        }

        if(button_get_status(BTN_PL2_RED))
        {
            changeLetterP2(1);
        }

        if(button_get_status(BTN_PL1_BLUE))
        {
            changeLetterP1(0);
        }

        if(button_get_status(BTN_PL2_BLUE))
        {
            changeLetterP2(0);
        }

        if(button_get_status(BTN_PL1_GREEN))
        {
            if(letter1set)
            player1set = 1;
            letter1set = 1;
        }

        if(button_get_status(BTN_PL2_GREEN))
        {
            if(letter3set)
            player2set = 1;
            letter3set = 1;
        }

        if(player2set && player1set)
        {
            ledBlink(rgb_led_pins[GREEN]);
            return; //exits function -- no need to break while-loop
        }
    }
}

//Display output
void drawDisplay(byte mode, byte num)
{
    switch(mode)
    {
        case 1: //Display Score
            genNames();
            char scorePlayer1String[3];
            char scorePlayer2String[3];
            sprintf(scorePlayer1String, "%d", scorePlayer1);
            sprintf(scorePlayer2String, "%d", scorePlayer2);
            display.drawLine(64,0,64,45);
            display.setFont(u8g_font_helvB12);
            display.drawStr(0,20, player1name);
            display.drawStr(0,35, scorePlayer1String);
            display.drawStr(100,20, player2name);
            display.drawStr(100,35, scorePlayer2String);
            display.setFont(u8g_font_helvB10);
            display.drawStr(5,60, "GREEN - START");
        break;

        case 2: //Set names
            genNames();
            display.drawLine(64,0,64,64);
            display.setFont(u8g_font_helvB10);
            display.drawStr(0,20, "P1 Name");
            display.drawStr(0,40, player1name);
            display.drawStr(66,20, "P2 Name");
            display.drawStr(66,40, player2name);
        break;

        case 3: //Countdown
            char countDownNumber[3];
            sprintf(countDownNumber, "%d", num);
            display.setFont(u8g_font_fub49n);
            display.drawStr(40,60, countDownNumber);
            break;

        case 4: //Display winner
            genNames();
            display.setFont(u8g_font_fub25r);
            display.drawStr(0,30, "Winner!");

            if(num == 1)
            {
                display.drawStr(40,60, player1name);
            }

            else
            {
                display.drawStr(40,60, player2name);
            }

            break;
    }
}

//Prints player initials to name strings.
void genNames(void)
{
    sprintf(player1name, "%c%c", letter1, letter2);
    sprintf(player2name, "%c%c", letter3, letter4);
}

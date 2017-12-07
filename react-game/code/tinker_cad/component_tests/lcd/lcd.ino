/*  Project simulation
    TinkerCad LCD test code

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-07

    Testing Liquid Crystal Display
 */

#include <LiquidCrystal.h>

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

void lcd_print_string(String s, int col, int row, bool clear);

void setup()
{
    lcd.begin(16, 2);
}

void loop()
{
    for(int i = 0; i < 10; i++)
    {
        lcd_print_string("Player 1: " + String(i),     0, 0, true);
        lcd_print_string("Player 2: " + String(i + 2), 0, 1, false);
        delay(1000);
    }
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

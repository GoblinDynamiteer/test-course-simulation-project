/*  Project simulation
    TinkerCad

    Mjukvaruutveckare Inbyggda System
    Johan Kampe
    2017-12-07

    Reaction game refactored.
*/

#include <Button.h>
#include <U8glib.h>

//Constants
#define pinLEDRed 9
#define pinLEDGreen 10
#define pinLEDBlue 11
#define ledBrightness 100

//Button objects for player buttons
Button player1Blue(2);
Button player1Green(3);
Button player1Red(4);
Button player2Blue(5);
Button player2Green(6);
Button player2Red(7);


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
byte getWinner(byte pin);

void setup(){
	randomSeed(analogRead(0));
	player1Blue.begin();
	player1Green.begin();
	player1Red.begin();
	player2Blue.begin();
	player2Green.begin();
	player2Red.begin();
	pinMode(pinLEDRed, OUTPUT);
	pinMode(pinLEDGreen, OUTPUT);
	pinMode(pinLEDBlue, OUTPUT);
	ledOff();
}

void loop(){
	if(!player1set){
		setName();
		}
	display.firstPage();
	do{
		drawDisplay(1,0);
	}while(display.nextPage());
	ledOff();
	while(1){
		if(player1Green.pressed() || player2Green.pressed())
			break;
		}
	countDown(3);
	int winner = getWinner(ledRandom());
	display.firstPage();
	do{
		drawDisplay(4,	winner);
	}while(display.nextPage());
	delay(1000);
}

byte getWinner(byte pin){
	byte winner = 0,  inputP1 = 0, inputP2 = 0;
	while(1){
		if(player1Red.pressed()){
			inputP1 = pinLEDRed;
			break;
		}
		if(player1Blue.pressed()){
			inputP1 = pinLEDBlue;
			break;
		}
		if(player1Green.pressed()){
			inputP1 = pinLEDGreen;
			break;
		}
		if(player2Red.pressed()){
			inputP2 = pinLEDRed;
			break;
		}
		if(player2Blue.pressed()){
			inputP2 = pinLEDBlue;
			break;
		}
		if(player2Green.pressed()){
			inputP2 = pinLEDGreen;
			break;
		}
	}
	//If a player has pressed a button that is same as LED color
	if(inputP1 == pin){
		winner = 1;
		scorePlayer1++;
	}
	else if(inputP2 == pin){
		winner = 2;
		scorePlayer2++;
	}
	//If a player has pressed a button that is not same as LED color
	//score goes to other player
	else if((inputP1 != 0) && (inputP1 != pin)){
		winner = 2;
		scorePlayer2++;
	}
	else if((inputP2 != 0) && (inputP2 != pin)){
		winner = 1;
		scorePlayer1++;
	}
	return winner;
}

void countDown(byte n){
	while(n){
		display.firstPage();
		do{
			drawDisplay(3,n);
		}while(display.nextPage());
		n--;
		delay(900);
	}
}


//Turns off LED
void ledOff(void){
	analogWrite(pinLEDBlue, 0);
	analogWrite(pinLEDRed, 0);
	analogWrite(pinLEDGreen, 0);
}

//Turns on LED
void ledOn(byte PinNo){
	ledOff();
	delay(10);
	analogWrite(PinNo, ledBrightness);
}

//Blinks led
void ledBlink(byte PinNo){
	for(int i = 0;i<5;i++){
		ledOff();
		delay(200);
		ledOn(PinNo);
		delay(200);
	}
	ledOff();
}

byte ledRandom(void){
	int randomPin = random(pinLEDRed,pinLEDBlue+1);
	ledOn(randomPin);
	return randomPin;
}

//Changes letters for player 1 name input
void changeLetterP1(bool upDown){
	if(upDown)
		if(!letter1set){letter1++;}
		else{letter2++;}
	else
		if(!letter1set){letter1--;}
		else{letter2--;}
}

//Changes letters for player 2 name input
void changeLetterP2(bool upDown){
	if(upDown)
		if(!letter3set){letter3++;}
		else{letter4++;}
	else
		if(!letter3set){letter3--;}
		else{letter4--;}
}

//Allows players to set names (initials)
void setName(void){
	while(1){
		display.firstPage();
		do{
			drawDisplay(2,0);
		}while(display.nextPage());

		if(player1Red.pressed())
			changeLetterP1(1);
		if(player2Red.pressed())
			changeLetterP2(1);
		if(player1Blue.pressed())
			changeLetterP1(0);
		if(player2Blue.pressed())
			changeLetterP2(0);

		if(player1Green.pressed()){
			if(letter1set)
				player1set = 1;
			letter1set = 1;
		}

		if(player2Green.pressed()){
			if(letter3set)
				player2set = 1;
			letter3set = 1;
		}
		if(player2set && player1set){
			ledBlink(pinLEDGreen);
			return; //exits function -- no need to break while-loop
			}
	}
}

//Display output
void drawDisplay(byte mode, byte num){
	switch(mode){
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
				if(num == 1){
					display.drawStr(40,60, player1name);
				}
				else{
					display.drawStr(40,60, player2name);
				}
			break;
	}
}

//Prints player initials to name strings.
void genNames(void){
	sprintf(player1name, "%c%c", letter1, letter2);
	sprintf(player2name, "%c%c", letter3, letter4);
}

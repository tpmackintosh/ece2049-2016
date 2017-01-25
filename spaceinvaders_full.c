/************** ECE2049 DEMO CODE ******************/
/**************  20 August 2016   ******************/
/***************************************************/

#include <msp430.h>
#include <stdlib.h>
#include <time.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"

void playGame();
void countDown();
void swDelay(long int n);
void displayRow(const char *arr, int sz);
void loseBuzz;

// Declare globals here
int state = 0;
// Main
void main(void)
{
	srand(time(NULL));
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

    // Useful code starts here
    initLeds();

    configDisplay();
    configKeypad();

  	// *** Intro Screen ***

    GrClearDisplay(&g_sContext); // Clear the display

  	while(1){
  		char curr_key = getKey();
  		if(curr_key == '*'){
  			state = 1;
  		}
  		switch (state){
  		case 0:
  			// Write some text to the display
  		  	GrStringDrawCentered(&g_sContext, "SPACE INVADERS", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
  		  	GrFlush(&g_sContext);
  		  	break;
  		case 1:
  			countDown();
  			playGame();
				loseBuzz();
  			GrClearDisplay(&g_sContext);
  			state = 0;
  			break;
  		}
  	}
}

void playGame(){
	int y = 0; int aliens = 0;
	long int counter = 0;
	char base_row[] = "1  2  3  4  5";

	displayRow(base_row,y);

	while(1){
		if (counter == 10000){
			GrClearDisplay(&g_sContext);
			y++;
			if(y == 5){break;}
			displayRow(base_row,y);
			counter = 0;
		}
		char curr_key = ' ';
		curr_key = getKey();
		counter++;

		switch(curr_key){
		case '1':
					GrClearDisplay(&g_sContext);
					// Check if row[x] == ' '
					base_row[0] = ' ';
					aliens++;
					displayRow(base_row,y);
					break;
		case '2':
					GrClearDisplay(&g_sContext);
					base_row[3] = ' ';
					aliens++;
					displayRow(base_row,y);
					break;
		case '3':
					GrClearDisplay(&g_sContext);
					base_row[6] = ' ';
					aliens++;
					displayRow(base_row,y);
					break;
		case '4':
					GrClearDisplay(&g_sContext);
					base_row[9] = ' ';
					aliens++;
					displayRow(base_row,y);
					break;
		case '5':
					GrClearDisplay(&g_sContext);
					base_row[12] = ' ';
					aliens++;
					displayRow(base_row,y);
					break;
		}
		if (aliens == 5){break;}
	}
}

void countDown(){
	long int delay = 40000;

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	swDelay(delay);
	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	swDelay(delay);
	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	swDelay(delay);
	GrClearDisplay(&g_sContext);
}

void swDelay(long int n){
	long int i;
	for (i = 0; i< n; i++){}
}

void displayRow(const char *arr, int y){
	GrStringDrawCentered(&g_sContext, arr, AUTO_STRING_LENGTH, 48, 16 + 16*y, TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
}

void loseBuzz(){
	buzzerOn(void);
	swDelay(1000);
	buzzerOff(void);
	swDelay(2000);
	buzzerOn(void);
	swDelay(2500);
	buzzerOff(void);
}

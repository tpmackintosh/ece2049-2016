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
void loseBuzz();
char displayGrid(const char *arr1,const char *arr2,const char *arr3,const char *arr4,const char *arr5);
void checkAlienKill(char* arr1,char* arr2,char* arr3,char* arr4,char* arr5, int column);

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
    long int music_count = 0;
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
  		  	GrStringDrawCentered(&g_sContext, "Press * to Play", AUTO_STRING_LENGTH, 48, 64, TRANSPARENT_TEXT);
  		  	GrFlush(&g_sContext);

//  		  	music_count++;
//  		  	if(music_count <= 1000){
//  		  		BuzzerOn(1/4);
//  		  	}
//  		  	if(music_count > 1000){
//  		  		BuzzerOff();
//  		  		BuzzerOn(1);
//  		  	}
//  		  	if(music_count > 1500){
//  		  		BuzzerOff();
//  		  		music_count = 0;
//  		  	}

  		  	break;
  		case 1:
  			BuzzerOff();
  			countDown();
  			playGame();
  			GrClearDisplay(&g_sContext);
  			GrStringDrawCentered(&g_sContext, "GAME OVER", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
  			GrFlush(&g_sContext);
  			swDelay(50000);
  			GrClearDisplay(&g_sContext);
  			state = 0;
  			loseBuzz();
  			break;
  		}
  	}
}

void playGame(){
	int i;
	int done = 0;
	int difficulty = 8;
	long int up_diff = 0;
	long int counter_max = 20000;
	long int counter = 0;
	char row_preload[] = "             ";
	char row1[] = "             ";
	char row2[] = "             ";
	char row3[] = "             ";
	char row4[] = "             ";
	char row5[] = "             ";
	char row_check[] = "             ";

	//char rand_arr[13];

	for(i=0; i<5; i++){
		if(rand()%difficulty == 1){
			row_preload[3*i] = '1';
		}
	}



	displayGrid(row1,row2,row3,row4,row5);

	while(1){
		up_diff++;
		if (up_diff == 40000){
			if(difficulty > 4){
				difficulty--;
			}
			up_diff = 0;
			counter_max /= 1.5;
		}

		if (counter >= counter_max){
			GrClearDisplay(&g_sContext);
			// Switch all rows
			for (i = 0; i < 13; i++){
			row5[i] = row4[i];
			row4[i] = row3[i];
			row3[i] = row2[i];
			row2[i] = row1[i];
			row1[i] = row_preload[i];
			}
			// Need to randomize PL
			for(i=0; i<5; i++){
				if(rand()%difficulty == 1){
					row_preload[3*i] = '1';
				}
			}
			if(done == 1){
				break;
			}
			for(i = 0; i<13; i++){
				if(row5[i] != row_check[i]){
					done = 1;
				}
			}

			displayGrid(row1,row2,row3,row4,row5);

			counter = 0;
		}
		char curr_key = ' ';
		curr_key = getKey();
		counter++;

		switch(curr_key){
		case '1':
				GrClearDisplay(&g_sContext);
				checkAlienKill(row1,row2,row3,row4,row5,0);
				displayGrid(row1,row2,row3,row4,row5);
				break;
		case '2':
				GrClearDisplay(&g_sContext);
				checkAlienKill(row1,row2,row3,row4,row5,1);
				displayGrid(row1,row2,row3,row4,row5);
				break;
		case '3':
				GrClearDisplay(&g_sContext);
				checkAlienKill(row1,row2,row3,row4,row5,2);
				displayGrid(row1,row2,row3,row4,row5);
				break;
		case '4':
				GrClearDisplay(&g_sContext);
				checkAlienKill(row1,row2,row3,row4,row5,3);
				displayGrid(row1,row2,row3,row4,row5);
				break;
		case '5':
				GrClearDisplay(&g_sContext);
				checkAlienKill(row1,row2,row3,row4,row5,4);
				displayGrid(row1,row2,row3,row4,row5);
				break;
		}
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
}

void loseBuzz(){
	BuzzerOn(1);
	swDelay(10000);
	BuzzerOff();
	swDelay(2000);
	BuzzerOn(1);
	swDelay(25000);
	BuzzerOff();
}

char displayGrid(const char *arr1,const char *arr2,const char *arr3,const char *arr4,const char *arr5){
	displayRow(arr1,0);
	displayRow(arr2,1);
	displayRow(arr3,2);
	displayRow(arr4,3);
	displayRow(arr5,4);
	GrFlush(&g_sContext);
}

void checkAlienKill(char* arr1,char* arr2,char* arr3,char* arr4,char* arr5, int column){
	if(arr5[column*3] != ' '){
		arr5[column*3] = ' ';
	}
	else if(arr4[column*3] != ' '){
		arr4[column*3] = ' ';
	}
	else if(arr3[column*3] != ' '){
		arr3[column*3] = ' ';
	}
	else if(arr2[column*3] != ' '){
		arr2[column*3] = ' ';
	}
	else if(arr1[column*3] != ' '){
		arr1[column*3] = ' ';
	}
}

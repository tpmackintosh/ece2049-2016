/************** ECE2049 DEMO CODE ******************/
/**************  20 August 2016   ******************/
/***************************************************/

#include <msp430.h>
#include "peripherals.h"

// Function prototypes

struct Notes{
		char pitch;
		int duration;
};

void countDown(void);
void swDelay(long int n);
void BuzzerOnPitch(char pitch);
char playNote(char pitch, char dur);
void configTA2(void);
void letsShred(void);
void initSW(void);
char swState(void);
void playerWin(void);
void playerLose(void);

long unsigned int timer = 0;
char lose_cnt = 0;

void main(void){
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

    // Config
    initLeds();
    configDisplay();
    configKeypad();
    initSW();

    // Declare variables
    char state = 0;


    GrClearDisplay(&g_sContext); // Clear the display

    _BIS_SR(GIE);

      	while(1){
      		char curr_key = getKey();
      		if(curr_key == '*'){
      			state = 1;
      		}

      		switch (state){
      		case 0: // Welcome screen
      			// Write some text to the display
      		  	GrStringDrawCentered(&g_sContext, "MSP430 HERO", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
      		  	GrStringDrawCentered(&g_sContext, "Press * to Play", AUTO_STRING_LENGTH, 48, 64, OPAQUE_TEXT);
      		  	GrFlush(&g_sContext);
      		  	break;
      		case 1:
      			countDown();
      			letsShred();
      			BuzzerOff();
      			state = 0;
      			break;

      		}
      	}
}

void countDown(void){
	long int delay = 40000;

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
	GrFlush(&g_sContext);
	setLeds(BIT3);
	BuzzerOnPitch(66);
	swDelay(delay);

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
	BuzzerOff();
	BuzzerOnPitch(66);
	GrFlush(&g_sContext);
	setLeds(BIT2);
	swDelay(delay);

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
	BuzzerOff();
	BuzzerOnPitch(66);
	GrFlush(&g_sContext);
	setLeds(BIT1);
	swDelay(delay);

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "GO", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
	BuzzerOff();
	BuzzerOnPitch(63);
	GrFlush(&g_sContext);
	setLeds(0x0F);
	swDelay(delay);

	GrClearDisplay(&g_sContext);
	BuzzerOff();

	setLeds(0x00);
}

void swDelay(long int n){
	long int i;
	for (i = 0; i< n; i++){}
}

void BuzzerOnPitch(char pitch)
{
	// Initialize PWM output on P3.5, which corresponds to TB0.5
	P3SEL |= BIT5; // Select peripheral output mode for P3.5
	P3DIR |= BIT5;

	TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
	TB0CTL  &= ~TBIE; 						// Explicitly Disable timer interrupts for safety

	// Now configure the timer period, which controls the PWM period
	// Doing this with a hard coded values is NOT the best method
	// We do it here only as an example. You will fix this in Lab 2.
	TB0CCR0   = pitch; 					// Set the PWM period in ACLK ticks, original 128
	TB0CCTL0 &= ~CCIE;					// Disable timer interrupts

	// Configure CC register 5, which is connected to our PWM pin TB0.5
	TB0CCTL5  = OUTMOD_7;					// Set/reset mode for PWM
	TB0CCTL5 &= ~CCIE;						// Disable capture/compare interrupts
	TB0CCR5   = TB0CCR0/2; 					// Configure a 50% duty cycle
}

void letsShred(void){

	char D = 56;
	char E = 50;
	char G = 42;
	char F = 47;
	char Fs = 44;
	char A = 37;
	char D_hi = 31;
	char B_hi = 34;
	char C_hi = 32;

	char pitch_arr_song1[] = {D, D, E, D, G, Fs, D, D ,E, D, A, G, D, D, D_hi, B_hi, G, Fs, E, C_hi, C_hi, B_hi, G, A, G};
	char dur_arr_song1[] = {1, 1, 2, 2, 2, 4, 1, 1, 2, 2, 2, 4, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 4};
	int i;

	// Playing song:
	char temp = 0;
	for(i = 0; i < 25; i++){
		temp = playNote(pitch_arr_song1[i], dur_arr_song1[i]);

		if(temp == 1){
			break;
		}
		if(temp == 2){
			playerLose();
			break;
		}
		if(i==24){
			playerWin();
		}
	}
}

char playNote(char pitch, char dur){
	BuzzerOnPitch(pitch);
	//int counter = 0;
	char sw = 0;
	timer = 0;
	char buttcheck = 0;

	if(pitch == 56 | pitch == 44 | pitch == 32){
		setLeds(BIT3);
		buttcheck = BIT0; // SW1
	}
	if(pitch == 50 | pitch == 37){
		setLeds(BIT2);
		buttcheck = BIT6; // SW2
	}
	if(pitch == 42 | pitch == 31){
		setLeds(BIT1);
		buttcheck = BIT2; // SW3
	}
	if(pitch == 47 | pitch == 34){
		setLeds(BIT0); // SW4
		buttcheck = BIT4;
	}
	char temp = 0;

	while(1){
		char curr_sw = swState();
		char curr_key = getKey();

		if (curr_sw == buttcheck){
			temp = 1;
		}
		if(sw == 0){
			configTA2();
			sw = 1;
		}
		if(timer > dur * 500){ // 1/8th note ~ 0.5 sec
			BuzzerOff();
			if(!temp){
				BuzzerOnPitch(254);
				timer=0;
				while (timer < 100){}
				timer = 0;
				lose_cnt++;
				if(lose_cnt == 5){
					return 2;
				}
			}
			timer=0;
			setLeds(0x00);
			while (timer < 100){}
			timer = 0;
			return 0;
		}
		if(curr_key == '#'){
			setLeds(0x00);
			return 1;
		}
	}
}

void configTA2(void){
	TA2CTL  = (TASSEL_1 + MC_1 + ID_0);  // Configure Timer A2 to use ACLK, divide by 1, up mode
	TA2CCR0 = 33;							// 32+1 = 33 ACLK ticks ~0.001 sec
	TA2CCTL0  = CCIE; 						// Explicitly enable timer
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR (void){
	timer++;
}

void initSW(void){
	P7SEL &= ~(BIT0 | BIT4);
	P7DIR &= ~(BIT0 | BIT4);
	P7REN |= (BIT0 | BIT4);
	P7OUT |= (BIT0 | BIT4);

	P3SEL &= ~(BIT6);
	P3DIR &= ~(BIT6);
	P3REN |= (BIT6);
	P3OUT |= (BIT6);

	P2SEL &= ~(BIT2);
	P2DIR &= ~(BIT2);
	P2REN |= (BIT2);
	P2OUT |= (BIT2);
}

char swState(void){
	//unsigned char state = 0x00;
	unsigned char mask = 0x00;
	// 0x01, 0x04, 0x10, 0x40 	0100 0100
	//state = (P7IN  P3IN & P2IN);

	if(~P7IN & BIT0){ // SW1
		mask |= BIT0; //
	}
	if(~P2IN & BIT2){ // SW3
		mask |= BIT2;
	}
	if(~P7IN & BIT4){ // SW4
		mask |= BIT4;
	}
	if(~P3IN & BIT6){ // SW2
		mask |= BIT6;
	}

	return mask;
}

void playerWin(void){
	BuzzerOnPitch(20);
	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "YOU WIN!!!!!", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
	GrFlush(&g_sContext);
	timer=0;
	setLeds(0x0F);
	while (timer < 2000){}
	timer = 0;
	BuzzerOff();
	setLeds(0x00);
}

void playerLose(void){
	BuzzerOnPitch(254);
	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "YOU lose", AUTO_STRING_LENGTH, 48, 48, OPAQUE_TEXT);
	GrFlush(&g_sContext);
	timer=0;
	setLeds(0x0F);
	while (timer < 2000){}
	timer = 0;
	BuzzerOff();
	setLeds(0x00);
}

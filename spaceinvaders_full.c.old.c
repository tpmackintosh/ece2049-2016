#include <msp430.h>
#include "peripherals.h"


 // Don’t remove any #include statements
int play_pulse(); // Function to detect if '*' is pressed
void playGame();
void display5Aliens(char alien_arr, int sz);

char alien_arr = ['0','1','2','3','4','5']
char curr_key; // Current key pressed
long int counter;

void main(void) {
   WDCTL = WDTPW | WDTHOLD; // Stop watchdog timer
   // Useful code starts here
   initLeds();
   configDisplay();
   configKeypad();
   // Your setup code goes here!
   // (Initialize variables, configure things, etc.)
   enum GAME_STATE = {WAITING = 0 , PLAYING = 1};
   int temp;

   enum GAME_STATE state = WAITING;
   while(1) {
       switch (state){
         case WAITING:
            // Intro screen display
            GrClearDisplay(&g_sContext); // Clear the display
            GrStringDrawCentered(&g_sContext, "SPACE INVADERS", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            GrStringDrawCentered(&g_sContext, "Press * to Play", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
            GrFlush(&g_sContext);

            temp = play_pulse(); //Output '0' if '*' pressed, '1' otherwise

            if (temp == 0){
              state = PLAYING;
            }
            else{
              state = WAITING;
            }
            break;
         case PLAYING:
            GrClearDisplay(&g_sContext); // Clear the display

            playGame(); //Executes game function, plays until loss
            state = WAITING;
            break;
       }
     }
}

int play_pulse(){
  int i;
  curr_key = getKey();
  if (curr_key == '*'){
    return 0;
  }
  else{
    return 1;
  }
}

void playGame{
  // 3 - 2 - 1 countdown
  for(i = 3; i >= 1; i--){
    GrStringDrawCentered(&g_sContext, i, AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    for(counter = 0; counter <=40000; counter++){}
  }

  int counter = 0;
  int y = 1;
  int max_time = 40000;
  char level1 = alien_arr

  while(1){
    curr_key = getKey();
    display5Aliens(level1, 5, 16*y);
    counter++;
    if(counter == max_time){
      y++;
    }
    for(i=0; i<5; i++){
      if (curr_key == level1[i]){
        level1[i] = NULL;
      }
      if (level1 == [NULL,NULL,NULL,NULL,NULL]){
        break;
      }
    }
  }
}

void display5Aliens(char alien_arr[], int sz, int y){
  for(i=0;i<sz;i++){
    GrStringDrawCentered(&g_sContext, alien_arr[i], AUTO_STRING_LENGTH, 16 + 0*16, y, TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
  }
}

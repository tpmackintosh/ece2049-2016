
#include <msp430.h>

// Temperature Sensor Calibration = Reading at 30 degrees C is stored at addr 1A1Ah
// See end of datasheet for TLV table memory mapping
#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
// Temperature Sensor Calibration = Reading at 85 degrees C is stored at addr 1A1Ch                                            //See device datasheet for TLV table memory mapping
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)

unsigned int in_temp;

int main(void)
{
  volatile float temperatureDegC;
  volatile float temperatureDegF;
  volatile float degC_per_bit;
  volatile unsigned int bits30, bits85;

  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
  
  REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
                          // internal reference voltages to
  	  	  	  // ADC12_A control registers
  	  	  	  
  ADC12CTL0 = ADC12SHT0_9 | ADC12REF2_5V | ADC12REFON | ADC12ON;     // Internal ref = 2.5V
  
  ADC12CTL1 = ADC12SHP;                     // Enable sample timer
  
  // Using ADC12MEM0 to store reading
  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense
  	                                    // ACD12SREF_1 = internal ref = 2.5v

  __delay_cycles(100);                    // delay to allow Ref to settle
  ADC12CTL0 |= ADC12ENC;           	  // Enable conversion

  // Use calibration data stored in info memory
  bits30 = CALADC12_15V_30C;
  bits85 = CALADC12_15V_85C;
  degC_per_bit = ((float)(100.0 + 40.0))/((float)(bits85-bits30));


  while(1)
  {
    ADC12CTL0 &= ~ADC12SC; 	// clear the start bit
    ADC12CTL0 |= ADC12SC;       // Sampling and conversion start
    			        // Single conversion (single channel)

    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY){
    	__no_operation();
    }
    in_temp = ADC12MEM0;      // Read in results if conversion

    // Temperature in Celsius. See the Device Descriptor Table section in the
    // System Resets, Interrupts, and Operating Modes, System Control Module
    // chapter in the device user's guide for background information on the
    // used formula.
    temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit -40.0;

    // Temperature in Fahrenheit Tf = (9/5)*Tc + 32
    temperatureDegF = temperatureDegC * 9.0/5.0 + 32.0;
	 
    __no_operation();                       // SET BREAKPOINT HERE
  }
}

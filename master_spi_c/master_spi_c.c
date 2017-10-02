/**
 * Send SPI Message to a Slave using a button press
 * 
 * 
 * 
 */
#include <msp430.h>
/**Value to Send**/
volatile unsigned char MST_Data = 0x04; 
void nop_delay(unsigned int d) {
  int i;
  for (i = 0; i<d; i++) {
    nop();
  }
}
int main(void)
{
/**Set all outputs to 0**/
P1OUT = 0x00; // P1 setup for LED & reset output
/**Quick Delay**/
nop_delay(1000);
/**Stop Watchdog Timer**/
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

/**Set pins 1.0 1.5 1.6 as **/
P1DIR |= BIT0 + BIT5 + BIT6; //
P1SEL = BIT1 + BIT2 + BIT4;
P1SEL2 = BIT1 + BIT2 + BIT4;

/**Set microcontroller to be 3-pin, 8 bit SPI Master**/
UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master

/**Set SCL from SMCLK**/
UCA0CTL1 |= UCSSEL_2; 

/**Set-up Clock**/
UCA0BR0 |= 0x02;
UCA0BR1 = 0; 
UCA0MCTL = 0; 

/**Initialize USCI state machine**/
UCA0CTL1 &= ~UCSWRST; 

/**SET SLAVE SELECT HIGH**/
P1OUT |= BIT5;

/** Wait for slave to initialize**/
nop_delay(2000); 

/**Button Interupts**/
P1IE |= BIT3;
P1IFG &= ~BIT3;

/**Enable Interrupts**/
__bis_SR_register(GIE); 
}

/**Interrupt for button Press**/
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
  /**SS LOW**/
  P1OUT &= ~BIT5;
  /**Wait for the thing to be ready**/
  while (!(IFG2 & UCA0TXIFG));
  /**LED high to show its getting something**/
  P1OUT |= BIT6;

  /**Send value**/
  UCA0TXBUF = MST_Data; 
  /**Turn off LED to show it is sent**/
  nop_delay(50);
  P1OUT &= ~BIT6;
  /**Turn off SS and reset interrupt**/
  P1OUT |= BIT5;
  P1IFG &=~BIT3;
}


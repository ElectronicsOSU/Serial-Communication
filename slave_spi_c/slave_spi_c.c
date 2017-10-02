#include "msp430g2553.h"
#include <string.h>

volatile int received_int;
volatile int flag = 0;

/** Delay function. **/
void nop_delay(unsigned int d) {
  int i;
  for (i = 0; i<d; i++) {
    nop();
  }
}
/**Function the flash when SPI is First Detected**/
void flash_spi_detected(void) {
    int i=0;
    P1OUT = 0;
    for (i=0; i < 8; ++i) {
        P1OUT = ~P1OUT;
        nop_delay(0x4fff);
        nop_delay(0x4fff);
    }
    P1OUT = 0;
    nop_delay(1000);
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  /**Setup LEDS**/
  P1DIR |= BIT0 + BIT6;
  /**Set all outputs to 0**/
  P1OUT = 0;
  /**Set direction SS to hight**/
  P1DIR &= ~BIT5;
  /**Wait for master to turn on**/
  while (P1IN & BIT4);                   // If clock sig from mstr stays low,
                                            // it is not yet in SPI mode

  flash_spi_detected();                 // Blink 1 times

  /**Configure P1.1 P1.2 and P1.4 to be SPI Pins**/
  P1SEL = BIT1 + BIT2 + BIT4;
  P1SEL2 = BIT1 + BIT2 + BIT4;
  /**Put USCI state machine in reset**/
  UCA0CTL1 = UCSWRST;        
  /**3-pin, 8-bit SPI master**/              
  UCA0CTL0 |= UCMSB + UCSYNC;   
  /**Initialize USCI state machine**/            
  UCA0CTL1 &= ~UCSWRST;                     
  IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt
  while(1)
  {
    if(flag == 1)
    {
      while(received_int>0)
      {
        received_int--;
        P1OUT |= BIT0;
        nop_delay(0x4fff);
        P1OUT &=~BIT0;
        nop_delay(0x4fff);
        flag = 0;
      }
    }
  }
 // __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
}

__attribute__((interrupt(USCIAB0RX_VECTOR))) void USCI0RX_ISR (void)
{
  
  while (!(IFG2 & UCA0TXIFG ));
  if(!(P1IN & BIT5))
  {
  P1OUT |= BIT6;
  P1OUT |= BIT0;
  received_int = UCA0RXBUF;
  flag = 1;
  /*while( i<value) {
    P1OUT |= BIT6;
    nop_delay(0x4fff);
    P1OUT &=~BIT6;
    nop_delay(0x4fff);
    i++;
  }
  P1OUT &= ~BIT0;
  P1OUT &=~BIT6;
  nop_delay(0x4fff);
  nop_delay(0x4fff);
  nop_delay(0x4fff);
  
  UCA0TXBUF = value;  
  }
  else{}
    /*
  if (UCA0STAT & UCOE) {
    P1OUT |= BIT0;
  }
  */
  
}

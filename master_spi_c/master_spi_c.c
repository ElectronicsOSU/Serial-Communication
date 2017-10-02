#include <msp430.h>

unsigned char MST_Data, SLV_Data;
void nop_delay(unsigned int d) {
  int i;
  for (i = 0; i<d; i++) {
    nop();
  }
}
int main(void)
{
volatile unsigned int i;
P1OUT = 0x00; // P1 setup for LED & reset output
nop_delay(1000);
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

P1DIR |= BIT0 + BIT5 + BIT6; //
P1SEL = BIT1 + BIT2 + BIT4;
P1SEL2 = BIT1 + BIT2 + BIT4;
UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
UCA0CTL1 |= UCSSEL_2; // SMCLK
UCA0BR0 |= 0x02; // /2
UCA0BR1 = 0; //
UCA0MCTL = 0; // No modulation
UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
//IE2 |= UCA0RXIE; // Enable USCI0 RX interrupt



//P1OUT &= ~BIT5; // Now with SPI signals initialized,
P1OUT |= BIT5; // reset slave
P1OUT |= BIT6;
nop_delay(2000); // Wait for slave to initialize
MST_Data = 0x01; // Initialize data values
SLV_Data = 0x00;
P1OUT &= ~BIT6;
//UCA0TXBUF = MST_Data; // Transmit first character

P1IE |= BIT3;
P1IFG &= ~BIT3;

__bis_SR_register(LPM0_bits + GIE); // CPU off, enable interrupts
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
  P1OUT &= ~BIT5;
  volatile unsigned int i;
  nop_delay(0x4fff);
  while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
  P1OUT |= BIT6;

  MST_Data = 0x4;
  // Increment master value // Increment expected slave value
  nop_delay(0x4fff);
  UCA0TXBUF = MST_Data; // Send next value
  P1OUT &= ~BIT6;
  nop_delay(50);
  P1OUT |= BIT5;
  P1IFG &=~BIT3;
}
// Test for valid RX and TX character
/*#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
{
volatile unsigned int i;
nop_delay(0x4fff);
while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
P1OUT |= BIT6;
/*if (UCA0RXBUF == SLV_Data) // Test for correct character RX'd
P1OUT |= BIT0; // If correct, light LED
else
MST_Data++;
if(MST_Data == 20)
{
  MST_Data = 1;
}
 // Increment master value // Increment expected slave value
nop_delay(0x4fff);
UCA0TXBUF = MST_Data; // Send next value
P1OUT &= ~BIT6;
nop_delay(50); // Add time between transmissions to
} // make sure slave can keep up
*/


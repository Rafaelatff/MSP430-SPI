#include <msp430.h> 
/*#include <stdint.h> // to use uint16_t

typedef struct{ // TO TEST LATER, NO NEED OF IMPLEMENTATION
    uint16_t UCBxCTLW0; // eUSCI_Bx Control Word 0 - Address offset: 0x00
    uint16_t RESERVED1; // RESERVED - Address offset: 0x02
    uint16_t RESERVED2; // RESERVED - Address offset: 0x04
    uint16_t UCBxBRW;   // eUSCI_Bx Bit Rate Control Word - Address offset: 0x06
    uint16_t UCBxSTATW; // eUSCI_Bx Status - Address offset: 0x08
    uint16_t RESERVED3; // RESERVED - Address offset: 0x0A
    uint16_t UCBxRXBUF; // eUSCI_Bx Receive Buffer - Address offset: 0x0C
    uint16_t UCBxTXBUF; // eUSCI_Bx Transmit Buffer - Address offset: 0x0E
    uint16_t RESERVED4[13]; // RESERVED - Address offset: 0x10
    uint16_t UCBxIE;    // eUSCI_Bx Interrupt Enable - Address offset: 0x2A
    uint16_t UCBxIFG;   // eUSCI_Bx Interrupt Flag - Address offset: 0x2C
    uint16_t UCBxIV;   // eUSCI_Bx Interrupt Vector - Address offset: 0x2E
}eUSCI_Config_t; */

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// -- Setup SPI A0
	UCB1CTLW0 |= UCSWRST;           // Put B1 into SW reset mode
	// Clock configuration
	UCB1CTLW0 |= UCSSEL__SMCLK;     // Choose SMCLK (10 MHz)
	UCB1BRW = 10;                   // Set prescalar to 10 (SMCLK/10 = 100 kHz)

	UCB1CTLW0 |= UCSYNC;            // Configure B1 as synchronous mode
	UCB1CTLW0 |= UCMST;             // Put into SPI master

	// -- Configure the ports
	// MOSI = P4.4
	// MISO = P4.3 (Not used right now)
	// SCLK = P5.3
	// STE  = P5.4 (Not used)

	// To configure P4.3 and P4.4
	P4SEL1 &= ~(BIT3 | BIT4);
	P4SEL0 = (BIT3 | BIT4);

	// To configure P5.3 and P5.4
	P5SEL1 &= ~(BIT3 | BIT4);
	P5SEL0 = (BIT3 | BIT4);

	PM5CTL0 &= ~LOCKLPM5; // Turn on I/O

	UCB1CTLW0 &= ~UCSWRST;           // Take B1 out SW reset mode

	int i;
	while(1){
	    UCB1TXBUF = 0x4D;
	    for (i=0; i<1000; i++);
	}

	return 0;
}

# MSP430-SPI-Master-Tx
Learning SPI for target MSP430 with the YouTube channel  'Digital Logic &amp; Programming - 14.2(a - f)' by Brock J. LaMeres.

I am using Code Composer Studio 10.2.0.

## SPI

* MOSI (or SIMO) - Master Out, Slave In - data line from the master to the slave.
* MISO (or SOMI) - Master In, Slave Out - data line from the slave to the master.
* SS (or STE, Slave Transmit Enable) - Slave Select
* SCLK - Serial Clock - clock - eUSCI SPI Master Mode maximum of 8 MHz. It uses internal clocks: SMCLK and MODCLK.

The SPI block diagram is found in 'MSP430FR4xx and MSP430FR2xx Family User's Guide' (slau455i) [REF1].

![image](https://user-images.githubusercontent.com/58916022/212563959-39e13930-dee6-4165-9465-6038dda4e0ea.png)

There are four situations, where:

* Master TX - Master transmitts data;
* Master RX - Master receives data. Since SPI use buffer, master needs to shift the register by 8 clocks to receive the data;
* Slave TX - Slave transmitts data;
* Slave RX - Slave receives data;

Important to remember:

* Falling edge of SCLK captures the data bit.
* Tx shifts the data bit on the rising edge of SCLK.
* BUS configuration: all slaves in parallel.
* Daisy-chained configuration: work as a ring. Master sends data to Slave 1, that sends to Slave 2, that sends back to Master.

## MSP430FR2476

Let's check the SPI pins of the Launchpad board.

![image](https://user-images.githubusercontent.com/58916022/212563539-1eef87f8-1b1e-461e-bda8-be4a2712fd30.png)

## Creating a new project

![image](https://user-images.githubusercontent.com/58916022/212566736-45a3b3f5-4952-4187-906c-0be3d7aa8dbb.png)

## coding - SPI Master TX

Challenge: Continually transmit the data 0x4D at a clock rate of 100 kHz. To produce this clock in SCLK, we will use the SMCLK (10 MHz) as the clock source (BRCLK) and divide it by 10. SPI in 3-wire mode, with default frame of 8-bit abd LSB first. As seen it before, we will use the pins:

* MOSI = P4.4
* MISO = P4.3
* SCLK = P5.3

![image](https://user-images.githubusercontent.com/58916022/212565912-9ceea0d2-7615-4fec-b3fc-0397bc2c70d7.png)

To configure the SPI we need:

* Step 1: Set the USCWRST bit in the UCBxCTLW0 configuration register to put the eUSCI_Bx peripheral into sw reset.
```c
UCB1CTLW0 |= UCSWRST; // put B1 into SW reset mode
```
* Step 1.1: Configure the clock by using SMCLK [REF1].

![image](https://user-images.githubusercontent.com/58916022/212566177-4d5a0064-f14c-4b21-91b0-19e2c8997b06.png)
```c
	// Clock configuration
	UCB1CTLW0 |= UCSSEL__SMCLK;     // Choose SMCLK (10 MHz)
	UCB1BRW = 10;                   // Set prescalar to 10 (SMCLK/10 = 100 kHz) 
```
* Step 2: Initialize all eUSCI_Bx configuration registers.
* Step 2.1: Configure the UCBxCTLW0 (eUSCI_Bx Control Word 0) register.
* Step 2.1.1: Configure the UCSYNC bit to configure eUSCI_Bx as synchronous mode (SPI).

![image](https://user-images.githubusercontent.com/58916022/212566365-87a67986-c701-49df-9497-d6271249b4af.png)

![image](https://user-images.githubusercontent.com/58916022/212566514-1a968f9f-c283-428e-8254-e9a4266548d6.png)

```c
UCB1CTLW0 |= UCSYNC;            // Configure B1 as synchronous mode
```

* Step 2.1.2: Configure the UCMST (master or slave), UCMODEex (3-pin or 4-pin) and UCSTEM (behavior of STE pint) bits.

![image](https://user-images.githubusercontent.com/58916022/212567730-b6a1baec-9134-42bb-a000-cca49f53fa0e.png)
```c
UCB1CTLW0 |= UCMST;             // Put into SPI master
```

* Step 2.1.3: Configure the UCCKPH and UCCKPL bits to configure the clock edge behavior and clock polarity.

![image](https://user-images.githubusercontent.com/58916022/212567567-99f3ed07-ecbd-4336-8111-35a7921fdbe6.png)

* Step 2.1.4: Configure the data frame UCMSB e UC7BIT registers.

![image](https://user-images.githubusercontent.com/58916022/212567813-186d9a86-a69e-421f-b9ae-dddc7845e28a.png)

We didn't configure the bits that already has a default configuration that we are going to use.

* Step 3: Configure ports.

To configure the I/O pins as SPI pins, we must configure the Function Select Registers (PxSEL0, PxSEL1) of those pins. The configuration can be seen in the [REF1].

![image](https://user-images.githubusercontent.com/58916022/212563760-9263ad5e-b236-4827-af48-e3d2f16d44d0.png)

* MOSI = P4.4
* MISO = P4.3 (Not used, since we only transmit data)
* SCLK = P5.3
* STE  = P5.4 (Not used)

The information can be found in the microcontroller datasheet.

![image](https://user-images.githubusercontent.com/58916022/212568578-fe863d7a-b2c4-470a-a19f-da65b5e4e852.png)
```c
// To configure P4.3 and P4.4
P4SEL1 &= ~(BIT3 | BIT4);
P4SEL0 = (BIT3 | BIT4);

// To configure P5.3 and P5.4
P5SEL1 &= ~(BIT3 | BIT4);
P5SEL0 = (BIT3 | BIT4);

PM5CTL0 &= ~LOCKLPM5; // Turn on I/O
```
* Step 4: Clear UCSWRST to take the eUSCI_Bx peripheral out of reset.
```c
UCB1CTLW0 &= ~UCSWRST;           // Take B1 out SW reset mode
```
* Step 5: Enable interrupts (optional) in the UCAxIE configuration register.

And finally the while loop:

```c
uint16_t i;
	while(1){
	    UCB1TXBUF = 0x4D;
	    for (i=0; i<1000; i++);
	}
```

## Results

No results in the oscilloscope #sadface

## Structs for registers (TO PLAY LATER)

MSP430 are 16-bit then we are going to create the structs considering it.

```c
#include <stdint.h> // to use uint16_t

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
}eUSCI_Config_t; 
```
![image](https://user-images.githubusercontent.com/58916022/212566618-af79f01c-3ee9-4f31-a21f-cca8258bb1dc.png)




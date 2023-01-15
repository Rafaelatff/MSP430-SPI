# MSP430-SPI
Learning SPI for target MSP430 with the YouTube channel  'Digital Logic &amp; Programming - 14.2(a - f)' by Brock J. LaMeres.

I am using Code Composer Studio 10.2.0.

## SPI

* MOSI (or SIMO) - Master Out, Slave In - data line from the master to the slave.
* MISO (or SOMI) - Master In, Slave Out - data line from the slave to the master.
* SS (or STE, Slave Transmit Enable) - Slave Select
* SCLK - Serial Clock - clock - eUSCI SPI Master Mode maximum of 8 MHz. It uses internal clocks: SMCLK and MODCLK.

Important to remember:

* Falling edge of SCLK captures the data bit.
* Tx shifts the data bit on the rising edge of SCLK.
* BUS configuration: all slaves in parallel.
* Daisy-chained configuration: work as a ring. Master sends data to Slave 1, that sends to Slave 2, that sends back to Master.

## MSP430FR2476

Let's check the SPI pins of the Launchpad board.

![image](https://user-images.githubusercontent.com/58916022/212563539-1eef87f8-1b1e-461e-bda8-be4a2712fd30.png)

## coding

The SPI block diagram is found in 'MSP430FR4xx and MSP430FR2xx Family User's Guide' (slau455i) [REF1].

![image](https://user-images.githubusercontent.com/58916022/212563959-39e13930-dee6-4165-9465-6038dda4e0ea.png)

There are four situations, where:

* Master TX - Master transmitts data;
* Master RX - Master receives data. Since SPI use buffer, master needs to shift the register by 8 clocks to receive the data;
* Slave TX - Slave transmitts data;
* Slave RX - Slave receives data;

To configure the SPI we need:

* Step 1: Set the USCWRST bit in the UCBxCTLW0 configuration register to put the eUSCI_Bx peripheral into sw reset.
* Step 2: Initialize all eUSCI_Bx configuration registers.
* Step 3: Configure ports.
* Step 4: Clear UCSWRST to take the eUSCI_Bx peripheral out of reset.
* Step 5: Enable interrupts (optional) in the UCAxIE configuration register.

To configure the I/O pins as SPI pins, we must configure the Function Select Registers (PxSEL0, PxSEL1) of those pins. The configuration can be seen in the [REF1].

![image](https://user-images.githubusercontent.com/58916022/212563760-9263ad5e-b236-4827-af48-e3d2f16d44d0.png)




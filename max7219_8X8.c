// MAX 7219 8x8 RED LED Display
// Using AVR MCU (ATmega328P), displays a message on a number of cascaded 8x8 matrices.
// Message and array for LED character representations are stored in flash.
// SPI is used to send data to the matrix driver chips.
// Last Modified: April 11th 2016
// Author: Joey Conenna
// modifed: ub4raf

#include <mega328p.h>
#include <delay.h>


#define DIGITS      4
#define BRIGHTNES   0x0F    //0x00..0x0F
#define DELAY_SNAKE 2
#define DELAY_VIDEO 10000 
#define ROTATE

#if         DELAY_VIDEO<20000
    #define VIDEO_DELAY delay_us(DELAY_VIDEO); 						// Delay
#else
    #define VIDEO_DELAY delay_ms(DELAY_VIDEO/1000);
#endif

 //static 
unsigned long int seconds=24*3600-10;
char video_buffer   [DIGITS*8];

// Array holding arrays of 5 Bytes for each representation of an ASCII character, stored in flash 
flash char characters[96][5]  = { 
 {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}, // space
 {0b00000000,0b00000000,0b01001111,0b00000000,0b00000000}, // !
 {0b00000000,0b00000111,0b00000000,0b00000111,0b00000000}, // "
 {0b00010100,0b01111111,0b00010100,0b01111111,0b00010100}, // #
 {0b00100100,0b00101010,0b01111111,0b00101010,0b00010010}, // $
 {0b00100011,0b00010011,0b00001000,0b01100100,0b01100010}, // %
 {0b00110110,0b01001001,0b01010101,0b00100010,0b01010000}, // &
 {0b00000000,0b00000101,0b00000011,0b00000000,0b00000000}, // '
 {0b00000000,0b00011100,0b00100010,0b01000001,0b00000000}, // (
 {0b00000000,0b01000001,0b00100010,0b00011100,0b00000000}, // )
 {0b00010100,0b00001000,0b00111110,0b00001000,0b00010100}, // *
 {0b00001000,0b00001000,0b00111110,0b00001000,0b00001000}, // +
 {0b00000000,0b01010000,0b00110000,0b00000000,0b00000000}, // ,
 {0b00001000,0b00001000,0b00001000,0b00001000,0b00001000}, // -
 {0b00000000,0b01100000,0b01100000,0b00000000,0b00000000}, // .
 {0b00100000,0b00010000,0b00001000,0b00000100,0b00000010}, // /
 {0b00111110,0b01010001,0b01001001,0b01000101,0b00111110}, // 0
 {0b00000000,0b01000010,0b01111111,0b01000000,0b00000000}, // 1
 {0b01000010,0b01100001,0b01010001,0b01001001,0b01000110}, // 2
 {0b00100001,0b01000001,0b01000101,0b01001011,0b00110001}, // 3
 {0b00011000,0b00010100,0b00010010,0b01111111,0b00010000}, // 4
 {0b00100111,0b01000101,0b01000101,0b01000101,0b00111001}, // 5 
 {0b00111100,0b01001010,0b01001001,0b01001001,0b00110000}, // 6
 {0b00000011,0b01110001,0b00001001,0b00000101,0b00000011}, // 7
 {0b00110110,0b01001001,0b01001001,0b01001001,0b00110110}, // 8
 {0b00000110,0b01001001,0b01001001,0b00101001,0b00011110}, // 9
 {0b00000000,0b01101100,0b01101100,0b00000000,0b00000000}, // :
 {0b00000000,0b01010110,0b00110110,0b00000000,0b00000000}, // ;
 {0b00001000,0b00010100,0b00100010,0b01000001,0b00000000}, // <
 {0b00010100,0b00010100,0b00010100,0b00010100,0b00010100}, // =
 {0b00000000,0b01000001,0b00100010,0b00010100,0b00001000}, // >
 {0b00000010,0b00000001,0b01010001,0b00001001,0b00000110}, // ?
 {0b00110010,0b01001001,0b01111001,0b01000001,0b00111110}, // @
 {0b01111110,0b00010001,0b00010001,0b00010001,0b01111110}, // A
 {0b01111111,0b01001001,0b01001001,0b01001001,0b00111110}, // B
 {0b00111110,0b01000001,0b01000001,0b01000001,0b00100010}, // C
 {0b01111111,0b01000001,0b01000001,0b01000001,0b00111110}, // D
 {0b01111111,0b01001001,0b01001001,0b01001001,0b01001001}, // E
 {0b01111111,0b00001001,0b00001001,0b00001001,0b00000001}, // F
 {0b00111110,0b01000001,0b01001001,0b01001001,0b00111010}, // G
 {0b01111111,0b00001000,0b00001000,0b00001000,0b01111111}, // H
 {0b01000001,0b01000001,0b01111111,0b01000001,0b01000001}, // I
 {0b00110000,0b01000001,0b01000001,0b00111111,0b00000001}, // J
 {0b01111111,0b00001000,0b00010100,0b00100010,0b01000001}, // K
 {0b01111111,0b01000000,0b01000000,0b01000000,0b01000000}, // L
 {0b01111111,0b00000010,0b00001100,0b00000010,0b01111111}, // M
 {0b01111111,0b00000100,0b00001000,0b00010000,0b01111111}, // N
 {0b00111110,0b01000001,0b01000001,0b01000001,0b00111110}, // O
 {0b01111111,0b00001001,0b00001001,0b00001001,0b00000110}, // P
 {0b00111110,0b01000001,0b01010001,0b00100001,0b01011110}, // Q
 {0b01111111,0b00001001,0b00001001,0b00011001,0b01100110}, // R
 {0b01000110,0b01001001,0b01001001,0b01001001,0b00110001}, // S 
 {0b00000001,0b00000001,0b01111111,0b00000001,0b00000001}, // T
 {0b00111111,0b01000000,0b01000000,0b01000000,0b00111111}, // U
 {0b00001111,0b00110000,0b01000000,0b00110000,0b00001111}, // V
 {0b00111111,0b01000000,0b00111000,0b01000000,0b00111111}, // W
 {0b01100011,0b00010100,0b00001000,0b00010100,0b01100011}, // X 
 {0b00000011,0b00000100,0b01111000,0b00000100,0b00000011}, // Y
 {0b01100001,0b01010001,0b01001001,0b01000101,0b01000011}, // Z
 {0b01111111,0b01000001,0b01000001,0b00000000,0b00000000}, // [
 {0b00000010,0b00000100,0b00001000,0b00010000,0b00100000}, // '\'
 {0b00000000,0b00000000,0b01000001,0b01000001,0b01111111}, // ]
 {0b00000100,0b00000010,0b00000001,0b00000010,0b00000100}, // ^
 {0b01000000,0b01000000,0b01000000,0b01000000,0b01000000}, // _
 {0b00000000,0b00000001,0b00000010,0b00000100,0b00000000}, // `
 {0b00100000,0b01010100,0b01010100,0b01010100,0b01111000}, // a
 {0b01111111,0b01001000,0b01000100,0b01000100,0b00111000}, // b
 {0b00111000,0b01000100,0b01000100,0b01000100,0b00100000}, // c
 {0b00111000,0b01000100,0b01000100,0b01001000,0b01111111}, // d
 {0b00111000,0b01010100,0b01010100,0b01010100,0b00011000}, // e
 {0b00001000,0b01111110,0b00001001,0b00000001,0b00000010}, // f
 {0b00001100,0b01010010,0b01010010,0b01010010,0b00111110}, // g
 {0b01111111,0b00001000,0b00000100,0b00000100,0b01111000}, // h
 {0b00000000,0b01000100,0b01111101,0b01000000,0b00000000}, // i
 {0b00100000,0b01000000,0b01000100,0b00111101,0b00000000}, // j
 {0b01111111,0b00010000,0b00101000,0b01000100,0b00000000}, // k
 {0b00000000,0b01000001,0b01111111,0b01000000,0b00000000}, // l
 {0b01111000,0b00000100,0b00001000,0b00000100,0b01111000}, // m
 {0b01111100,0b00001000,0b00000100,0b00000100,0b01111000}, // n
 {0b00111000,0b01000100,0b01000100,0b01000100,0b00111000}, // o
 {0b01111100,0b00010100,0b00010100,0b00010100,0b00001000}, // p
 {0b00001000,0b00010100,0b00010100,0b01111100,0b00000000}, // q
 {0b01111100,0b00001000,0b00000100,0b00000100,0b00001000}, // r
 {0b01001000,0b01010100,0b01010100,0b01010100,0b00100000}, // s
 {0b00000100,0b00111111,0b01000100,0b01000000,0b00100000}, // t
 {0b00111100,0b01000000,0b01000000,0b00100000,0b01111100}, // u
 {0b00011100,0b00100000,0b01000000,0b00100000,0b00011100}, // v
 {0b00111100,0b01000000,0b00110000,0b01000000,0b00111100}, // w
 {0b01000100,0b00101000,0b00010000,0b00101000,0b01000100}, // x
 {0b00001100,0b01010000,0b01010000,0b01010000,0b00111100}, // y
 {0b01000100,0b01100100,0b01010100,0b01001100,0b01000100}, // z          
 {0b00000000,0b00001000,0b00110110,0b01000001,0b00000000}, // {
 {0b00000000,0b00000000,0b01111111,0b00000000,0b00000000}, // |
 {0b00000000,0b01000001,0b00110110,0b00001000,0b00000000}, // }
 {0b00001000,0b00000100,0b00000100,0b00001000,0b00000100} // ~    
}; // characters[95]
/*flash char dig5x7 [10][5] = {       //// it is copy paste from main characters fonts table
 {0b00111110,0b01010001,0b01001001,0b01000101,0b00111110}, // 0
 {0b00000000,0b01000010,0b01111111,0b01000000,0b00000000}, // 1
 {0b01000010,0b01100001,0b01010001,0b01001001,0b01000110}, // 2
 {0b00100001,0b01000001,0b01000101,0b01001011,0b00110001}, // 3
 {0b00011000,0b00010100,0b00010010,0b01111111,0b00010000}, // 4
 {0b00100111,0b01000101,0b01000101,0b01000101,0b00111001}, // 5 
 {0b00111100,0b01001010,0b01001001,0b01001001,0b00110000}, // 6
 {0b00000011,0b01110001,0b00001001,0b00000101,0b00000011}, // 7
 {0b00110110,0b01001001,0b01001001,0b01001001,0b00110110}, // 8
 {0b00000110,0b01001001,0b01001001,0b00101001,0b00011110}};// 9  
 */
flash char dig5x8 [10][5] = {
 {0b01111110,0b10010001,0b10001001,0b10000101,0b01111110}, // 0 /
 {0b00000010,0b10000010,0b11111111,0b10000000,0b11000000}, // 1 /
 {0b11000010,0b11100001,0b10010001,0b10001001,0b10000110}, // 2  
 {0b01000001,0b10000001,0b10001001,0b10011111,0b01110001}, // 3   /
 {0b00011000,0b00010100,0b00010010,0b11111111,0b00010000}, // 4
 {0b01000111,0b10000101,0b10000101,0b01000101,0b00111001}, // 5 
 {0b01111100,0b10001010,0b10001001,0b10001001,0b01110001}, // 6
 {0b00000011,0b11110001,0b00001001,0b00000101,0b00000011}, // 7    /
 {0b01110110,0b10001011,0b10001001,0b10001011,0b01110110}, // 8   
 {0b00000110,0b10001001,0b10001001,0b01001001,0b00111110}};// 9 
 
flash char dig4x8 [10][4] = {
 {0b01111110,0b10010001,0b10001001,0b01111110}, // 0 /
 {0b00000000,0b10000010,0b11111111,0b10000000}, // 1 /
 {0b11000010,0b10100001,0b10010001,0b10001110}, // 2 /
 {0b01000001,0b10000001,0b10000101,0b01111011}, // 3 /
 {0b00011000,0b00010100,0b00010010,0b11111111}, // 4 /
 {0b01000111,0b10001001,0b10001001,0b01110001}, // 5 
 {0b01111100,0b10010010,0b10010001,0b01110001}, // 6
 {0b00000011,0b11110001,0b00001001,0b00000111}, // 7 /
 {0b01110110,0b10001001,0b10001001,0b01110110}, // 8  /
 {0b01000110,0b10001001,0b10001001,0b01111110}};// 9 
flash char dig3x6 [10][3] = {
 {0b01111000,0b10000100,0b01111000}, // 0
 {0b10001000,0b11111100,0b10000000}, // 1
 {0b11100100,0b10100100,0b10011000}, // 2
 {0b10000100,0b10010100,0b01111000}, // 3
 {0b00111000,0b00100000,0b11111100}, // 4
 {0b10011100,0b10010100,0b01100100}, // 5 
 {0b01111000,0b10010100,0b01100100}, // 6
 {0b00000100,0b11100100,0b00011100}, // 7
 {0b11101000,0b10010100,0b11101000}, // 8
 {0b10011000,0b10010100,0b01111000}};// 9 

flash char message[]=" ~ ~~~  ub4raf  ~~~ ~          ";

void send_spi   (char );
void send_7219  (char ,char*);
void send_time  (void );
void send_all_7219 (char, char);
void send_dig_7219 (char, char, char);

void initBuffer(void);
void pushBuffer(char);
void displayMessage(flash char *, char);
void sendMessage(char *, char);
void displayBuffer(void);

void pushCharacter(char);

void pushDig48(char);
void pushDig36(char);
void pushDig58(char);

char state=0;





#ifndef RXB8
#define RXB8 1
#endif

#ifndef TXB8
#define TXB8 0
#endif

#ifndef UPE
#define UPE 2
#endif

#ifndef DOR
#define DOR 3
#endif

#ifndef FE
#define FE 4
#endif

#ifndef UDRE
#define UDRE 5
#endif

#ifndef RXC
#define RXC 7
#endif

#define DATA_REGISTER_EMPTY (1<<UDRE)

// USART Transmitter buffer
#define TX_BUFFER_SIZE0 256
char tx_buffer0[TX_BUFFER_SIZE0];

#if TX_BUFFER_SIZE0 <= 256
unsigned char tx_wr_index0,tx_rd_index0,tx_counter0;
#else
unsigned int tx_wr_index0,tx_rd_index0,tx_counter0;
#endif

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
#if TX_BUFFER_SIZE0 != 256
   if (tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
#if TX_BUFFER_SIZE0 != 256
   if (tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
#endif
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
// Reinitialize Timer 0 value
TCNT0=0x88;
// Place your code here
 #warning TODO: interrupt based time counter
}

// SPI interrupt service routine
interrupt [SPI_STC] void spi_isr(void)
{
unsigned char data;
data=SPDR;
// Place your code here
#warning TODO: interrupt based SPI sending
}

// Pin change 16-23 interrupt service routine
interrupt [PC_INT2] void pin_change_isr2(void)
{
// Place your code here
delay_ms(200);
if(  (~(PIND&0xFC) )&0xFC)
    {   
        //putchar('!');
        switch ( (~(PIND&0xFC) )&0xFC )
            {
                case 1<<2:
                    seconds+=60;
                    break; 
                case 1<<3:
                    seconds-=60;
                    if(seconds>=24*60*60)seconds+=24*60*60;
                    break;
                case 1<<4:
                    seconds+=3600;
                    break;
                case 1<<5:
                    seconds-=3600;
                    if(seconds>=24*60*60)seconds+=24*60*60;
                    break;
                case 1<<6:
                    seconds=0;
                    break;
                case 1<<7:
                    state++;
                    switch (state)
                        {
                            case 0:
                            case 2:
                                send_all_7219(0x0F,0x00);               
                                delay_ms(1);
                                send_all_7219(0x0C,0x01);               
                                delay_ms(1);
                                break;
                            case 4:
                                send_all_7219(0x0F,0x00);               
                                delay_ms(1);
                                send_all_7219(0x0C,0x01);               
                                delay_ms(1);
                                state=0;
                                break;
                            case 1:
                                send_all_7219(0x0F,0x01);               
                                delay_ms(1);
                                send_all_7219(0x0C,0x01);               
                                delay_ms(1);
                                break;
                            case 3:
                                send_all_7219(0x0F,0x00);               
                                delay_ms(1);
                                send_all_7219(0x0C,0x00);               
                                delay_ms(1);
                                break;
                        }    
                    break;            
            }
            
    }
}


        
void main(void)
{
// Declare your local variables here
signed char i,j,k;

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port B initialization
// Func7=In Func6=In Func5=Out Func4=In Func3=Out Func2=Out Func1=In Func0=In 
// State7=T State6=T State5=0 State4=T State3=0 State2=0 State1=T State0=T 
PORTB=0x00;
DDRB=0x2C;

// Port C initialization
// Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0xFC;
DDRD =0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 15,625 kHz
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
TCCR0A=0x00;
TCCR0B=0x05;
TCNT0=0x88;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2A output: Disconnected
// OC2B output: Disconnected
ASSR=0x00;
TCCR2A=0x00;
TCCR2B=0x00;
TCNT2=0x00;
OCR2A=0x00;
OCR2B=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// Interrupt on any change on pins PCINT0-7: Off
// Interrupt on any change on pins PCINT8-14: Off
// Interrupt on any change on pins PCINT16-23: On
EICRA=0x00;
EIMSK=0x00;
PCICR=0x04;
PCMSK2=0xFC;
PCIFR=0x04;

// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=0x01;

// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=0x00;

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART0 Mode: Asynchronous
// USART Baud Rate: 1200
UCSR0A=0x00;
UCSR0B=0x48;
UCSR0C=0x06;
UBRR0H=0x03;
UBRR0L=0x40;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
ADCSRB=0x00;
DIDR1=0x00;

// ADC initialization
// ADC disabled
ADCSRA=0x00;

/*
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 250,000 kHz
// SPI Clock Phase: Cycle Start
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0xD2;
SPSR=0x00;

// Clear the SPI interrupt flag
#asm
    in   r30,spsr
    in   r30,spdr
#endasm
 */
 
    DDRB |= ((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //ножки SPI на выход
	PORTB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //низкий уровень
	SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR0)| (1<<SPR1);
// TWI initialization
// TWI disabled
TWCR=0x00;

// Global enable interrupts
#asm("sei")
/*
  PB5-LED(d13 in arduino/SCK
  PB4 MISO // not used
  PB3 MOSI
  PB2 SS
*/

delay_ms(100);
puts("hello watch\r");

delay_ms(10);
send_all_7219(0x0F,0x00);               
delay_ms(10);

send_all_7219(0x0C,0x01);               
delay_ms(10);

send_all_7219(0x0B,0x07);               
delay_ms(10);

send_all_7219(0x09,0x00);               
delay_ms(10);

send_all_7219(0x0A,0x00);               
delay_ms(10);
puts("initialized\r");
delay_ms(50);

     for(i=0;i<8;i++)
        {              
            send_all_7219(i+1,0x00);
            delay_ms(10); 
        }  
        
send_all_7219(0x0A,0x0F);               
delay_ms(10);                      
puts("erased\n"); 
delay_ms(50);
for(k=0;k<4;k++)    //snake effect
{
    for(j=0;j<4;j++)
    {
      for(i=j;i<7-j;i++)
        {              
            send_dig_7219(i+1,1<<j,k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(i+1,0,k);
        }   
        
      for(i=j;i<7-j;i++)
        {              
            send_dig_7219(8-j,1<<i,k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(8-j,0,k);
        } 
        
      for(i=j;i<7-j;i++)
        {              
            send_dig_7219(8-i,1<<7-j,k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(8-i,0x00,k);
        }     
      for(i=j;i<7-j;i++)
        {              
            send_dig_7219(1+j,1<<(7-i),k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(1+j,0,k);
        } 
    }  
    for(j=3;j>=0;j--)
    {
      for(i=j+1;i<7-j;i++)
        {              
            send_dig_7219(i+1,1<<j,k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(i+1,0,k);
        }   
        
      for(i=j;i<7-j;i++)
        {              
            send_dig_7219(8-j,1<<i,k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(8-j,0,k);
        } 
        
      for(i=j;i<7-j;i++)
        {              
            send_dig_7219(8-i,1<<7-j,k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(8-i,0x00,k);
        }     
      for(i=j;i<8-j;i++)
        {              
            send_dig_7219(1+j,1<<(7-i),k);
            delay_ms(DELAY_SNAKE*j+3*DELAY_SNAKE);
            send_dig_7219(1+j,0,k);
        } 
    } 
 }  
 send_all_7219(0x0A,BRIGHTNES); 
 for(i=0;i<8;i++)
        {              
            send_all_7219(i+1,0x00);
            delay_ms(10); 
        }                
 delay_ms(10);
 initBuffer();
 
 displayMessage(message,sizeof(message)-1);
while (1)

      { 
      delay_ms(1000); 
      send_time();      
      seconds+=1;
      if(seconds>=24*60*60)seconds=0;
      }
}

void send_time (void)
    {
        char time[7];
        char line[16];
        time[0]=seconds%10;
        time[1]=(seconds/10)%6;
        time[2]=(seconds/60)%10;
        time[3]=(seconds/600)%6;
        time[4]=(seconds/3600)%10;
        time[5]=(seconds/36000)%6;
        time[6]=0;
        sprintf(line,"%d%d:%d%d:%d%d",time[5],time[4],time[3],time[2],time[1],time[0]);
        printf(" %s =%d\n\r",line,seconds);
        initBuffer();
        //sendMessage(line+3,5);
              
        
        if(time[5])
            pushDig48(time[5]);
        else
            {
                pushBuffer(0x00);pushBuffer(0x00);
                pushBuffer(0x00);pushBuffer(0x00);
            } 
        pushBuffer(0x00);
        pushDig48(time[4]);    
        pushBuffer(0x00);
        if(seconds%2)     
            pushBuffer(0x24);
        else
            pushBuffer(0x00);
        pushBuffer(0x00);
        pushDig58(time[3]); 
        pushBuffer(0x00);
        pushDig58(time[2]);
        pushBuffer(0x00);
        pushBuffer(0x00);
        pushDig36(time[1]);           
        pushBuffer(0x00);
        pushDig36(time[0]); 
        displayBuffer();
    }

void send_spi   (char data)
    {
        SPDR = data;
        while (!(SPSR & (1 << SPIF)));
    }
void send_7219  (char length,   char* data)
    {   
        char i;
        PORTB &= ~(1<<PORTB2);
        for(i=0;i<length;i++)
            send_spi(data[i]);
        PORTB |= (1<<PORTB2);
    }     
    
void send_all_7219 (char command, char data)
    {
        char line [2*DIGITS];
        char i;
            for (i=0;i<DIGITS;i++)
                {
                    line[2*i]  =command;
                    line[2*i+1]=data;
                } 
        send_7219 (2*DIGITS,line);   
    }

void send_dig_7219 (char command, char data, char digit)
    {
        char line [2*DIGITS];
        char i;
            for (i=0;i<DIGITS;i++)
                {
                    line[2*i]  =0x00;
                    line[2*i+1]=0x00;
                }
        line [2*digit]   =command; 
        line [2*digit+1] =data;
        send_7219 (2*DIGITS,line);   
    }             
    
void initBuffer(void)
    {   
        char i;
        for(i = 0; i < DIGITS*8; i++)
		video_buffer[i] = 0x00;
    }
void pushBuffer(char x)
    {    
        char i;
        for(i = 0; i < DIGITS*8 - 1; i++)
		    video_buffer[i] = video_buffer[i+1];	
	    video_buffer[DIGITS*8 - 1] = x;
    }
void pushCharacter(char c)
    {     
        char j;
        for(j = 0; j < 5; j++)				// For 5 bytes representing each character
		    {
                pushBuffer(characters[c][j]);   // Push the byte from the characters array to the display buffer
                displayBuffer();
                VIDEO_DELAY;   // always use push one column + display + delay.
		    }
    }  
    

void pushDig48(char c)
    {     
        char j;
        for(j = 0; j < 4; j++)				// For 5 bytes representing each character
		    {
                pushBuffer(dig4x8[c][j]);   // Push the byte from the characters array to the display buffer
		    }
    } 
void pushDig36(char c)
    {     
        char j;
        for(j = 0; j < 3; j++)				// For 5 bytes representing each character
		    {
                pushBuffer(dig3x6[c][j]);   // Push the byte from the characters array to the display buffer
		    }
    }  
void pushDig58(char c)
    {     
        char j;
        for(j = 0; j < 5; j++)				// For 5 bytes representing each character
		    {
                pushBuffer(dig5x8[c][j]);   // Push the byte from the characters array to the display buffer
		    }
    } 

void displayMessage(flash char* arrayPointer, char arraySize)
    {   
        char i,c;
        for(i = 0; i < arraySize; i++)
            {  
                c= *(arrayPointer + i) - 32;
                pushCharacter(c);	// Send converted ASCII value of character in message to index in characters array (-32 sends corrent index to characters array)
                pushBuffer(0x00);						// Add empty column after character for letter spacing
                displayBuffer();						// Display & send
                VIDEO_DELAY;  // always use push one column + display + delay.
            }
    }     
    
void sendMessage(char* arrayPointer, char arraySize)
    {   
        char i;
        for(i = 0; i < arraySize; i++)
            {
                pushCharacter(*(arrayPointer + i) - 32);	// Send converted ASCII value of character in message to index in characters array (-32 sends corrent index to characters array)
                pushBuffer(0x00);						// Add empty column after character for letter spacing                
                //delay_us(5); 						// withou delay
            } 
        displayBuffer();						// Display &
    }
void displayBuffer(void)
    {   
        char i,j; 
    #ifdef ROTATE
    char k;    
    char transposed_buffer[DIGITS*8];
        for(k=0;k<DIGITS*8;k++)
              transposed_buffer[k]=0;
    for(k=0;k<DIGITS;k++)
      for(i=0;i<8;i++)// transpose of bit matrix:
        for(j=0;j<8;j++)
            transposed_buffer[j+k*8]|=(1&(video_buffer[i+k*8]>>j))<<i;
    #endif
       for( i = 0; i < DIGITS; i++) // For each cascaded device
           {
               for( j = 1; j < 9; j++) // For each column
               {
                    #ifdef ROTATE 
                        send_dig_7219(9-j, transposed_buffer[j + i*8 - 1],DIGITS-i-1);                    
                    #else 
                        send_dig_7219(j, video_buffer[j + i*8 - 1],i);
                    #endif
               }
           }
    }

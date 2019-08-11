# AVR_Scrolling_Text_Led_Matrix
AVR 8X8 Red LED Scrolling Text Display

Code Vision compiler
mega328p chip (chinese arduino)

Simple watch.
JUST AN EXAMPLE, not realy time mesarument watch!

Number of dot matrixes:
#define DIGITS      4

Software brightnes:
#define BRIGHTNES   0x0F    //0x00..0x0F

Snake video effect velocity:
#define DELAY_SNAKE 2

Scrolling velocity:
#define DELAY_VIDEO 10000 

90 degree rotation of matrix 
#define ROTATE

put byte to SPI transmitter
void send_spi   (char );

send to cascade of max7219 array data with length byte
void send_7219  (char length, char* data);

void send_time  (void );

send to all of max7219 the same command and data bytes
void send_all_7219 (char, char);

send command and data byte to the max7219 with number digit
void send_dig_7219 (char command, char data, char digit);

erase video buffer
void initBuffer(void);

add to buffer one 8pixel height column
void pushBuffer(char);

display scrolling string from flash, with length.
void displayMessage(flash char *, char);

just display string, without scrolling:
void sendMessage(char *, char);

low level send all buffer to cascade of max7219
void displayBuffer(void);

add to buffer character with width 5 pixel
void pushCharacter(char);

add to buffer digits with different size of width*height pixels
void pushDig48(char);
void pushDig36(char);
void pushDig58(char);


// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

// Modified for SPFD5408 Library by Joao Lopes
// Version 0.9.2 - Rotation for Mega and screen initial

#include <Arduino.h>
// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
// #include <SPFD5408_TouchScreen.h>
#include <SPI.h>
// #include <SD.h>


uint8_t sin_table[100]=
{

0x32,0x35,0x38,0x3b,0x3e,0x41,0x44,0x47,0x4a,0x4d,0x4f,0x52,0x54,0x56,0x59,0x5a,0x5c,0x5e,0x5f,0x60,
0x62,0x62,0x63,0x64,0x64,0x64,0x64,0x64,0x63,0x62,0x62,0x60,0x5f,0x5e,0x5c,0x5a,0x59,0x56,0x54,0x52,
0x4f,0x4d,0x4a,0x47,0x44,0x41,0x3e,0x3b,0x38,0x35,0x32,0x2f,0x2c,0x29,0x26,0x23,0x20,0x1d,0x1a,0x17,
0x15,0x12,0x10,0xe,0xb,0xa,0x8,0x6,0x5,0x4,0x2,0x2,0x1,0x0,0x0,0x0,0x0,0x0,0x1,0x2,
0x2,0x4,0x5,0x6,0x8,0xa,0xb,0xe,0x10,0x12,0x15,0x17,0x1a,0x1d,0x20,0x23,0x26,0x29,0x2c,0x2f


};

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// TftSpfd5408 tft;

void DrawSineWave(uint16_t x , uint16_t y , uint16_t Color)
{
  uint16_t SineX = 0;
  for(int counter = x; counter < 320; counter++)
  {
    tft.drawPixel(counter , sin_table[SineX++]+y  , Color);
    if(SineX == 100)  
    {
      SineX = 0;
    }
    
  //  delay(10);
  }
  
}
uint16_t * SampleSignal(void)
{
  static uint16_t Voltage[2700] = {0};
  for(int counter = 0; counter< 2700; counter++)
  { 
      Voltage[counter] = analogRead(A8);
      delay(1);
  }
  return Voltage;
}
uint16_t j = 0;

void DrawSignal(uint16_t x1, uint16_t  y1, uint16_t x2, uint16_t  y2, uint16_t Color)
{
  static uint16_t Amplitude[2] = {y1};
  static uint16_t X_SCALE = 1 , Y_SCALE = 1;  
    uint16_t * ptr = SampleSignal();
  for(int counter = x1; counter < x2-1; counter++)
  {
     Amplitude[1] =  map(ptr[(counter-x1)*X_SCALE] , 0 , 4095 , y1 , y2);
     tft.drawLine(counter, Amplitude[0], counter+1, Amplitude[1], Color );
     Amplitude[0] = Amplitude[1];
    //X_SCALE = (uint32_t)map( analogRead(A9) , 0 , 4095 , 1 , 1000000);
    // Serial.println("Enetr Scale");
    if(Serial.available() > 0)
    {
     j =  Serial.parseInt();
     if( j > 0)
     {
       X_SCALE = j;
     }
    }
    //X_SCALE = (uint16_t)map( analogRead(A8) , 1 , 100 , y1 , y2);
    // Amplitude[1] = (uint16_t)map( analogRead(A8) , 0 , 4095 , y1 , y2);
    //  tft.drawLine(counter, Amplitude[0], counter+1, Amplitude[1], Color );
    // Amplitude[0] = Amplitude[1];
    //delay(5);
  }
}


void Set_BackGroundText(void)
{
  tft.setCursor(7, 10);
  tft.println("CH1"); 
  tft.setCursor(7, 20);
  tft.println("Vpp");
  tft.setCursor(7, 30); 
  tft.println("3.3V");

  tft.setCursor(7, 70);
  tft.println("CH1"); 
  tft.setCursor(7, 80);
  tft.println("Freq");
  tft.setCursor(7, 90); 
  tft.println("50Hz");

  tft.setCursor(7, 130);
  tft.println("CH1"); 
  tft.setCursor(7, 140);
  tft.println("Period");
  tft.setCursor(7, 150); 
  tft.println("1.98ms");

  tft.setCursor(7, 190);
  tft.println("CH1"); 
  tft.setCursor(7, 200);
  tft.println ("+Duty");
  tft.setCursor(7, 210); 
  tft.println("49.2%");

  tft.drawFastVLine(185, 0, 240, RED);
  tft.drawFastHLine(50, 120, 320, RED);
  tft.drawRect(0 , 0 , 320 , 240 , MAGENTA);
}
void SetBackGround(uint16_t color) {
 uint16_t divisionFactor = 60;

 tft.fillRect( 50, 0, 270 , 240 , BLACK);

  for(uint16_t y=0; y<240; y+=10) 
  {
    if(y == divisionFactor)
    {
      divisionFactor += 60;
      tft.drawFastHLine(0, y, 320, color);
    }
    else 
      tft.drawFastHLine(50, y, 320, color);
  }
  for(uint16_t x=50; x<320; x+=10) 
  {
      tft.drawFastVLine(x, 0, 240, color);
  }
 // Set_BackGroundText();
  tft.drawFastVLine(185, 0, 240, RED);
  tft.drawFastHLine(50, 120, 320, RED);
  tft.drawRect(0 , 0 , 320 , 240 , MAGENTA);
}

void SetBackGround2(uint16_t color) {
 uint16_t divisionFactor = 60;

 tft.fillRect( 50, 60, 270 , 100 , BLACK);

  for(uint16_t y=0; y<240; y+=10) 
  {
    if(y == divisionFactor)
    {
      divisionFactor += 60;
       tft.drawFastHLine(0, y, 320, color);
    }
    else 
    tft.drawFastHLine(50, y, 320, color);
  }
  for(uint16_t x=50; x<320; x+=10) 
  {
    tft.drawFastVLine(x, 0, 240, color);
  }


  tft.drawFastVLine(185, 0, 240, RED);
  tft.drawFastHLine(50, 120, 320, RED);
  tft.drawRect(0 , 0 , 320 , 240 , MAGENTA);
}



void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  analogReadResolution(12);  

  tft.reset();
  uint16_t id;
  id = tft.readID();
  tft.begin(id);
  //Serial.println(F("Benchmark                Time (microseconds)"));
  
  tft.setRotation(3);
  tft.setTextColor(GREEN); 
  tft.setTextSize(1);
  tft.println("HELLo WORLD");
  tft.fillScreen(BLACK);
  
  SetBackGround(BLUE);
  Set_BackGroundText();

}


void loop(void) {
  /**
   * Arduino DUE (write8 as Function) takes 134.198 ms to set BackGround
   * Arduino DUE (write8 as Macro) takes 142.44 ms to set BackGround
   * Arduino DUE (write8 as Function without PIO_SET and CLR) takes 71.695 ms to set BackGround
   * Arduino DUE (write8 as Macros without PIO_SET and CLR) takes 78.163 ms to set BackGround
   * Arduino DUE (write8 with bitwise) takes 64.961 ms to set BackGround
   * Arduino DUE test1,2,3 takes 64.957 ms to set BackGround
   * Arduino UNO takes 253.532 ms to set BackGround
   */
 
 // DrawSineWave( 50  , 60 , YELLOW);
  DrawSignal( 50 , 120 , 319 ,  60, YELLOW);
  delay(10);
 
}
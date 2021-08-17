
#include <Adafruit_GFX.h>        
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "MAX30105.h"           
#include "heartRate.h" 
//#include <EEPROM.h>     

MAX30105 particleSensor;

const byte RATE_SIZE = 4; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0; 
float beatsPerMinute;
int beatAvg;
//int temp;
//int x = 1;

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET    -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

static const unsigned char PROGMEM logo2_bmp[] =
{ 0x03, 0xC0, 0xF0, 0x06, 0x71, 0x8C, 0x0C, 0x1B, 0x06, 0x18, 0x0E, 0x02, 0x10, 0x0C, 0x03, 0x10,             
0x04, 0x01, 0x10, 0x04, 0x01, 0x10, 0x40, 0x01, 0x10, 0x40, 0x01, 0x10, 0xC0, 0x03, 0x08, 0x88,
0x02, 0x08, 0xB8, 0x04, 0xFF, 0x37, 0x08, 0x01, 0x30, 0x18, 0x01, 0x90, 0x30, 0x00, 0xC0, 0x60,
0x00, 0x60, 0xC0, 0x00, 0x31, 0x80, 0x00, 0x1B, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x04, 0x00,  };

static const unsigned char PROGMEM logo3_bmp[] =
{ 0x01, 0xF0, 0x0F, 0x80, 0x06, 0x1C, 0x38, 0x60, 0x18, 0x06, 0x60, 0x18, 0x10, 0x01, 0x80, 0x08,
0x20, 0x01, 0x80, 0x04, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0xC0, 0x00, 0x08, 0x03,
0x80, 0x00, 0x08, 0x01, 0x80, 0x00, 0x18, 0x01, 0x80, 0x00, 0x1C, 0x01, 0x80, 0x00, 0x14, 0x00,
0x80, 0x00, 0x14, 0x00, 0x80, 0x00, 0x14, 0x00, 0x40, 0x10, 0x12, 0x00, 0x40, 0x10, 0x12, 0x00,
0x7E, 0x1F, 0x23, 0xFE, 0x03, 0x31, 0xA0, 0x04, 0x01, 0xA0, 0xA0, 0x0C, 0x00, 0xA0, 0xA0, 0x08,
0x00, 0x60, 0xE0, 0x10, 0x00, 0x20, 0x60, 0x20, 0x06, 0x00, 0x40, 0x60, 0x03, 0x00, 0x40, 0xC0,
0x01, 0x80, 0x01, 0x80, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x30, 0x0C, 0x00,
0x00, 0x08, 0x10, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x01, 0x80, 0x00  };


void setup() {  
  //Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.display();
  delay(3000);

  //temp=EEPROM.read(6);
  particleSensor.begin(Wire, I2C_SPEED_FAST); 
  particleSensor.setup(); 
  //particleSensor.enableDIETEMPRDY();
  
  
  particleSensor.setPulseAmplitudeRed(0x0A); 

}

void loop() {
 long irValue = particleSensor.getIR();
 //int temp = particleSensor.readTemperature();
 
   
                                           
if(irValue > 7000){ 

    //temperat=temp;
    //EEPROM.write(6,temperat);                                       
    display.clearDisplay();                                   
    display.drawBitmap(5, 5, logo2_bmp, 24, 21, WHITE);       
    display.setTextSize(2);                                   
    display.setTextColor(WHITE); 
    display.setCursor(70,0);                
    display.println("BPM");             
    display.setCursor(80,18);                
    display.println(beatAvg);
    //display.setCursor(55,10);
    //display.println(temp);
      
    display.display();
    
 
  if (checkForBeat(irValue) == true)                        
  {
    display.clearDisplay();                                
    display.drawBitmap(0, 0, logo3_bmp, 32, 32, WHITE);    
    display.setTextSize(2);                                
    display.setTextColor(WHITE);             
    display.setCursor(70,0);                
    display.println("BPM");             
    display.setCursor(80,18);                
    display.println(beatAvg);
    display.setCursor(55,10);
    //display.println(temp);
    display.display();
    
                                         
    //We sensed a beat!
    long delta = millis() - lastBeat; 
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);           

    if (beatsPerMinute < 255 && beatsPerMinute > 20)               
    {
      rates[rateSpot++] = (byte)beatsPerMinute; 
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
   // Serial.println("Temperaturer in C* =");
   // Serial.print(temp, 4); 
  }
  

}

  
  
  
  
  if (irValue < 7000){       
     beatAvg=0;
     display.clearDisplay();
     display.setTextSize(1);                    
     display.setTextColor(WHITE);             
     display.setCursor(10,5);                
     display.println("Please Place "); 
     display.setCursor(10,15);
     display.println("your finger");  
     display.setCursor(10,25);
     display.println("on sensor");
     display.setCursor(100,15);
     display.println("* *");
     display.setCursor(105,25);
      display.println("-");
     display.display();

     }

}

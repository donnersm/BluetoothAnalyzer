#include "I2SClocklessLedDriver.h"

#define NUM_LEDS_PER_STRIP 256
#define NUMSTRIPS 16
//here we have 4 colors per pixel
uint8_t leds[NUMSTRIPS*NUM_LEDS_PER_STRIP*4];

int pins[16]={0,2,4,5,12,13,14,15,16,18,19,21,22,23,25,26};

I2SClocklessLedDriver driver;
void setup() {
    Serial.begin(115200);
    
    driver.initled(leds,pins,NUMSTRIPS,NUM_LEDS_PER_STRIP,ORDER_GRBW);
    driver.setBrightness(10);
    
}

int off=0;
long time1,time2,time3;
void loop() {
    time1=ESP.getCycleCount();
    for(int j=0;j<NUMSTRIPS;j++)
    {
        
        for(int i=0;i<NUM_LEDS_PER_STRIP;i++)
        {
            
            driver.setPixel((i+off)%NUM_LEDS_PER_STRIP+NUM_LEDS_PER_STRIP*j,255-i,i,((128-i)+255)%255,i/25);
            
        }
    }
    time2=ESP.getCycleCount();
    driver.showPixels();
    time3=ESP.getCycleCount();
    Serial.printf("Calcul pixel fps:%.2f   showPixels fps:%.2f   Total fps:%.2f \n",(float)240000000/(time2-time1),(float)240000000/(time3-time2),(float)240000000/(time3-time1));
    off++;
    
}
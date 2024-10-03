# I2SClocklessLedDriver for esp32
## Introduction
### What kind of led do you want to drive
This library is a new take on driving ws2812 leds with I2S on an esp32. It allows to drive up to 16 strips leds in parallel of  
* RGB:
    * WS2812,
    * WS2813,
    * WS2815 
* RGBW 
    * SK6812. 

If you are using RGB led type then this library is fully compatible with fastLED library (in which you cand find my previous version of  my I2S driver).

### Why have I rewritten the library ?
I have rewritten the library out of the FastLED framework  to allow easier testing but also create a pixel pusher independant of the library you want to use. Once totally done I will certainly re-merge it with the FasLED library.

But the main reason is the way I wanted to drive the leds. Indeed the library gives you two more options that can prove to be useful. One of the mode could reminder the older of us to some old school stuff.

I am trying to be kinda lenghtly on this readme. I hope to explain the why of some functions and for the user to use the one most suitable for its use case.

## Let's start
### Array of strips
In most leds driver librairies you declare each strip attached to one pin, one line at a time.

example 1: For 4 strips in FastLED
```C
CRGB leds1[number_of_leds1];
CRGB leds2[number_of_leds2];
CRGB leds3[number_of_leds3];
CRGB leds4[number_of_leds4];

FasLED.addLeds<PIN1,ORDER>(leds1,number_of_leds1);
FasLED.addLeds<PIN2,ORDER>(leds2,number_of_leds2);
FasLED.addLeds<PIN3,ORDER>(leds3,number_of_leds3);
FasLED.addLeds<PIN4,ORDER>(leds4,number_of_leds4);
```

example 2: If you are using a large array of same length strips, you would do this:
```C
CRGB leds[4*NUM_LED_PER_STRIPS];
FasLED.addLeds<PIN1,ORDER>(leds,0,NUM_LED_PER_STRIPS);
FasLED.addLeds<PIN2,ORDER>(leds,NUM_LED_PER_STRIPS,NUM_LED_PER_STRIPS);
FasLED.addLeds<PIN3,ORDER>(leds,2*NUM_LED_PER_STRIPS,NUM_LED_PER_STRIPS);
FasLED.addLeds<PIN4,ORDER>(leds,3*NUM_LED_PER_STRIPS,NUM_LED_PER_STRIPS);
```

example 3: For information if you want to get (for development purpose ease) the leds1,leds2,...
```C
CRGB *leds1=leds;
CRGB *leds2=leds+NUM_LED_PER_STRIPS;
CRGB *leds3=leds+2*NUM_LED_PER_STRIPS;
CRGB *leds4=leds+3*NUM_LED_PER_STRIPS;
```

If all the strips of the first example are of the same size, then the 2 examples are the doing exactly the same. Hence when using strips of different lengths we cannot put them in a  big array ? **FALSE**. You cant create a large array when using `NUM_LED_PER_STRIP` being the largest of `number_of_leds`. Of course you array woul be larger than you actual numbre of leds but we can do with the lost of space.




### OK, but what is the link between an array of strips and this driver ?
Here is how we would declare the 4 strips in of our example:
```C
CRGB leds[4*NUM_LED_PER_STRIPS];
int pins[4]={PIN1,PIN2,PIN3,PIN4};
driver.initled((uint8_t*)leds,pins,4,NUM_LED_PER_STRIPS,ORDER_GRB);
```
We are declaring that my `leds` array represent 4 strips of `NUM_LED_PER_STRIPS` leds ,each strip being linked to the pins defined in the pins array `pins`. This is way easier to declare a lot of strips. 


### If you want strips of different lengths
```C
#define LENGTH1 200
#define LENGTH2 300
#define LENGTH3 100
int pins[3]={0,2,4};
int lengths[3]={LENGTH1,LENGTH2,LENGTH3};
CRGB leds[LENGTH1 + LENGTH2 + LENGTH3];

driver.initled((uint8_t*)leds,pins,lengths,3,ORDER_GRB);
```

### First let's declare a new driver

```C
#include NUMSTRIPS x
#include NUM_LEDS_PER_STRIP y
#include "I2SClocklessLedDriver.h"

I2SClocklessLedDriver driver;
```

### How to define my leds array ?
#### You are using RGB type leds
RGB type leds store the information of the led over 3 bytes. `Red,Green,Blue`.  Hence the size in bytes of a led array of `NUM_LEDS` is `3xNUM_LEDS`
```C
uint8_t leds[3*NUM_LEDS];

//if you are using FastLED library this definition will be equivalent to the previous as the CRGB object is 3 bytes
CRGB leds[NUM_LEDS];

//you can use either of those
```
#### You are using RGBW type leds
This time to store the information of the led you will need 4 bytes `Red,Green,Blue,White` Hence the size in bytes of a led array of `NUM_LEDS` is `4xNUM_LEDS`
```C
uint8_t leds[4*NUM_LEDS];
```

### Driver functions
#### `initled(uint8_t *leds,int * Pins,int NUMSTRIPS,int num_led_per_strip,colorarrangment cArr)`:

 This function initialize the strips.
* `*leds`: a pointer to the leds array
* `*Pins`: a pointer to the pins array
* `num_strips`: the number of parallel strips
* `num_led_per_strip`: the number of leds per strip (or the number of leds in the longuest strip)
* `cArr`: The led ordering
    * `ORDER_GRBW`: For the RGBW strips
    * `ORDER_RGB`
    * `ORDER_RBG`
    * `ORDER_GRB` : The most often used
    * `ORDER_GBR`
    * `ORDER_BRG`
    * `ORDER_BGR`
 
 example 4: declaring 12 strips of 256 leds in GRB 
 ```C
 #define NUMSTRIPS 12
 #define NUM_LEDS_PER_STRIP 256
 
 #include "I2SClocklessLedDriver.h"

 I2SClocklessLedDriver driver;
 
 uint8_t leds[3*NUM_STRIPS*NUM_LEDS_PER_STRIP]; //equivalent of CRGB leds[NUM_LEDS_PER_STRIPS*NUM_LEDS_PER_STRIPS]
 int pins[NUM_STRIPS] ={0,2,4,5,12,13,14,15,16,29,25,26};
 driver.initled((uint8_t*)leds,pins,NUM_STRIPS,NUM_LED_PER_STRIP,ORDER_GRB);
 ```
 
 example 5: declaring 12 strips of 256 leds in RGBW
 ```C
 #define NUMSTRIPS 12
 #define NUM_LED_PER_STRIP 256
 
 #include "I2SClocklessLedDriver.h"

 I2SClocklessLedDriver driver;
 
 uint8_t leds[4*NUMSTRIPS*NUM_LED_PER_STRIP]; 
 int pins[NUMSTRIPS] ={0,2,4,5,12,13,14,15,16,29,25,26};
 driver.initled((uint8_t*)leds,pins,NUMSTRIPS,NUM_LED_PER_STRIP,ORDER_GRBW);
 ```


#### `initled(uint8_t *leds,int * Pins,int* lengths int NUMSTRIPS,colorarrangment cArr)`:

 This function initialize the strips.
* `*leds`: a pointer to the leds array
* `*Pins`: a pointer to the pins array
* `*lengths`: a pointer to the led strips length array
* `num_strips`: the number of parallel strips
* `cArr`: The led ordering
    * `ORDER_GRBW`: For the RGBW strips
    * `ORDER_RGB`
    * `ORDER_RBG`
    * `ORDER_GRB` : The most often used
    * `ORDER_GBR`
    * `ORDER_BRG`
    * `ORDER_BGR`

####  `void initled(Pixels pix,int *Pinsq)`

If you want to use the pixelslib library features [https://github.com/hpwit/PixelsTypes]

```C
#define NUMSTRIPS 4
#define USE_PIXELSLIB
#define SIZE_STRIP1 45
#define SIZE_STRIP2 20
#define SIZE_STRIP3 80
#define SIZE_STRIP4 63
#include "I2SClocklessLedDriver.h"

I2SClocklessLedDriver driver;

int lengths[4]={SIZE_STRIP1,SIZE_STRIP2,SIZE_STRIP3,SIZE_STRIP4};
Pixels leds=Pixels(lengths,4);
int pins[NUMSTRIPS] ={0,2,4,5};
driver.initled(leds,pins);


```

 #### `setBrightness(int brightness)`:
 
 This function sets the default brightness for 0->255
 
#### `void setGamma(float gammar,float gammab,float gammag)` :
This function allows you set the a gamma value per channel (you do not have to do this in your code anymore) more or less like the color correction of Fastled but using the gamma alogrithm

 #### `void setGamma(float gammar,float gammab,float gammag,float gammaw)`:
 Same as above only for RGBW leds


 #### `setPixel(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue)`:
 Set the color of a pixel 
 NB1: if you are using a RGBW led, this function will do and RGB->RGBW transformation with the following algotithm thanks to  @Jonathanese 
```C
W = min(R, G, B);
R = R - W; 
G = G - W;
B = B - W;
```
 NB: if you are familiar with FastLED it would be `leds[pos]=CRGB(red,green,blue)` as you will see in the examples

 
 #### `setPixel(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue,uint8_t white)`:
 Set the color of a pixel for RGBW strips
 
 #### `showPixels()`:
 
 This function displays the pixels.

 #### `showPixels(uint8_t *leds)`:
This function allow set 'on the go' the pointer to the leds. This will help if you are using two buffers for your animation. 
It can also be used to ease dithering see example `Dithering` (I need to work on a hardware implementation btw)
```C
#define NUMSTRIPS 12
 #define NUM_LED_PER_STRIP 256
 
 #include "I2SClocklessLedDriver.h"

 I2SClocklessLedDriver driver;
uint8_t leds1[NUM_LEDS*3];
uint8_t leds2[NUM_LEDS*3];

int pins[NUMSTRIPS] ={0,2,4,5,12,13,14,15,16,29,25,26};
driver.initled((uint8_t*)leds,pins,NUMSTRIPS,NUM_LED_PER_STRIP,ORDER_GRB);

//displyaing the leds in leds1
driver.showPixels();

//displaying the leds in leds2
driver.showPixels(leds2);

```

### And if you do not wanna wait while displaying ? 
`void showPixels(displayMode dispmode)` and `void showPixels(displayMode dispmode,uint8_t *newleds)` are two functions that can allow you to display the pixels without having to wait

```C
showPixels(NO_WAIT); //it will start displaying the leds but giving you back the process 
 showPixels(NO_WAIT,newleds); //same here

 //i.e if you do this
//A
showPixels(NO_WAIT); 
delay(20);
//B
 //between A and B it will have passed either 20 or the time of the showPixels (if it's longer than 20ms)
```

if you do this
```C
showPixels(NO_WAIT);
showPixels(NO_WAIT);
```
then the ssocn showPixels will wait for the first one to end before starting the second one.


### 'HARDWARE SCROLLING'
Old term for a nice trick. The idea is to do a remapping of the leds within the driver directly so that the leds are displayed in another order. Pixels are pushed one at a time, and the normal way to do it is by going led 0,1,2,3 ....,N
Let's say that I want to 'scroll' by 5 pixels all the leds. Normally you would move leds 4->N-1 into 0,N-5 and then copy led 0=>led N-4 act. and then do the fastled.show().
The way I do it is to push within the driver led 4,5,6,7, ...., N-1,0,1,2,3 by calculating each time which pixels needs to be displayed using a simple algorithm about something along this `lednumber=> (lednumber+scroll)%N` (then a bit more complicated to take into account snake arrangement or not ,...)

To activate this you need to add
#define ENABLE_HARDWARE_SCROLL

#### `OffsetDisplay` object:
```C
struct OffsetDisplay
{
    int offsetx;
    int offsety;
    int panel_height;
    int panel_width;
};
```
At the initiation of the leds a default Offdisplay is created with certain values. You can get this default object with `getDefaultOffset();`.

In the example `snakewithhardwarescroll.ino` each strip is treated as 'individual' and each snake will go around a single strip

#### Defining a panel
To be able to 'hardware scroll' in all directions you need to define how you panel is setup.
for instance if you have a panel 100 leds wide 20 leds height `panel_height=20` and `panel_witdh=100`.
If you are using mutilple strips you have two parameters 
NB: these parameters need to be put before `#include "I2SClocklessLedDriver.h"` :

`SNAKEPATTERN`
* `#define SNAKEPATTERN 0` if your strip are not in snake pattern.
* `#define SNAKEPATTERN 1` if your strip are arange in snake pattern **this is the default you do not need to put it in your program**

`ALTERNATEPATTERN`
* `#define ALTERNATEPATTERN 0` if the all the strip start on the same side
* `#define ALTERNATEPATTERN 1` if the all the strip start on alternate side  **this is the default you do not need to put it in your program**


#### `showPixels(OffDisplay offset)`:
This function can help you scroll your leds without doing a mem copy.

#### `showPixels(uint8_t * leds,OffDisplay offset)`:
Same function as before, where you can set the led buffer you want to display.

#### Is it reallly needed ?
Maybe not but fun (humm maybe not that fun lol) to make but great results.

 #### Examples:
 
* `gettingstarted.ino`: an example to use 16 parallel strips of 256 leds 
* `gettingstartedFastLED.ino`: an example to use 16 parallel strips of 256 leds using FastLED objects 
* `gettingstartedRGBW.ino`: an example to use 16 parallel strips of 256 leds of RGBW leds
* `Dithering`; how to use the showPixels(uint8_t *leds) to easly do dithering (just an example)
* `snakewithhardwarescroll.ino`: an example of use of the showPixel(OffsetDisplay offset) to do the snake
* `panelhardwarescroll.ino`: an example of hardware scrolling x and y direction



## Artifacts, DMA, second core, transposition, ...

### Artifacts
The ESP32 is a great controller with Wi-FI ,Bluetooth, RTOS, ... but this can cause the program you're running to stop at certain point in time or interupts not behaving as usual. This happen for a very small amount of time that you usually don't notice but when driving leds this can cause artifacts as these leds are really timing specifics. These artifacts have been reported especially when using wi-fi. A lot of effort has been put in order to avoid the artifacts. And it seems to work quite fine with this version of the driver.

To avoid this issue the idea would be to delegate the signal sending to a peripheral that does not rely on the CPU.

#### DMA and I2S
On the ESP32 it's possible to link a specific type of memory DMA (direct memory access) to the I2S driver (or the SPI). What happen in that case is that the I2S will be fed by the DMA without use of the CPU.
The I2S driver will push the data to the pins at a spefic fixed rate without the use of the CPU too.

That is the technique which used.

#### Great BUT ...
The driver uses two DMA buffers which are linked to one another. (B1 and B2). When B1 is read and pushed by the I2S then it will B2 and push it then back to B1 etc ... . Each time a buffer has finished to be pushed a interrupt occurs. In the driver the first set of pixels is loaded in B1 and during the time that B1 is pushed by the I2S we load B2. Once B1 has been pushed it will move to read B2 and push an interupt that allows us to load the next batch in B1. Once B2 has finished to be pushed an interupt occurs and start reading B1 it then load the next set of pixels in B2 and so on and so forth until all the led has been pushed.

This process works very fine, except of the interupt gets stucks because of wi-fi or something else. The interrupt code is stored in a specific part od the memory IRAM_ATTR that is 'protected' from interupts. **But it can happen that the interupts gets 'pushed' by wi-fi or other internal ticks.**

### A solution : push everything in DMA
The idea is to create a 'big' DMA buffer already filled with all the leds and the tell the I2S to read from that huge buffer. Hence no interupt to take care and the CPU rests during that time.

#### Of course but I need memory
To transmit a RGB pixel we need to transmit 24 bits adn 32 bits for a RGBW pixel. To respect the timing requirements of the leds we send 'ticks' during wich the ourput is high or low. in our case we sent 3 ticks per bits. for
* a 0 bit , 1 0 0 are sent (the output stays high during 416ns and low 834ns)
* a 1 bit , 1 1 0 are sent (the output stays high during 834ns and low 416ns)

NB:  This is the common use approximation of the real timing but it works fine.

Hence to send 24 bit we need 3x24=72 'ticks'. Hence this big DMA buffer will need to need to be 3 times bigger than the led array.

But we are not sending 1 strip at a time but up to 16 strips.

#### Transposition
The driver whatever the number of strips, sends 16 bits (2 bytes ) to the I2S at each 'ticks'. That means for sending 16 parallel pixels (1 pixel of each 16 strips) you need a buffer of size 24x3x2=144 bytes instead of 16x3=48 bytes in the leds array for RGB leds.

The operation that loads the leds of each strips in serie and move it in parallel is called transposition.

As a consequence the size of the big DMA buffer is only link to the `NUM_LED_PER_STRIP` and not the `NUMSTRIPS`. For instance a DMA buffer for 4 strips of 256 leds will be of the same size of 16 strips of 256 leds.

### OK I have enough memory and what else ?
For most of your usage you will have enough memory. Hence the big buffer can be created allowing some new stuff

#### No need for second core
Normally to speed up things, you may program your animation on one core and display on the seconf core using two leds buffers. Here no need. When launching the new function described bellow, The CPU will not be used for the actual 'push' to the leds hence you CPU is free to do someting else.

### Enabling Full DMA buffer
```c
#define FULL_DMA_BUFFER //this will enable the full dma buffer

#define NUMSTRIPS 12
#define NUM_LED_PER_STRIP 256

#include "I2SClocklessLedDriver.h"

I2SClocklessLedDriver driver;

uint8_t leds[4*NUMSTRIPS*NUM_LED_PER_STRIP]; 
int pins[NUMSTRIPS] ={0,2,4,5,12,13,14,15,16,29,25,26};
driver.initled((uint8_t*)leds,pins,NUMSTRIPS,NUM_LED_PER_STRIP,ORDER_GRBW);

```
Now three new functions are available

#### `showPixelsFirstTranspose()` 
This function will transpose the entire led array and the display it. Has this function as en async function when lauching twice it will wait for the first one the finish

**It's like you are running it on a second core without using it**

Example: if you size of your strip is 500 leds it will take 18ms to display
```c
//the duration of the to commands below will be 18ms+18ms =36ms
showPixels();
delay(18);

//the duration of the two commands below will be 19ms 
//the full transposition in the buffer will take 1 ms more or less then the code 
//goes to the delay function has the displaying if the DMA buffer does not require CPU
showPixelsFirstTranpose();
delay(18);


//in the example below if the modifytheledfunction() lasts less than the time  need to display the leds 
//then the second call will wait before starting and then it's like the modifytheledfunction()
showPixelsFirstTranpose();
modifytheledfunction() ....
showPixelsFirstTranpose();

```

Example: `FullBufferFastLED.ino` this example is the equivalent of  `gettingstartedFastLED.ino` but using the buffer. It can be noticed that the overall fps is now higher. 

### `void showPixelsFirstTranspose(uint8_t *new_leds)`
This function is equivalent to `showPixels(uint8_t *new_leds)`

### `void showPixelsFirstTranspose(OffsetDisplay offdisp)`
This function is equivalent to `showPixels(OffsetDisplay offdisp)`

### `void showPixelsFirstTranspose(OffsetDisplay offdisp,uint8_t * temp_leds)`
This function is equivalent to `showPixels((OffsetDisplay offdisp,uint8_t * temp_leds)`

#### `setPixelinBuffer(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue)`
This function put a pixel directly in the DMA buffer doing the transposition for RGB leds


#### `setPixelinBuffer(uint32_t pos, uint8_t red, uint8_t green, uint8_t blue,uint8_t white)`
This function put a pixel directly in the DMA buffer doing the transposition for RGBW leds

**If you are using these two functions and use `showPixelsFirstTranpose()` it will not work as this function will erase the DMA buffer while transposing the entire led buffer**
**When using the setPixelinBuffer functions the offsetdisplay are not taken, into account**

To display the content of the DMA buffer directly use

#### `showPixelsFromBuffer()`
This function directly show the leds from the DMA buffer without doing any transposition of the led buffer. 

Example: `FullBufferWithoutTransposition.ino` 

#### Remember what a video chip is ?
A video chip is  in continuously displaying the content of the video RAM (with some perks like for a game boy) without using the CPU at all.

Now you can consider the DMA buffer as video RAM and the video chip as the I2S. We just need to have hte showPixelsFromBuffer to  loop.

`showPixelsFromBuffer(LOOP)` : this function will display the content of the DMA buffer wihtout using the CPU

Example: `FullBufferLoop.ino`  In this example only with one show function

If you want to stop the loop `stopDisplayLoop()`  look at the example `FullBufferLoopStartStop.ino` . The lopp is stopped after 500 'turns' and restart afer 1500.

#### What about frame synchro ??

Using the loop functionality you don't know when you update the DMA buffer which pixel the I2s is currently displaying. As a consequence it can lead to not smooth animations.

If you're animation is not smooth enough we can sync using `waitSync()`.

Example: `FullBufferLoopSync.ino`  play with the `waitSync()` to see the difference.

## Conclusion
I guess I am getting crazy doing that lol. If you have the memory for it then use the DMA buffer and the `showPixelsFirstTranpose()`  if you can you are sure not to have issue with interupts.

If you wanna play old school, use `showPixelsFromBuffer(LOOP)`.  

In any case do not hesitate to contact me for features and remarks

## What is next ?
Add functionnalities to set the sync at any point plus interupt. (for the one old enough it will remind them of souvenirs)
Improve the speed of `setPixelinBuffer` the function is a bit slow for now.









 
 
 
 
 


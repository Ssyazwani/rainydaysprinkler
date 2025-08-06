//third test run- PA is for debug,
//changed the wiring to PB

//ultra sensor works
//LED works
//soil moisture works
//valve might need extra battery pack to work

// will try to complete by next week T_T

#undef __ARM_FP
#include "mbed.h"
#define WAIT_TIME_US_0 10
#define WAIT_TIME_US_1 20
#define WAIT_TIME_US_2 58
#define WAIT_TIME_MS_0 25
#define WAIT_TIME_MS_1 30
#define WAIT_TIME_MS_2 100
#define WAIT_TIME_MS_3 300
#define WAIT_TIME_MS_4 500
#define WAIT_TIME_MS_5 1000
#define WAIT_TIME_MS_6 2000
#define WAIT_TIME_MS_7 3000
#define WAIT_TIME_MS_8 4000
//input of IR sensor data
DigitalOut Trig(PB_8);
DigitalIn Echo(PB_9);
//turn on/off the on-board LED: PB14
DigitalOut LED(PC_0);

DigitalIn Moist_WET(PC_9); // need to test
DigitalIn Moist_DRY(PC_10); // need to test



//pc9, digital in pc10
unsigned char Count = 0;
float objDistance = 0;
void BlinkLED(int interval, int period); //blink LED, interval is the delay time to
//toggle the LED, period is how long to blink LED
int main()
{
    while (true) {
        LED = 0;
        Count = 0;
        Trig = 0; //drag the trigger signal to low first
        thread_sleep_for(WAIT_TIME_MS_1); //maintain at low for a while
        Trig = 1;
        wait_us(WAIT_TIME_US_1); //minimum 10us, here we apply 20us
        Trig = 0;
        
     while (Echo == 0); //wait until Echo becomes high
     while ( (Echo == 1) && (Count < 200) )
     {
        wait_us(WAIT_TIME_US_2); //delay 58us which is almost 1cm in measurement:
                                  //0.03432 cm/us x 58us) / 2 = 0.99528 cm
        Count ++; //counts up if echo signal keeps high.
        //However, 200 counts is equal to 11.6ms which limits
        //the max measurable distance is 2m in this sample code
        }
        if (Echo == 1) //if obstacle is more than 2m away, consider no obstacle is  detected
           continue;
           objDistance = (0.03432 * 58 * Count) / 2;
           printf ("Obstacle is %.2f cm away!\n", objDistance);
           if (Count < 30) //if obstacle is quite near, like 30cm
            {//blink LED_RED with high frequency
            BlinkLED(WAIT_TIME_MS_2, WAIT_TIME_MS_5); //internal 100ms, totoal time 1s
            } else if (Count < 60) {//if obstracle is near, like 60cm
            //blink LED PB14 with medium frequency
            BlinkLED(WAIT_TIME_MS_3, WAIT_TIME_MS_6); //internal 300ms, totoal time 2s
            } else {//if obstracle is not near
                //blink LED PB14 with low frequency
            BlinkLED(WAIT_TIME_MS_5, WAIT_TIME_MS_7); //internal 1000ms, totoal time 2s
            }
        }
    }
void BlinkLED(int interval, int period){
   int loopCount = period / interval;
   for (int i = 0; i < loopCount; i ++){
    LED = !LED;
    thread_sleep_for(interval);

   }

}
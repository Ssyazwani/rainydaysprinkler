// teammate came up with code
// LED usage is abit confusing, will be discussing how to improve and make it better
// need to work on Valve to make sure it works


/* LedBlinky
 * mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#undef __ARM_FP 
 
#include "mbed.h" 
 
#define WAIT_TIME_US_0 10  
#define WAIT_TIME_US_1 20 
#define WAIT_TIME_US_2 58  
 
#define WAIT_TIME_MS_0 30 
#define WAIT_TIME_MS_1 100 
#define WAIT_TIME_MS_2 1000 
#define WAIT_TIME_MS_3 3000 
#define WAIT_TIME_MS_4 4000 
#define WAIT_TIME_MS_5 6000 

// Moisture sensor
//DigitalIn moistA_PC9(PC_9);
DigitalIn moistD_PC10(PC_10);

// MCU - User blue button
DigitalIn button(PC_13);

// LED 
// red
DigitalOut leds_red(PC_0);

// green
DigitalOut leds_green(PC_1);

// blue
DigitalOut leds_blue(PC_2);

// Ultrasound trigger and echo
DigitalOut Trig(PB_8); 
DigitalIn  Echo(PB_9); 

// Valve
DigitalOut valve(PB_7);

// MCU red and green LEDs
DigitalOut led_green(PB_15);
DigitalOut led_red(PB_14);
 
unsigned char Count = 0; 
float objDistance = 0; 

bool emptyFlag = false; 
bool fullFlag = false;

void BlinkLED(int interval, int period);    //blink LED, interval is the delay time to 

int main() 
{      
    while (true) 
    {    
        leds_red=0;
        Count = 0; 
 
        Trig = 0;   //drag the trigger signal to low first 
        thread_sleep_for(WAIT_TIME_MS_0);  //maintain at low for a while 
 
        Trig = 1; 
        wait_us(WAIT_TIME_US_1);    //minimum 10us, here we apply 20us 
        Trig = 0; 
 
        while (Echo == 0);//wait until Echo becomes high 
         
        while ( (Echo == 1) && (Count < 200) ) 
        { 
            wait_us(WAIT_TIME_US_2); //delay 58us 
            Count ++;                
        } 
 
        if (Echo == 1)  
            continue;  
         
        objDistance = (0.03432 * 58 * Count) / 2; 
        printf ("\nObstacle is %.2f cm away!\n", objDistance); 
 
        // no object 5.97, with object thin - 4.98 (1/5 -1 filled)
        // 3.98 cm (2/5 filled)
        // 2.99 cm
        if (Count <= 3)     //if obstacle is quite near
        { 
            //blink LED red with high frequency 
            printf("The container is full. Do not need to fill up the water again.");
            BlinkLED(WAIT_TIME_MS_1, WAIT_TIME_MS_2); 
            fullFlag = true;     //full of rainwater
            emptyFlag = false;   // empty (lack of rainwater)
        } 
        else    //if obstracle is not near 
        { 
            //blink LED red with low frequency 
            printf("\nThe container is empty or nearing empty!");
            BlinkLED(WAIT_TIME_MS_2, WAIT_TIME_MS_3); //internal 1000ms, total time 2s 
            fullFlag = false;
            emptyFlag = true;   
        }       

        if (moistD_PC10 == 1)
        { 
            // DRY
            printf("\nDry - led green is on");
            led_green = 1;      
            thread_sleep_for(WAIT_TIME_MS_1);  

            // open valve to water the plant
            // set valve to open for 4 seconds and MCU red led to on
            printf("\nset valve to open for 4 seconds and MCU red led to on");
            valve = 1;
            led_red = 1;    // MCU red LED is on
            thread_sleep_for(WAIT_TIME_MS_4); 

            // set valve to close and MCU red led to off
            valve = 0;  //valve is close
            led_red = 0;    // MCU red LED is off
            thread_sleep_for(WAIT_TIME_MS_2);   // wait 1 second
        }
        else {
            // WET
            printf("\nWet - led green is off");
            led_green = 0;
        }

        if (emptyFlag) {
            // Ask to fill up water with blue LED on
            printf("\n\nPlease fill up water!\nThe container is empty. Blue LED is on (purple when red is on too)");
            leds_blue = 1; //LED blue will light up
        }

        if (fullFlag) {
            // Container is full
            printf("\n\nWater is fiiled up. Container is full. Blue LED is off");
            leds_blue = 0; //LED blue will not light up
        }
    } 
} 
 
void BlinkLED(int interval, int period) 
{ 
    int loopCount = period / interval; 
 
    for (int i = 0; i < loopCount; i ++) 
    { 
        leds_red = !leds_red; 
        thread_sleep_for(interval); 
    } 
     
} 
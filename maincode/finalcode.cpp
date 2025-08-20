

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

#define MAX_COUNT 20
#define WATER_AVAILABLE_THRESHOLD_CM 6

DigitalOut Trig(PB_8);
DigitalIn Echo(PB_9);

DigitalIn PC12 (PC_12);

DigitalOut waterPump (PC_13);
DigitalOut red_LED (PC_0);
DigitalOut green_LED (PC_1);
DigitalOut blue_LED (PC_2);

DigitalIn Moist_DRY(PC_10);

unsigned char Count = 0;
float objDistance =0;

void setLEDcolour(bool red, bool green, bool blue){
    red_LED = red;
    green_LED = green;
    blue_LED = blue;
}

int main(){
    while(true){
        Trig = 0;
        thread_sleep_for(WAIT_TIME_MS_1);
        Trig = 1;
        wait_us(WAIT_TIME_US_1);
        Trig =0;

        Count =0;
        while (Echo == 0);
        while((Echo ==  1 ) &&(Count < MAX_COUNT)){
            wait_us(WAIT_TIME_US_2);
            Count++;
        }

        if(Echo == 1){
            printf("No water");
            waterPump =0;
            setLEDcolour(0,0,1);
            thread_sleep_for(1000);
            continue;
        }

        objDistance = (0.03432 * 58* Count)/2;
        printf("Distance between Sensor and Water%.2f cm\n",objDistance);

        if(objDistance <WATER_AVAILABLE_THRESHOLD_CM){
            if(PC12 == 1){
                
                waterPump =0;
                setLEDcolour(1,0,0);
                prinf("Stop watering");

            } else {

                printf("Soil is dry, start watering");
                waterPump = 1;
                setLEDcolour(0,1,0);
                thread_sleep_for(500);

                waterPump =0;
                setLEDcolour(1,0,0);

                prinf("Watering done\n");
            }

            thread_sleep_for(10000);

        }


    }
}


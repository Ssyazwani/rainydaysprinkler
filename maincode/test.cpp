//first testcode with prototype

//H3 - Ultrasonic Level - PA0, PA2
//H5 - LED - PC0,PC1,PC2
//H6 - Valve - PB1
//H7 - Soil Moisture - PC8,PC9

#undef __ARM_FP

#include "mbed.h"

DigitalOut trigger(PA_2);// edit the PIN
DigitalIn echo (PA_3);//edit the PIN

//AnalogIn soilSensor(PC_8); the soilsensor PIN is wrong

DigitalOut red_LED(PC_0);
DigitalOut green_LED(PC_1);
DigitalOut blue_LED(PC_2);

DigitalOut valve(PB_1);

Timer echoTimer;

#define WET_THRSEHOLD 0.5f
#define SPEED_OF_SOUND_CM_PER_US 0.0343f

void delay_us(int us){
    wait_us(us);
}

float getDistanceCM(){
    trigger =1;
    wait_us(10);
    trigger =0;

    while(echo ==0){

    }
        echoTimer.reset();
        echoTimer.start();

    while(echo ==1){

    }
    echoTimer.stop();

    float duration_us = echoTimer.read_us();
    float distance =(duration_us *SPEED_OF_SOUND_CM_PER_US)/2.0f;
    return distance;
}

int main(){

    printf("System started\n");

    while(true){
        //Sensor
        float distance = getDistanceCM();
        printf("Distance:%.2fcm\n", distance);

        if (distance <4.0f)
        printf("Enough water\n");
        else
        printf("Not Enough Water\n");

        //Soil Moisture
        float moisture = soilSensor.read();
        printf("Soil Moisture: %.3f\n",moisture);

        if (moisture < WET_THRSEHOLD){
            red_LED = 1;
            green_LED = 0;
            blue_LED = 0;

            //Valve
            valve =1;
            printf("Valve Open\n");

        } else {
            red_LED = 0;
            green_LED = 0;
            blue_LED = 0;


            //Valve
            valve =0;
            printf("Valve Closed\n");
        }

        

        thread_sleep_for(1000);
    }
}
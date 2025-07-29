//first testcode with prototype

#include "mbed.h"

DigitalOut tigger(D1);
DigitalIn echo (D2);

AnalogIn soilSensor(A0);
DigitalOut dryLed(LED1);

DigitalIn controlInput(D3);
DigitalOut valve(D4);

Timer echoTimer;

#define WET_THRSEHOLD 0.5f
#define SPEED_OF_SOUND_CM_PER_US 0.0343f

void delay_us(int us){
    wait_us(us);
}

float getDistanceCM(){
    tigger =1;
    wait_us(10);
    trigger =0;

    while(echo ==0);
        echoTimer.reset();
        echoTimer.start();

    while(echo ==1);
    echoTimer.stop;

    float duration_us = echoTimer.read.us();
    float distance =(uratio_us *SPEED_OF_SOUND_CM_PER_US)/2.0f;
    return distance;
}

int main(){

    printf("System started\n");

    while(true){
        //Sensor
        float distance = getDistanceCM();
        printf("Distnance:%.2fcm\n", distance);

        if (distance <100.0f)
        printf("Enough water\n");
        else
        printf("Not Enough Water\n");

        //Soil Moisture
        float moisture = soilSensor.read();
        printf("Soil Moisture: %.3f\n",moisture);

        if (moisture < WET_THRSEHOLD){
            dryLED = 1; // LED ON if soil is dry
        } else {
            dryLED = 0;
        }

        //Valve

        if(controlInput == 1){
            valve =1;
            printf("Valve Open\n");
        } else{
            valve =0;
            printf("Valve Closed\n");
        }

        thread_sleep_for(1000);
    }
}
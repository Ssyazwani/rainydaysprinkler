//test code troubleshoot
//trying out ultrasensor
// we are using a diff type, so it might be abit off

//will be trying out the ultrasensor the sch uses, buying other spares in case 
//other components did not work


#undef __ARM_FP

#include "mbed.h"

DigitalOut trigger(PA_2);
DigitalIn echo (PA_3);

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

float getDistanceCM() {
    trigger = 0;
    wait_us(2);
    trigger = 1;
    wait_us(15);
    trigger = 0;

    Timer timeout;
    timeout.start();

    while (echo == 0) {
        if (timeout.read_ms() > 50) {
            printf("Timeout waiting for echo HIGH\n");
            return -1.0f;
        }
    }

    echoTimer.reset();
    echoTimer.start();

    while (echo == 1) {
        if (timeout.read_ms() > 100) {
            printf("Timeout waiting for echo LOW\n");
            return -1.0f;
        }
    }

    echoTimer.stop();
    float duration_us = echoTimer.read_us();
    printf("Echo duration: %.0f us\n", duration_us);

    float distance = (duration_us * SPEED_OF_SOUND_CM_PER_US) / 2.0f;
    return distance;
}



int main(){

    printf("System started\n");

    while(true){
        //Sensor
        float distance = getDistanceCM();
        if (distance < 0) {
    printf("No valid echo detected\n");
    } else {
    printf("Distance: %.2f cm\n", distance);
    }


    }
}


// output was Distance: 100cm 
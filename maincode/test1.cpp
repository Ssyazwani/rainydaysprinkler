//second  testcode with prototype

//H3 - Ultrasonic Level -  PA3(Trigger), PA2(Echo)
//H5 - LED - PC0,PC1,PC2
//H6 - Valve - PB1
//H7 - Soil Moisture - PC8,PC9

// Switch utrasensor
// it might be wiring and voltage problem - Valve, sensor needs 5 V for output
// our microcontroller GPIO is only 3.3V, need breadboard and resistors
// will edit the hardware wiring tmr
// polling mode

// but can be used

#undef __ARM_FP

#include "mbed.h"

// HC-SR04 pins
DigitalOut trigger(PA_3);
DigitalIn echo(PA_2);

// Valve and LEDs
DigitalOut valve(PB_1); // Active high
DigitalOut red_LED(PC_0);
DigitalOut green_LED(PC_1);
DigitalOut blue_LED(PC_2);

Timer echoTimer;

// Constants
#define SPEED_OF_SOUND_CM_PER_US 0.0343f
#define DISTANCE_THRESHOLD_CM    1.5f // Change as needed

// Get distance using HC-SR04 (in cm)
float getDistanceCM() {
    trigger = 0;
    wait_us(2);
    trigger = 1;
    wait_us(10);
    trigger = 0;

    Timer timeout;
    timeout.start();

    // Wait for echo HIGH (signal start)
    while (echo == 0) {
        if (timeout.elapsed_time().count() > 50000) { // 50 ms timeout
            printf("Timeout: No echo start detected.\n");
            return -1.0f;
        }
    }

    echoTimer.reset();
    echoTimer.start();

    // Wait for echo LOW (signal end)
    while (echo == 1) {
        if (timeout.elapsed_time().count() > 100000) { // 100 ms timeout
            printf("Timeout: Echo signal stuck HIGH.\n");
            return -1.0f;
        }
    }

    echoTimer.stop();

    float duration_us = echoTimer.elapsed_time().count(); // microseconds
    float distance = (duration_us * SPEED_OF_SOUND_CM_PER_US) / 2.0f;

    return distance;
}

// Main loop
int main() {
    printf("=== Water Level Sensor System Started ===\n");

    while (true) {
        float distance = getDistanceCM();

        if (distance < 0) {
            // Sensor error
            printf("Sensor error, skipping...\n");
            red_LED = 1;
            green_LED = 0;
            blue_LED = 0;
            valve = 0;
        } else {
            printf("Distance: %.2f cm\n", distance);

            if (distance <= DISTANCE_THRESHOLD_CM) {
                // Not enough water → open valve
                printf("Water level LOW → Opening valve\n");
                red_LED = 0;
                green_LED = 1;
                blue_LED = 0;
                valve = 1;
            } else {
                // Enough water → close valve
                printf("Water level OK → Closing valve\n");
                red_LED = 0;
                green_LED = 0;
                blue_LED = 1;
                valve = 0;
            }
        }

        thread_sleep_for(1000); // Wait 1 second before next reading
    }
}


//Output was 1.73 cm - more reasonable

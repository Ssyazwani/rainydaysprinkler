//valve needed alot of current to work
//it works but the code seems to be wrong (code in test3), wrong as in it doesnt match flowchart
//edit to match flowchart, will test out ASAP



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

#define MAX_COUNT 20  // Define based on distance expected from your sensor
#define WATER_AVAILABLE_THRESHOLD_CM 6.5 // MAX distance is 20 cm, Water is available if distance < 30 cm


// Ultrasonic sensor pins
DigitalOut Trig(PB_8);
DigitalIn Echo(PB_9);

// Valve and RGB LEDs
DigitalOut waterPump (PB_7);       // Control WaterPump
DigitalOut red_LED(PC_0);     // Red = stop water pump - wet
DigitalOut green_LED(PC_1);   // Green = start water pump - dry
DigitalOut blue_LED(PC_2);    // Blue = no water

// Soil moisture sensors
// and Moist_DRU is HIGH when dry
DigitalIn Moist_DRY(PC_10);


//pc9, digital in pc10
unsigned char Count = 0;
float objDistance = 0;

// Helper to set LED color
void setLEDColor(bool red, bool green, bool blue) {
    red_LED = red;
    green_LED = green;
    blue_LED = blue;
}

int main() {
    while (true) {
        // Trigger the ultrasonic sensor
        Trig = 0;
        thread_sleep_for(WAIT_TIME_MS_1); // Wait 30 ms
        Trig = 1;
        wait_us(WAIT_TIME_US_1); // Send 20 µs pulse
        Trig = 0;

        // Wait for echo
        Count = 0;
        while (Echo == 0); // Wait until Echo goes high
        while ((Echo == 1) && (Count < MAX_COUNT)) {
            wait_us(WAIT_TIME_US_2); // Wait ~58 µs per cm
            Count++;
        }

        if (Echo == 1) {
            // Echo still high after max count → too far
            printf("No water (distance is too far)\n");
            waterPump = 0;
            setLEDColor(0, 0, 1); // Blue LED
            thread_sleep_for(1000);
            continue;
        }

        // Calculate distance (in cm)
        objDistance = (0.03432 * 58 * Count) / 2;
        printf("Distance Between Sensor and Water: %.2f cm\n", objDistance);

        if (objDistance < WATER_AVAILABLE_THRESHOLD_CM) {

            // Water is available
            if (Moist_DRY == 1) {
                // Soil is dry → start watering
                printf("Soil is dry. Watering...\n");
                waterPump = 1;
                setLEDColor(0, 1, 0); // Green LED

                // Wait until soil becomes wet
                while (Moist_DRY == 1) {
                    thread_sleep_for(1000); // Check every 1s
                }

                // Stop watering
                waterPump = 0;
                setLEDColor(1, 0, 0); // Red LED
                printf("Soil is now wet. Stopped watering.\n");

            } else {
                // Soil is already wet
                waterPump = 0;
                setLEDColor(1, 0, 0); // Red LED
                printf("Soil is already wet. No watering needed.\n");
            }

        } else {
            // Water is NOT available
            waterPump = 0;
            setLEDColor(0, 0, 1); // Blue LED
            printf("Water not available. Valve remains OFF.\n");
        }

        thread_sleep_for(1000); // Delay before next cycle
    }
}
// Define sensor pins
#include "main.h"
#include <stdio.h>

// Pin definitions (fix GPIO macro)
#define TRIGGER_PIN GPIO_PIN_1
#define ECHO_PIN    GPIO_PIN_2
#define TRIGGER_PORT GPIOA
#define ECHO_PORT    GPIOA

// Timer setup
#define TIMER_INSTANCE TIM1
#define TIMER_CHANNEL TIM_CHANNEL_1

// Function prototypes
void delay_us(uint32_t us);
uint32_t GetEchoDuration();  // You must implement this using timer/capture

int main(void)
{
    HAL_Init();
    // SystemClock_Config(), GPIO & Timer init must be done before this loop

    while (1) {
        // 1. Send trigger pulse
        HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_SET);
        delay_us(10);  // Wait 10 µs
        HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_RESET);

        // 2. Wait a bit for echo (or use a flag from interrupt)
        HAL_Delay(100);  // You might replace this with actual capture wait

        // 3. Get echo duration and calculate distance
        uint32_t echoDuration = GetEchoDuration();  // In µs
        float distance = (echoDuration * 0.0343f) / 2.0f;  // In cm

        // 4. Print status
        if (distance < 100.0f) {
            printf("Enough Water\n");
        } else {
            printf("Not Enough Water\n");
        }

        HAL_Delay(1000);  // 1 second between checks
    }
}

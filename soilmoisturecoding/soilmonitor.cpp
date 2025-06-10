// STM32F4 Discovery, Soil Moisture Sensor Example

#include "stm32f4xx_hal.h"

// Define the ADC channel for the soil moisture sensor
#define ADC_CHANNEL ADC_CHANNEL_1

// Define the GPIO pin connected to the sensor's analog output
#define SENSOR_PIN GPIOA_Pin_10

// Define the GPIO port for the sensor pin
#define SENSOR_PORT GPIOA


#define WET_THRESHOLD 1500


DigitalOut L1 (LED_L1);

// Function to initialize the ADC
void ADC_init(void) {
    // Configure the ADC 
    ADC_HandleTypeDef hadc1;
    HAL_ADC_Init(&hadc1);

    // Configure the ADC channel
    ADC_ChannelConfTypeDef sConfigADC = {0};
    sConfigADC.Channel = ADC_CHANNEL_1;
    sConfigADC.Rank = ADC_RANK_FIRST;
    sConfigADC.SamplingTime = ADC_SAMPLE_TIME_8_CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfigADC);
}

// Function to read the sensor value
uint32_t read_soil_moisture(void) {
    // Start ADC conversion
    HAL_ADC_Start(&hadc1);

    // Wait for the conversion to complete
    while (!HAL_ADC_PollForConversion(&hadc1, 1000)) { } // Wait up to 1 second

    // Get the converted value
    return HAL_ADC_GetValue(&hadc1);
}

// Main function
int main(void) {
    // Initialize the microcontroller
    HAL_Init();

    // Initialize the ADC
    ADC_init();

    // Enable GPIO clock for the sensor pin
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure the GPIO pin for analog input
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SENSOR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(SENSOR_PORT, &GPIO_InitStruct);

    // Main loop
    while (1) {
        // Read the soil moisture value
        uint32_t moistureValue = read_soil_moisture();

        // Print the value to the serial monitor (example)
        printf("Soil Moisture Value: %u\n", moistureValue);

        // Add a delay
        HAL_Delay(1000);


        //LED light on when it is dry, off when it is wet enough

        if (moistureValue < WET_THRESHOLD) {
          L1 =0 ;
        } else {
          L1 =1;
        }
          



    }
}
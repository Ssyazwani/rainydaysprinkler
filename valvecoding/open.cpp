#define VALVE_PIN   GPIO_PIN_5
#define VALVE_PORT  GPIOA

#define INPUT_PIN   GPIO_PIN_0
#define INPUT_PORT  GPIOB

int main(void)
{
    HAL_Init();
    // Initialize clocks, GPIOs, etc.

    while (1) {
        // Read input pin (e.g., button or sensor)
        GPIO_PinState inputState = HAL_GPIO_ReadPin(INPUT_PORT, INPUT_PIN);

        if (inputState == GPIO_PIN_SET) {
            // Open the valve
            HAL_GPIO_WritePin(VALVE_PORT, VALVE_PIN, GPIO_PIN_SET);
            printf("Valve Open\n");
        } else {
            // Close the valve
            HAL_GPIO_WritePin(VALVE_PORT, VALVE_PIN, GPIO_PIN_RESET);
            printf("Valve Closed\n");
        }

        HAL_Delay(200);  // Debounce or poll delay
    }
}

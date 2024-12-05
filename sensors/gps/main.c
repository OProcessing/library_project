#include "gps.h"

extern UART_HandleTypeDef huart1;

int main(void) {
    HAL_Init();
    SystemClock_Config();

    MX_USART1_UART_Init();
    GPS_Init(&huart1);

    while (1) {
        HAL_Delay(1000);
        printf("Latitude: %.6f, Longitude: %.6f, Altitude: %.2f m\n",
               GPS.latitude, GPS.longitude, GPS.altitude);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        GPS_UART_Callback();
    }
}

#define true 1
#define false 0
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semaphore.h"

void vRobo1(void *pvParameters);
void vMaquina1(void *pvParameters);
int main(void){
    xTaskCreate(&vRobo1, "Robo 1", 1024, NULL, 1, NULL);
    xTaskCreate(&vMaquina1, "Maquina1 2", 1024, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0;
}

void vRobo1(void *pvParameters){
    while(true){
        printf("Retirando insumos do deposito de entrada...\r\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vMaquina1(void *pvParameters){
    while(true){
        printf("...Colocando no deposito de entrada de M1\r\n");
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

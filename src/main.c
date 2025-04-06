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
#include "semphr.h"

SemaphoreHandle_t sem_Maquina1 = NULL, sem_Produto1 = NULL, sem_AcessoRetirada = NULL;


void vRobo1(void *argument);
void vMaquina1(void *argument);
void vRobo2(void *argument);
void vRobo3(void *argument);

int main(void){

	sem_Maquina1 = xSemaphoreCreateBinary();
	sem_Produto1 = xSemaphoreCreateBinary();
	sem_AcessoRetirada = xSemaphoreCreateBinary();

	// Inicialmente a máquina está livre:
	xSemaphoreGive(sem_Maquina1);
	//xSemaphoreGive(sem_Produto1);
	// Acesso à retirada também liberado:
	xSemaphoreGive(sem_AcessoRetirada);

    xTaskCreate(&vRobo1, "Start Robo1", 1024, NULL, 1, NULL);
    xTaskCreate(&vRobo2, "Start Robo2", 1024, NULL, 1, NULL);
    xTaskCreate(&vRobo3, "Start Robo3", 1024, NULL, 2, NULL);
    xTaskCreate(&vMaquina1, "Start Maquina1", 1024, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0;
}

void vRobo1(void *argument){
  static int contador = 0;
  static BaseType_t pegou_maquina = pdFALSE;

  for(;;){
    if (!pegou_maquina)
    {
      // Pega o semáforo da máquina só uma vez
      if (xSemaphoreTake(sem_Maquina1, pdMS_TO_TICKS(1)) == pdTRUE)
      {
        pegou_maquina = pdTRUE;
        contador = 0;
        printf("[R1] M1 vazio, R1 trabalhando...\n");
      }
    }

    if (pegou_maquina)
    {
      contador++;
      sleep(1);
      if(contador==1)
        printf("[R1] Pegando produto da entrada...\r\n");
      else if(contador==100)
        printf("[R1] Movendo produto da entrada para M1...\r\n");
      else if(contador==600)
        printf("[R1] Inserindo produto em M1.\r\n");
      else if(contador > 700)
      {
        xSemaphoreGive(sem_Maquina1);
        pegou_maquina = pdFALSE;
        contador = 0;
        printf("[R1] Entregue a M1.\n");
      }
    }

    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void vMaquina1(void *argument)
{
  static int contador = 0;
  static BaseType_t ocupando = pdFALSE;

  for(;;){
    if (!ocupando)
    {
      if (xSemaphoreTake(sem_Maquina1, pdMS_TO_TICKS(1)) == pdTRUE)
      {
        ocupando = pdTRUE;
        contador = 0;
        printf("[M1] Produto recebido. Iniciando processamento...\r\n");
      }
    }

    if (ocupando)
    {
      sleep(1);
      contador++;

      if (contador == 1500)
      {
        printf("[M1] Produto processado.\r\n");
        xSemaphoreGive(sem_Produto1);       // Sinaliza que produto está pronto
        xSemaphoreGive(sem_Maquina1);       // Libera máquina
        ocupando = pdFALSE;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(1));
  }
}


void vRobo2(void *argument){
	static int contador = 0;
	static BaseType_t pegou = pdFALSE;
  
	for(;;){
	  if (!pegou && xSemaphoreTake(sem_Produto1, pdMS_TO_TICKS(1)) == pdTRUE)
	  {
		if (xSemaphoreTake(sem_AcessoRetirada, pdMS_TO_TICKS(1)) == pdTRUE)
		{
		  pegou = pdTRUE;
		  contador = 0;
		}
	  }
  
	  if (pegou)
	  {
		sleep(1);
		contador++;
		if(contador == 1)
		  printf("[R2] Pegando produto de M1...\r\n");
		else if(contador == 100)
		  printf("[R2] Movendo produto de M1 para M2...\r\n");
		else if(contador == 600)
		  printf("[R2] Inserindo produto em M2.\r\n");
		else if(contador > 700)
		{
		  pegou = pdFALSE;
		  contador = 0;
		  xSemaphoreGive(sem_AcessoRetirada); // Libera acesso para outro robô
		}
	  }
  
	  vTaskDelay(pdMS_TO_TICKS(1));
	}
  }
  void vRobo3(void *argument){
	static int contador = 0;
	static BaseType_t pegou = pdFALSE;
  
	for(;;){
	  if (!pegou && xSemaphoreTake(sem_Produto1, pdMS_TO_TICKS(1)) == pdTRUE)
	  {
		if (xSemaphoreTake(sem_AcessoRetirada, pdMS_TO_TICKS(1)) == pdTRUE)
		{
		  pegou = pdTRUE;
		  contador = 0;
		}
	  }
  
	  if (pegou)
	  {
		sleep(1);
		contador++;
		if(contador == 1)
		  printf("[R3] Pegando produto de M1...\r\n");
		else if(contador == 100)
		  printf("[R3] Movendo produto de M1 para M3...\r\n");
		else if(contador == 900)
		  printf("[R3] Inserindo produto em M3.\r\n");
		else if (contador > 1000)
		{
		  pegou = pdFALSE;
		  contador = 0;
		  xSemaphoreGive(sem_AcessoRetirada); // Libera acesso para outro robô
		}
	  }
  
	  vTaskDelay(pdMS_TO_TICKS(1));
	}
  }
  
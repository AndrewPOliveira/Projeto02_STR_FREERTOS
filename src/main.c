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
SemaphoreHandle_t sem_Maquina2 = NULL, sem_Produto2 = NULL;
SemaphoreHandle_t sem_Maquina3 = NULL, sem_Produto3 = NULL;
SemaphoreHandle_t sem_ProdutoDepositado_M2 = NULL, sem_ProdutoDepositado_M3 = NULL;


void vRobo1(void *argument);
void vRobo2(void *argument);
void vRobo3(void *argument);
void vRobo4(void *argument);
void vMaquina1(void *argument);
void vMaquina2(void *argument);
void vMaquina3(void *argument);


int main(void){

	sem_Maquina1 = xSemaphoreCreateBinary();
	sem_Produto1 = xSemaphoreCreateBinary();
  sem_Maquina2 = xSemaphoreCreateBinary();
	sem_Produto2 = xSemaphoreCreateBinary();
  sem_Maquina3 = xSemaphoreCreateBinary();
	sem_Produto3 = xSemaphoreCreateBinary();
  sem_ProdutoDepositado_M2 = xSemaphoreCreateBinary();
  sem_ProdutoDepositado_M3 = xSemaphoreCreateBinary();
	sem_AcessoRetirada = xSemaphoreCreateBinary();

	// Inicialmente a máquina está livre:
	xSemaphoreGive(sem_Maquina1);
  xSemaphoreGive(sem_Maquina2);
  xSemaphoreGive(sem_Maquina3);
	// Acesso à retirada também liberado:
	xSemaphoreGive(sem_AcessoRetirada);

    xTaskCreate(&vRobo1, "Start Robo1", 1024, NULL, 1, NULL);
    xTaskCreate(&vRobo2, "Start Robo2", 1024, NULL, 2, NULL);
    xTaskCreate(&vRobo3, "Start Robo3", 1024, NULL, 3, NULL);
    xTaskCreate(&vRobo4, "Start Robo4", 1024, NULL, 4, NULL);
    xTaskCreate(&vMaquina1, "Start Maquina1", 1024, NULL, 1, NULL);
    xTaskCreate(&vMaquina2, "Start Maquina2", 1024, NULL, 2, NULL);
    xTaskCreate(&vMaquina3, "Start Maquina3", 1024, NULL, 2, NULL);
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
        printf("\033[0m\033[33;44m[R1] M1 vazio, R1 trabalhando...\033[0m\n");
      }
    }

    if (pegou_maquina)
    {
      contador++;
      sleep(1);
      if(contador==1)
        printf("\033[0m\033[33;44m[R1] Pegando produto da entrada...\033[0m\r\n");
      else if(contador==100)
        printf("\033[0m\033[33;44m[R1] Movendo produto da entrada para M1...\033[0m\r\n");
      else if(contador==600)
        printf("\033[0m\033[33;44m[R1] Inserindo produto em M1.\033[0m\r\n");
      else if(contador > 700)
      {
        xSemaphoreGive(sem_Maquina1);
        pegou_maquina = pdFALSE;
        contador = 0;
        printf("\033[0m\033[33;44m[R1] Entregue a M1.\033[0m\n");
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
        printf("\033[0m\033[31m[M1] Produto recebido. Iniciando processamento...\033[0m\r\n");
      }
    }

    if (ocupando)
    {
      sleep(1);
      contador++;

      if (contador == 1500)
      {
        printf("\033[0m\033[31m[M1] Produto processado.\033[0m\r\n");
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
		  printf("\033[4;35m[R2] Pegando produto de M1...\033[0m\\r\n");
		else if(contador == 100)
		  printf("\033[4;35m[R2] Movendo produto de M1 para M2...\033[0m\\r\n");
		else if(contador == 600)
		  printf("\033[4;35m[R2] Inserindo produto em M2.\033[0m\\r\n");
		else if(contador > 700)
		{
      pegou = pdFALSE;
      contador = 0;
      xSemaphoreGive(sem_ProdutoDepositado_M2); // Notifica M2
      xSemaphoreGive(sem_AcessoRetirada);
		}
	  }
  
	  vTaskDelay(pdMS_TO_TICKS(1));
	}
  }
  void vRobo3(void *argument){
	static int contador = 0;
	static BaseType_t pegou = pdFALSE;
  
	for(;;){
	  if (!pegou && xSemaphoreTake(sem_Produto1, pdMS_TO_TICKS(1)) == pdTRUE  
        && xSemaphoreTake(sem_Maquina3, pdMS_TO_TICKS(1)) == pdTRUE)
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
      xSemaphoreGive(sem_ProdutoDepositado_M3); // Notifica a Máquina 3
      xSemaphoreGive(sem_Maquina3);
		  pegou = pdFALSE;
		  contador = 0;
		  xSemaphoreGive(sem_AcessoRetirada); // Libera acesso para outro robô
		}
	  }
  
	  vTaskDelay(pdMS_TO_TICKS(1));
	}
  }
  
  void vMaquina2(void *argument)
  {
    static int contador = 0;
    static BaseType_t ocupando = pdFALSE;
  
    for (;;)
    {
      if (!ocupando)
      {
        if (xSemaphoreTake(sem_ProdutoDepositado_M2, pdMS_TO_TICKS(1)) == pdTRUE)
        {
          ocupando = pdTRUE;
          contador = 0;
          printf("\033[0;92m[M2] Produto recebido. Iniciando processamento...\033[0m\\r\n");
        }
      }
  
      if (ocupando)
      {
        sleep(1);
        contador++;
  
        if (contador == 1500)
        {
          printf("\033[0;92m[M2] Produto processado.\033[0m\\r\n");
          xSemaphoreGive(sem_Produto2);     // Produto pronto
          ocupando = pdFALSE;
        }
      }
  
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }

  void vMaquina3(void *argument)
  {
    static int contador = 0;
    static BaseType_t ocupando = pdFALSE;
  
    for (;;)
    {
      if (!ocupando)
      {
        if (xSemaphoreTake(sem_ProdutoDepositado_M3, pdMS_TO_TICKS(1)) == pdTRUE)
        {
          ocupando = pdTRUE;
          contador = 0;
          printf("\033[46m[M3] Produto recebido. Iniciando processamento...\033[0m\\r\n");
        }
      }
  
      if (ocupando)
      {
        sleep(1);
        contador++;
  
        if (contador == 1500)
        {
          printf("\033[46m[M3] Produto processado.\033[0m\\r\n");
          xSemaphoreGive(sem_Produto3);     // Produto pronto
          ocupando = pdFALSE;
        }
      }
  
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }

void vRobo4(void *argument){
  static int contador = 0;
	static BaseType_t pegou = pdFALSE;
  static int pegou_produto = 0;
  
	for(;;){
    if (!pegou)
    {
      if(xSemaphoreTake(sem_Produto2, pdMS_TO_TICKS(1)) == pdTRUE)
      {
        pegou = pdTRUE;
        contador = 0;
        pegou_produto = 2;
      }
      else if(xSemaphoreTake(sem_Produto3, pdMS_TO_TICKS(1)) == pdTRUE)
      {
        pegou = pdTRUE;
        contador = 0;
        pegou_produto = 3;
      }

    }
  
	  if (pegou_produto == 2 && pegou)
	  {
		sleep(1);
		contador++;
		if(contador == 1)
		  printf("\033[0;103m[R4] Pegando produto de M2...\033[0m\\r\n");
		else if(contador == 100)
		  printf("\033[0;103m[R4] Movendo produto de M2 para saida...\033[0m\\r\n");
		else if(contador == 600)
		  printf("\033[0;103m[R4] Inserindo produto na saida.\033[0m\\r\n");
		else if(contador > 700)
		{
      pegou = pdFALSE;
      contador = 0;
      printf("\033[0;103m[R4] Produto entregue na saída.\033[0m\\r\n");
      xSemaphoreGive(sem_Maquina2);
		}
	  }
    else if (pegou_produto == 3 && pegou)
	  {
		sleep(1);
		contador++;
		if(contador == 1)
		  printf("\033[0;101m[R4] Pegando produto de M3...\033[0m\\r\n");
		else if(contador == 100)
		  printf("\033[0;101m[R4] Movendo produto de M3 para saida...\033[0m\\r\n");
		else if(contador == 600)
		  printf("\033[0;101m[R4] Inserindo produto na saida.\033[0m\\r\n");
		else if(contador > 700)
		{
      pegou = pdFALSE;
      contador = 0;
      printf("\033[0;101m[R4] Produto entregue na saída.\033[0m\\r\n");
      xSemaphoreGive(sem_Maquina3);
		}
	  }
	  vTaskDelay(pdMS_TO_TICKS(1));
	}

}
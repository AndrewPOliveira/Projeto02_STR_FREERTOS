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

int flag_Maquina1 =0, flag_produto1 =0, flag_robo2 = 0, flag_robo3 = 0;
int flag_retira_produto1 = 0;


void vRobo1(void *argument);
void vMaquina1(void *argument);
void vRobo2(void *argument);
void vRobo3(void *argument);

int main(void){
    xTaskCreate(&vRobo1, "Start Robo1", 1024, NULL, 1, NULL);
    xTaskCreate(&vRobo2, "Start Robo2", 1024, NULL, 1, NULL);
    xTaskCreate(&vRobo3, "Start Robo3", 1024, NULL, 1, NULL);
    xTaskCreate(&vMaquina1, "Start Maquina1", 1024, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0;
}

void vRobo1(void *argument){
  static int contador = 0;
  for(;;){
	  if(!flag_Maquina1){
		  if(contador <100)
		  {
			  printf("[R1] Pegando produto da entrada...\r\n");
			  sleep(1);
			  contador++;
		  }
		  else if(contador <600)
		  {
			  printf("[R1] Movendo produto da entrada para M1...\r\n");
			  sleep(1);
			  contador++;
		  }
		  else if(contador <700)
		  {
			  printf("[R1] Inserindo produto em M1.\r\n");
			  sleep(1);
			  contador++;
		  }
		  else
		  {
			  flag_Maquina1 = 1;
			  contador =0;
		  }
	  }
    vTaskDelay(pdMS_TO_TICKS(100));;
  }
}

void vMaquina1(void *argument)
{
  static int contador = 0;
  for(;;){
	  if(flag_Maquina1){
		  if(contador <1500)
		  {
			  printf("[M1] Processando produto...\r\n");
			  sleep(1);
			  contador++;
		  }else
		  {
			  printf("[M1] Produto processado.\r\n");
			  flag_produto1 = 1;
			  contador = 0;
		  }

	  }
    vTaskDelay(pdMS_TO_TICKS(100));;
  }
  /* USER CODE END vMaquina1 */
}


/* USER CODE END Header_vRobo2 */
void vRobo2(void *argument){
  static int contador = 0;
  for(;;){
	  if(flag_produto1 && !flag_robo2 && !flag_retira_produto1)
	  {
		  flag_robo2 = 1;
		  flag_retira_produto1 = 1;
	  }
	  if(flag_robo2)
	  {
		  if(contador <100)
		  {
			  printf("[R2] Pegando produto de M1...\r\n");
			  sleep(1);
			  contador++;
		  }
		  else if(contador <600)
		  {
			  flag_retira_produto1 = 0;
			  flag_Maquina1 = 0;
			  flag_produto1 = 0;
			  printf("[R2] Movendo produto de M1 para M2...\r\n");
			  sleep(1);
			  contador++;
		  }
		  else if(contador <700)
		  {
			  printf("[R2] Inserindo produto em M2.\r\n");
			  sleep(1);
			  contador++;
		  }
		  else
		  {
			  flag_robo2 = 0;
			  contador =0;
		  }
	  }
	  vTaskDelay(pdMS_TO_TICKS(100));;
    }
}
void vRobo3(void *argument){
  static int contador = 0;
  for(;;){
	  if(flag_produto1 && !flag_robo3 && !flag_retira_produto1)
	  {
		  flag_robo3 = 1;
		  flag_retira_produto1 = 1;
	  }
	  if(flag_robo3)
	  {
		  if(contador <100)
		  {
			  printf("[R3] Pegando produto de M1...\r\n");
			  sleep(1);
			  contador++;
		  }
		  else if(contador <900)
		  {
			  flag_retira_produto1 = 0;
			  flag_Maquina1 = 0;
			  flag_produto1 = 0;
			  printf("[R3] Movendo produto de M1 para M3...\r\n");
			  sleep(1);
			  contador++;
		  }
		  else if(contador <1000)
		  {
			  printf("[R3] Inserindo produto em M3.\r\n");
			  sleep(1);
			  contador++;
		  }
		  else
		  {
			  flag_robo3 = 0;
			  contador =0;
		  }
	  }
	  vTaskDelay(pdMS_TO_TICKS(100));;
  }
}
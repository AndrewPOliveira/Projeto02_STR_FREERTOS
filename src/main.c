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
    xTaskCreate(&vRobo3, "Start Robo3", 1024, NULL, 2, NULL);
    xTaskCreate(&vMaquina1, "Start Maquina1", 1024, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0;
}

void vRobo1(void *argument){
  static int contador = 0;
  for(;;){
	  if(!flag_Maquina1)
	  {
		contador++;
		sleep(1);
		  if(contador==1)
			  printf("[R1] Pegando produto da entrada...\r\n");
		  else if(contador==100)
			  printf("[R1] Movendo produto da entrada para M1...\r\n");
		  else if(contador==600)
			  printf("[R1] Inserindo produto em M1.\r\n");
		  else if(contador> 700)
		  {
			  flag_Maquina1 = 1;
			  contador =0;
		  }
	  }
    vTaskDelay(pdMS_TO_TICKS(1));;
  }
}

void vMaquina1(void *argument)
{
  static int contador = 0;
  for(;;){
	  if(flag_Maquina1){
		sleep(1);
		contador++;
		  if(contador ==1)
		  {
			  printf("[M1] Processando produto...\r\n");
		  }else if (contador==1500)
		  {
			  printf("[M1] Produto processado.\r\n");
			  flag_produto1 = 1;
		  }
	  }else{
		contador = 0;
	  }
    vTaskDelay(pdMS_TO_TICKS(1));;
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
		contador++;
		sleep(1);
		  if(contador ==1)
			  printf("[R2] Pegando produto de M1...\r\n");
		  else if(contador ==100)
		  {
			  flag_retira_produto1 = 0;
			  flag_Maquina1 = 0;
			  flag_produto1 = 0;
			  printf("[R2] Movendo produto de M1 para M2...\r\n");
		  }
		  else if(contador ==600)
			  printf("[R2] Inserindo produto em M2.\r\n");
		  else if(contador>700)
		  {
			  flag_robo2 = 0;
			  contador =0;
		  }
	  }
	  vTaskDelay(pdMS_TO_TICKS(1));;
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
		contador++;
		sleep(1);
		  if(contador ==1)
			  printf("[R3] Pegando produto de M1...\r\n");
		  else if(contador ==100)
		  {
			  flag_retira_produto1 = 0;
			  flag_Maquina1 = 0;
			  flag_produto1 = 0;
			  printf("[R3] Movendo produto de M1 para M3...\r\n");
		  }
		  else if(contador ==900)
			  printf("[R3] Inserindo produto em M3.\r\n");
		  else if (contador > 1000)
		  {
			  flag_robo3 = 0;
			  contador =0;
		  }
	  }
	  vTaskDelay(pdMS_TO_TICKS(1));;
  }
}
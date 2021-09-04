/****
  * @file    main.c
  * @author  G. Garcia & A. Riedinger.
  * @version 0.1
  * @date    29-08-21
  * @brief   Generacion de una senal de 350 Hz cuyo ciclo de trabajo puede ser modificado
  * 		 al pulsar el UserButton. La tension de la senal se incrementa en intervalos
  * 		 de 0.5 V cada vez que se pulsa el UserButton hasta llegar a VDD.

  * SALIDAS:
  	  *	LCD
  	  *	OC1	 		PD12
  *
  * ENTRADAS:
  	  * UserButton	PC13
  ****/

/*------------------------------------------------------------------------------
LIBRERIAS:
------------------------------------------------------------------------------*/
#include "mi_libreria.h"

/*------------------------------------------------------------------------------
DEFINICIONES:
------------------------------------------------------------------------------*/
//Tiempo de interrupcion por Systick - 50mseg:
#define TimeINT_Systick 0.05

//Ticks del despachador de tareas:
#define Ticks_ReadUserButton 2
#define Ticks_RefreshTIM4	 4

//User Button:
#define UserButton_Port	GPIOC
#define UserButton		GPIO_Pin_13

//Oscilador 1 del TIMER 4:
#define OC1_Port	GPIOD
#define OC1			GPIO_Pin_12

//Parametros del OC1:
#define TimeBase 	200000	//200kHz
#define Freq		350 	//350 Hz frecuencia de trabajo

/*------------------------------------------------------------------------------
DECLARACION DE FUNCIONES LOCALES:
------------------------------------------------------------------------------*/
void READ_USER_BUTTON(void);
void REFRESH_TIM4(void);

/*------------------------------------------------------------------------------
DECLARACION VARIABLES GLOBALES:
------------------------------------------------------------------------------*/
//Definicion de los pines del LCD:
LCD_2X16_t LCD_2X16[] = {
			// Name  , PORT ,   PIN      ,         CLOCK       ,   Init
			{ TLCD_RS, GPIOC, GPIO_Pin_10, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_E,  GPIOC, GPIO_Pin_11, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D4, GPIOC, GPIO_Pin_12, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D5, GPIOD, GPIO_Pin_2,  RCC_AHB1Periph_GPIOD, Bit_RESET },
			{ TLCD_D6, GPIOF, GPIO_Pin_6,  RCC_AHB1Periph_GPIOF, Bit_RESET },
			{ TLCD_D7, GPIOF, GPIO_Pin_7,  RCC_AHB1Periph_GPIOF, Bit_RESET }, };

//Variable de control del ciclo de trabajo de la senal a generar:
uint32_t DutyCycle = 50;

//Variable para contar cuantas veces se pulso el boton:
uint32_t Pulses = 0;

//Variables del TS:
uint32_t ReadUserButton = 0;
uint32_t RefreshTIM4 = 0;

int main(void)
{
/*------------------------------------------------------------------------------
CONFIGURACION DEL MICRO:
------------------------------------------------------------------------------*/
	SystemInit();

	//Inicializacion del DISPLAY LCD:
	INIT_LCD_2x16(LCD_2X16);

	//Inicializacion del TIM4:
	INIT_TIM4(OC1_Port, OC1);

/*------------------------------------------------------------------------------
BUCLE PRINCIPAL:
------------------------------------------------------------------------------*/
    while(1)
    {
		//Task Scheduler:
		if (ReadUserButton == Ticks_ReadUserButton)
			READ_USER_BUTTON();
		else if (RefreshTIM4 == Ticks_RefreshTIM4)
			REFRESH_TIM4();
    }
}

/*------------------------------------------------------------------------------
INTERRUPCIONES:
------------------------------------------------------------------------------*/
//Interrupcion por tiempo - Systick cada 50mseg:
void SysTick_Handler()
{
	ReadUserButton++;
	RefreshTIM4++;
}

/*------------------------------------------------------------------------------
TAREAS:
------------------------------------------------------------------------------*/
void READ_USER_BUTTON()
{
	READ_DI(UserButton_Port, UserButton);
}

void REFRESH_TIM4()
{
	SET_TIM4(OC1, TimeBase, Freq, DutyCycle);
}


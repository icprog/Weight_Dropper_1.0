/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "main.h"

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "ADC_example.h"
#include "I2C_controller.h"
//#include "FSM.h"
#include "status_leds.h"
#include "stm32f0xx_tim.h"
#include "Buffer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*
 * I2C2_semaphore_control is used when a task want to take control of the I2C2 bus
 * This does not necessarily mean that the task is using the I2C2 bus it just
 * means that it has a use for it and needs to block it from other tasks
 */
static SemaphoreHandle_t I2C2_semaphore_control;


void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}
/*-----------------------------------------------------------*/

void CreateSemaphores(void){
	//The I2C2 Semaphore to safeguard tasks from overlapping usage of the bus
	I2C2_semaphore_control = xSemaphoreCreateBinary();
	//Just double check that initialized successfully
	configASSERT(I2C2_semaphore_control);
	//For some reason the semaphore start empty so we must free it
	xSemaphoreGive(I2C2_semaphore_control);
}

int main(void)
{

	blink_led_C8_C9_init();

	/*
	SetClockForADC();
	SetClockForADC();
	CalibrateADC();
	EnableADC();
	ConfigureADC();
	ConfigureTIM15();
	ADC1->CR |= ADC_CR_ADSTART;
	*/


	Configure_GPIO_I2C2();
	Configure_I2C2_Master();


	//init_FSM();


	//Configure_GPIO_USART1();
	//Configure_USART1();

	//safety checks

	vBootTaskInit();
	CreateSemaphores();

	/* Start the kernel.  From here on, only tasks and interrupts will run. */
	vTaskStartScheduler();

	//This should never happen
	while(1);
}


void bootUpSeq(void *dummy){
	vGeneralTaskInit();
	vTaskDelete(NULL);
}

void blinkyTask(void *dummy){
	while(1){
		GPIOA->ODR ^= GPIO_ODR_3;
		/* maintain LED3 status for 200ms */
		vTaskDelay(500);
	}
}

void vBootTaskInit(void){
	xTaskCreate(bootUpSeq,
		(const signed char *)"boot",
		configMINIMAL_STACK_SIZE,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL               );///* pvCreatedTask */
}

void vGeneralTaskInit(void){
    xTaskCreate(blinkyTask,
		(const signed char *)"blinkyTask",
		configMINIMAL_STACK_SIZE,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL              ); // pvCreatedTask */
}

void TIM16_IRQHandler(void)//Once per second
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET){
		GPIOC->ODR ^= GPIO_ODR_9;
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
	}
}

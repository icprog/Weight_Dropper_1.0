/*
 * I2C_manager.c
 *
 *  Created on: Aug 11, 2016
 *      Author: auvic
 */
#include "stm32f0xx.h"
#include "I2C_controller.h"
#include <string.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Local global variables
void (*data_update_callback)(uint8_t);
uint8_t (*data_to_send_update)(void);

TaskHandle_t xTaskToNotify = NULL;

extern void Configure_GPIO_I2C2(void)
{
  /* Enable the peripheral clock of GPIOB */
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

  /* (1) Open drain for I2C signals */
  /* (2) AF1 for I2C signals */
  /* (3) Select AF mode (10) on PB6 and PB7 */
  GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; /* (1) */
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~(GPIO_AFRH_AFRH7 | GPIO_AFRH_AFRH6)) \
                  | (1 << (7 * 4)) | (1 << (6 * 4)); /* (2) */
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODER7 | GPIO_MODER_MODER6)) \
                 | (GPIO_MODER_MODER7_1 | GPIO_MODER_MODER6_1); /* (3) */

}

extern void Configure_I2C2_Master(void)
{
  /* Enable the peripheral clock I2C2 */
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  RCC->CFGR3 |= RCC_CFGR3_I2C1SW;

  /* Configure I2C2, master */
  /* (1) Timing register value is computed with the AN4235 xls file,
   fast Mode @400kHz with I2CCLK = 48MHz, rise time = 140ns, fall time = 40ns */
  /* (2) Periph enable */
  /* (3) Slave address = 0x5A, write transfer, 1 byte to transmit, autoend */
  I2C1->TIMINGR = (uint32_t)0x00B01A4B; /* (1) */
  I2C1->CR1 = I2C_CR1_PE; /* (2) */
  I2C1->CR2 =  I2C_CR2_AUTOEND | (1<<16) | (Humidty_sensor<<1); /* (3) */

  /* Configure IT */
  /* (4) Set priority for I2C2_IRQn */
  /* (5) Enable I2C2_IRQn */
  NVIC_SetPriority(I2C1_IRQn, 6); /* (4) */
  NVIC_EnableIRQ(I2C1_IRQn); /* (5) */
}

static void I2C2_set_address_and_byte_count(uint8_t address, uint8_t bytes_to_send){
	I2C1->CR2 =  (address<<1);
	I2C1->CR2 |= (bytes_to_send << 16);
}

static void reset_possibly_set_config_options(){
	I2C1->CR1 &= (~(I2C_CR1_RXIE | I2C_CR1_TXIE | I2C_CR1_TCIE));
	I2C1->CR2 &= (~I2C_CR2_RD_WRN);
}

static void set_task_to_notify_handle(){
	/* Store the handle of the calling task. */
	xTaskToNotify = xTaskGetCurrentTaskHandle();
}

extern void I2C1_IRQHandler(void){
	if((I2C1->ISR & I2C_ISR_TXIS) == I2C_ISR_TXIS){
		I2C1->TXDR = data_to_send_update();
	}
	if((I2C1->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE){
		data_update_callback(I2C1->RXDR);
	}
	if((I2C1->ISR & I2C_ISR_TC) == I2C_ISR_TC){
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		/* At this point xTaskToNotify should not be NULL as a transmission was
		in progress. */
		configASSERT( xTaskToNotify != NULL );

		/* Notify the task that the transmission is complete. */
		vTaskNotifyGiveFromISR( xTaskToNotify, &xHigherPriorityTaskWoken );
		I2C1->CR2 |=  I2C_CR2_STOP;
	}
}

extern void I2C2_send_message_no_cb(uint8_t message, uint8_t address, uint8_t bytes_to_send){
	I2C2_set_address_and_byte_count(address, bytes_to_send);
	I2C1->TXDR = message;
	reset_possibly_set_config_options();
	set_task_to_notify_handle();
	I2C1->CR1 |= I2C_CR1_TCIE;
	I2C1->CR2 |= I2C_CR2_START;
}

extern void I2C2_send_message_with_cb(uint8_t address, uint8_t bytes_to_send, uint8_t (*callback_func)(void)){
	I2C2_set_address_and_byte_count(address, bytes_to_send);
	data_to_send_update = callback_func;
	I2C1->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE;
	reset_possibly_set_config_options();
	set_task_to_notify_handle();
	I2C1->TXDR = data_to_send_update();
	I2C1->CR2 |=  I2C_CR2_START;
}

extern void I2C2_recv_message_with_cb(int address, int bytes_to_send, void (*callback_func)(uint8_t)){
	I2C2_set_address_and_byte_count(address, bytes_to_send);
	data_update_callback = callback_func;
	reset_possibly_set_config_options();
	set_task_to_notify_handle();
	I2C1->CR1 |= I2C_CR1_RXIE | I2C_CR1_TCIE;
	I2C1->CR2 |= I2C_CR2_RD_WRN | I2C_CR2_START;
}

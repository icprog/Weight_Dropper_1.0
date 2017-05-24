/*
 * solenoid_driver.c
 *
 *  Created on: Apr 1, 2017
 *      Author: abates
 */
#include "stm32f0xx.h"
#include "solenoid_driver.h"

#include "FreeRTOS.h"
#include "timers.h"

static xTimerHandle xSafetyStopTimer;
static const long timer_id = 0x1234;

/* SOLENOID_TIMER_COUNTER_DUTY_CYCLE
 * Values in array determined experimentally for 16V at start of array
 * to 26V at the end of the array. (1V increments). eg: 650 is 65% duty
 * cycle for 16V to 17V.
 */
const int duty_cycle[11] = {650, 650, 600, 550, 500, 500, 500, 450, 450, 400, 400};

static void vSafetyStopTimerCallback( TimerHandle_t *pxTimer )
{
	if( (long)pvTimerGetTimerID(pxTimer) == timer_id)
		LEFT_SOLENOID_CCR = 0;
		RIGHT_SOLENOID_CCR = 0;
}

static void timer3_it_config(void){
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 24kHz
	TIM_TimeBaseStructure.TIM_Prescaler = 2 - 1; // 24 MHz Clock (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;

	//initialize structs
	GPIO_InitTypeDef GPIO_struct;

	//initialize the LEDs
	GPIO_struct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_struct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_struct);

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 0; //0%
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM2, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM2, &outputChannelInit);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_2);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

static void software_timer_config(void){
	xSafetyStopTimer = xTimerCreate(
		"TurnOffTime",				/* Just a name */
		SOLENOID_TIME_ON_MS / portTICK_RATE_MS,	/* Configure timer for 5 seconds */
		pdFALSE,					/* Disable auto-reload */
		(void *)timer_id,			/* Unique timer identifier */
		vSafetyStopTimerCallback);	/* Specify callback function */
}

void solenoid_driver_init(void){
	timer3_it_config();
	software_timer_config();
}

void turn_on_solenoid(Solenoids solenoid, uint16_t dutyCyclePercent10000){
	if(solenoid == left_solenoid){
		LEFT_SOLENOID_CCR = dutyCyclePercent10000;
	}else if(solenoid == right_solenoid){
		RIGHT_SOLENOID_CCR = dutyCyclePercent10000;
	}else{
		//Shouldn't get here
	}
	xTimerReset(xSafetyStopTimer, 100);
}

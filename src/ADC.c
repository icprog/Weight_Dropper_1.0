//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "ADC.h"
#include "stm32f0xx.h"
#include "system_info.h"

//#define calculation_constant (18*1*1000/4096)
#define calculation_constant 8 // approximately = (3*10*1000/4096)

// ----------------------------------------------------------------------------
uint16_t battery_voltage = 0;

int buffer_sum = 0;
int circular_buffer[8] = {0};
int index = 0;

void doADC(void *dummy){

		ADC_InitTypeDef ADC_InitStructure;

		/*peripheral clock for ADC1 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

		/* Deinitializes ADC1 peripheral registers to their default reset values*/
		ADC_DeInit(ADC1);

		ADC_StructInit(&ADC_InitStructure);

		/* ADC1 Configuration Settings  */
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;

		ADC_Init(ADC1, &ADC_InitStructure);

		/* Using channel 3 with 239.5 Cycles sampling time */
		ADC_ChannelConfig(ADC1, ADC_Channel_3 , ADC_SampleTime_239_5Cycles);

		/* Enable the ADC */
		ADC_Cmd(ADC1, ENABLE);

		/* Wait for the ADRDY flag to check ADC is ready */
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

		/*Start conversion */
		ADC_StartOfConversion(ADC1);
		while(1){
			unsigned int voltage_steps = ADC1->DR; //get value from adc

			/* divide by resolution of adc (4096 steps)
			 * multiply by VDD = 3V
			 * compensate for voltage divider with 10x
			 * multiply by 1000 for mV*/
			battery_voltage = (voltage_steps*calculation_constant);
			/*
			 * Creating circular buffer of eight values:
			 */
			buffer_sum = buffer_sum - circular_buffer[index % 8] + battery_voltage;
			circular_buffer[index % 8] = battery_voltage;
			index++;

			vTaskDelay(500);
		}
}
uint16_t current_avg_voltage_mv(){

	return buffer_sum/8;
}

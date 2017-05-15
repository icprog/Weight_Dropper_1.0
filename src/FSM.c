/*
 * FSM.c
 *
 *  Created on: Feb 3, 2017
 *      Author: asus-andy
 */
#include "FSM.h"
#include "Buffer.h"
#include "UART.h"
#include "solenoid_driver.h"

#include "FreeRTOS.h"
#include "task.h"

void FSM_Init(){
	Buffer_init(&inputBuffer);
}

void FSM(void *dummy){
	//initialize the FSM and UART
	FSM_Init();
	UART_init();

	inputBuffer.size = 0;

	//temporary storage to return from buffer
	char commandString[MAX_BUFFER_SIZE] = "";
	int tempVar;

	while(1){
		//it's important that this is while, if the task is accidentally awaken it
		//can't execute without having at least one item the input puffer
		while(inputBuffer.size == 0){
			//sleeps the task into it is notified to continue
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		}
		//Write a statement here to do a string comparison on commands
		Buffer_pop(&inputBuffer, commandString);
		char arguement = commandString[3];
		commandString[3] = '\0';

		char tempOutputString[MAX_BUFFER_SIZE] = "";

		//convert to volts and shift to array value
		int temp = (int)((((current_avg_voltage_mv())+500)/1000) - 16);
				if(temp < 0){
					temp = 0;
				}
				else if(temp > 10){
					temp = 10;
				}

		if(strcmp(commandString, "DPR") == 0){
			turn_on_solenoid(right_solenoid, duty_cycle[temp]);
			UART_push_out("Right\r\n");
		}
		else if(strcmp(commandString, "DPL") == 0){
			turn_on_solenoid(left_solenoid, duty_cycle[temp]);
			UART_push_out("Left\r\n");
		}
		else if(strcmp(commandString, "RID") == 0){
			UART_push_out("Weight ");
			UART_push_out("Dropper");
			UART_push_out("\r\n");
		}
		else{
			UART_push_out("error: ");
			UART_push_out(commandString);
			UART_push_out("\r\n");
		}
	}
}

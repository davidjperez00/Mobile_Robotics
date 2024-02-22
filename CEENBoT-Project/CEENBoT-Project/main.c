/*
 * Auth:   David Perez
 * Date:   2/21/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 3
 * Desc :  Main file for controlling the CEENBoT robot.
 */ 

#include "capi324v221.h"
#include "stdbool.h"
#include "src/app.h"
#include "avr/interrupt.h"

void CBOT_main(void)
{
	
	// Initialize the servo motor system
	STEPPER_open();
		
	// enable interrupts?
	//sei();
	
	// Loop forever
    while (1);
}


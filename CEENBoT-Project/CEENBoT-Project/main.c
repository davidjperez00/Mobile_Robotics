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

void CBOT_main(void)
{
	// Initialize the LCD subsystem
	LCD_open();
	
	// Initialize the servo motor system
	STEPPER_open();

	// Initialize the ADC subsystem
	ADC_open();

	// Set the voltage reference (we're using 5v reference).
	ADC_set_VREF(ADC_VREF_AVCC);
		
	app_main(SS_EXCITATORY);
}


/*
 * Auth:   David Perez
 * File:   app.c
 * Date:   2/21/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 3
 * Desc :  Provides application level control for IR sensor object avoidance.
 */ 

#include "app.h"
#include "motor_control.h"
#include "capi324v221.h"

float read_right_pr_sensor_voltage (void)
{
	// Set the channel we will sample from (right photo-resistor sensor)
	ADC_set_channel(ADC_CHAN3);

	// Sample the ADC
	ADC_SAMPLE sample = ADC_sample();

	// Convert the ADC sample to a voltage
	return ( ( sample * 5.0f) / 1024);
}

float read_left_pr_sensor_voltage (void)
{
	// Set the channel we will sample from (left photo-resistor sensor)
	ADC_set_channel(ADC_CHAN4);

	// Sample the ADC
	ADC_SAMPLE sample = ADC_sample();

	// Convert the ADC sample to a voltage
	return ( ( sample * 5.0f) / 1024);
}

// @brief: Avoid obstacles using the IR sensors.
void IR_AVOID (void)
{
	// If both sensors detect object turn the robot
	// in an arbitrary direction (left is chosen)
	if (ATTINY_get_IR_state(ATTINY_IR_BOTH)){
		
		// Stop the robots forward motion
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);

		// Back up robot away from object
		move_backwards_in_inches_stwt(3);
		
		// Turn right
		reverse_backup_60_degrees(false);
		
		return;
	}
	
	// Check if object is detected to the left of the CEENBoT
	if (ATTINY_get_IR_state(ATTINY_IR_LEFT)){
		
		// Stop the robot
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
		
		// Back up robot away from object
		move_backwards_in_inches_stwt(3);
		
		// Turn right
		reverse_backup_60_degrees(false);
		
		return;
		
	}
	
	// Check if object is detected in to the right of the CEENBoT
	if (ATTINY_get_IR_state(ATTINY_IR_RIGHT)){
			
		// Stop the robot
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
		
		// Back up robot away from object
		move_backwards_in_inches_stwt(3);
				
		// Turn right
		reverse_backup_60_degrees(true);	
		
		return;
	}
}

void CRUISE (void)
{
	// Drive the robot forward 4 inches non-blocking function
	move_forward_in_inches_stnb(4, 150);
	
}

// @brief: Provides higher level control between CRUISE()
//     and IR_AVOID().
void app_main (void)
{	
	while ( 1 ) {
		
		IR_AVOID();
		
		CRUISE();	
	
	}
}
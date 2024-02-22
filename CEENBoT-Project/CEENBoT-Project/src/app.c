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

//enum {
	//
//} DetectionState;

// @brief: Avoid obstacles using the IR sensors.
void IR_AVOID (void)
{
	// If both sensors detect object turn the robot
	// in an arbitrary direction (left is chosen)
	if (ATTINY_get_IR_state(ATTINY_IR_BOTH)){
		
		// ?? Turn the robot left
		// Stop the robot and back up
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
		
		return;
	}
	
	// Check if object is detected in front of the CEENBoT
	if (ATTINY_get_IR_state(ATTINY_IR_LEFT)){
		
		// Stop the robot
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
		
		// back up?
		// turn the left wheel simultaneously as right wheel?
		move_backwards_in_inches_stwt();
		
		// Turn right
		turn_90_degrees(false);
		
		return;
		
	}
	
	// Check if object is detected in front of the CEENBoT
	if (ATTINY_get_IR_state(ATTINY_IR_RIGHT)){
			
		// Stop the robot
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
				
		// Turn right
		turn_90_degrees(true);
		
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
	//int i = 0;
	
	while ( 1 ) {
		
		IR_AVOID();
		
		CRUISE();	
	
		//i++;
	}
	
	
	
}
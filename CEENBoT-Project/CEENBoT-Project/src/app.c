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

// @brief: Avoid obstacles using the IR sensors.
void IR_AVOID (void)
{
	// If both sensors detect object turn the robot
	// in an arbitrary direction (left is chosen)
	
	

	// Check if object is detected in front of the CEENBoT
	bool object_detected = ATTINY_get_IR_state();

	// Get the state of the push botton switch ?
	// ATTINY_get_SW_state();


	// Perform some motor action


}

void CRUISE (void)
{
	// Drive the robot forward 4 inches
	move_forward_in_inches_stnb(4);
	
}

// @brief: Provides higher level control between CRUISE()
//     and IR_AVOID().
void app_main (void)
{
	
	
}
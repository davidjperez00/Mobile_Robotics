/*
 * Auth:   David Perez
  * File:  motor_control.c
 * Date:   2/21/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 3
 * Desc :  Provides motor control functionality to drive servo motors.
 */ 

#include "motor_control.h"
#include "capi324v221.h"
#include <math.h>


# define pi 3.14159265359
# define WHEEL_RADIUS 1.6250 // inches
# define CIRCUMFERENCE (2 * pi * WHEEL_RADIUS) // inches
# define SPR 200 // stepper motor steps per revolution
// Approximate Distance Per Step for each stepper motor step (in inches)
# define DPS (CIRCUMFERENCE / SPR)

double steps_per_distance(double distance_inches)
{
	return distance_inches / DPS;
}

void move_forward_in_inches_stwt(double distance)
{
	// Get approximate number of stepper motor steps to move
	// the robot forward.
	double num_steps = steps_per_distance(distance);
	
	double left_additional_steps = (0.015 * num_steps);
	
	// The longer the distance the greater the overshoot.
	num_steps = num_steps - (0.0185 * num_steps);
	
	// Compensate for shorter distance undershoots
	num_steps += 4;
	
	// Move Both wheels forward
	STEPPER_move_stwt(STEPPER_BOTH,
	STEPPER_FWD, floor(num_steps+left_additional_steps), 400, 200, STEPPER_BRK_OFF,  // left
	STEPPER_FWD, floor(num_steps), 400, 200, STEPPER_BRK_OFF); // right
}

// @brief: Moves the motor by 'inches' using
//     non-blocking motor control.
void move_forward_in_inches_stnb(double distance, int velocity)
{
	// Get approximate number of stepper motor steps to move
	// the robot forward.
	double num_steps = steps_per_distance(distance);
	
	double left_additional_steps = (0.015 * num_steps);
	
	// The longer the distance the greater the overshoot.
	num_steps = num_steps - (0.0185 * num_steps);
	
	// Compensate for shorter distance undershoots
	num_steps += 4;
	
	// Move Both wheels forward with non-blocking function
	STEPPER_move_stnb(STEPPER_BOTH,
	STEPPER_FWD, floor(num_steps+left_additional_steps), velocity, 200, STEPPER_BRK_OFF,  // left
	STEPPER_FWD, floor(num_steps), velocity, 200, STEPPER_BRK_OFF); // right
}

// @brief: Move backwards in inches with blocking
void move_backwards_in_inches_stwt(double distance)
{
	// Get approximate number of stepper motor steps to move
	// the robot forward.
	double num_steps = steps_per_distance(distance);
	
	double left_additional_steps = (0.015 * num_steps);
	
	// The longer the distance the greater the overshoot.
	num_steps = num_steps - (0.0185 * num_steps);
	
	// Compensate for shorter distance undershoots
	num_steps += 4;
	
	// Move Both wheels forward
	STEPPER_move_stwt(STEPPER_BOTH,
	STEPPER_REV, floor(num_steps+left_additional_steps), 200, 200, STEPPER_BRK_OFF,  // left
	STEPPER_REV, floor(num_steps), 200, 200, STEPPER_BRK_OFF); // right
}

void turn_90_degrees(bool turn_left)
{
	if (turn_left)
	{
		// Turn left (~90 degrees)
		STEPPER_move_stwt(STEPPER_BOTH,
		STEPPER_REV, 131.5, 600, 400, STEPPER_BRK_OFF,  // left
		STEPPER_FWD, 115, 600, 400, STEPPER_BRK_OFF); // right
		
		} else {
		// Turn right (~90 degrees)
		STEPPER_move_stwt(STEPPER_BOTH,
		STEPPER_FWD, 115, 600, 400, STEPPER_BRK_OFF,  // left
		STEPPER_REV, 130.5, 600, 400, STEPPER_BRK_OFF); // right
	}
}

// 90/115 = 1.2777




void reverse_backup_60_degrees(bool turn_left)
{
	if (turn_left)
	{
		// Turn left (~90 degrees)
		STEPPER_move_stwt(STEPPER_BOTH,
		STEPPER_REV, 175, 200, 400, STEPPER_BRK_OFF,  // left
		STEPPER_REV, 30, 200, 400, STEPPER_BRK_OFF); // right
		
		} else {
		// Turn right (~90 degrees)
		STEPPER_move_stwt(STEPPER_BOTH,
		STEPPER_REV, 30, 200, 400, STEPPER_BRK_OFF,  // left
		STEPPER_REV, 175, 200, 400, STEPPER_BRK_OFF); // right
	}
}
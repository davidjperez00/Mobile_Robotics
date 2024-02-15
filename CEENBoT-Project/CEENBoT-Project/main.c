/*
 * Auth:   David Perez
 * Date:   Today's Date
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 2
 * Desc :  TODO
 */ 

#include "capi324v221.h"
#include <math.h> 
#include "stdbool.h"

# define pi 3.14159265359
# define WHEEL_RADIUS 1.6250 // inches
# define CIRCUMFERENCE (2 * pi * WHEEL_RADIUS) // inches
# define SPR 200 // stepper motor steps per revolution
// Approximate Distance Per Step for each stepper motor step (in inches)
# define DPS (CIRCUMFERENCE / SPR)


// Counter for lab sub parts
typedef enum{
	LP1_0,
	LP1_1,
	LP1_2,
	LP1_3,
	NUM_LABPART1,
} LabPart1;

double steps_per_distance(double distance_inches)
{
	return distance_inches / DPS;
}


void move_forward_in_inches(double distance)
{
	// Get approximate number of stepper motor steps to move
	// the robot forward.
	double num_steps = steps_per_distance(distance);
	
	// TODO: Determine necessary motor control options
			// before engaging motors
			
	double left_additional_steps = (0.004 * num_steps);
	//double right_additional_steps = 0;
	
	// The longer the distance the greater the overshoot.
	// Reduce each distance by .8%.
	num_steps = num_steps - (0.006 * num_steps);
	
	// Compensate for shorter distance undershoots
	num_steps += 2;
	
	// Move Both wheels forward
	STEPPER_move_stwt(STEPPER_BOTH,
	STEPPER_FWD, floor(num_steps+left_additional_steps), 200, 200, STEPPER_BRK_OFF,  // left
	STEPPER_FWD, floor(num_steps), 200, 200, STEPPER_BRK_OFF); // right
		
	// todo: consider more steps here for difference distances
	//			or make control system for correction.
	
}

// Changing to higher speed
void move_forward_in_inches_p1_1(double distance)
{
	// Get approximate number of stepper motor steps to move
	// the robot forward.
	double num_steps = steps_per_distance(distance);
	
	// TODO: Determine necessary motor control options
	// before engaging motors
	
	double left_additional_steps = (0.0154 * num_steps);
	//double right_additional_steps = 0;
	
	// The longer the distance the greater the overshoot.
	// Reduce each distance by .8%.
	num_steps = num_steps - (0.018 * num_steps);
	
	// Compensate for shorter distance undershoots
	num_steps += 4;
	
	// Move Both wheels forward
	STEPPER_move_stwt(STEPPER_BOTH,
	STEPPER_FWD, floor(num_steps+left_additional_steps), 400, 200, STEPPER_BRK_OFF,  // left
	STEPPER_FWD, floor(num_steps), 400, 200, STEPPER_BRK_OFF); // right
	
	// todo: consider more steps here for difference distances
	//			or make control system for correction.
	
}

void lab_1_app(void)
{
	// Index for robot movement tasks
	LabPart1 lab_index = LP1_0;
	
	// Execute each movement for lab 2 part 1
	while (lab_index < NUM_LABPART1)
	{
		switch (lab_index) {
			case LP1_0:
			// Drive 6 inches forward
			move_forward_in_inches_p1_1(6);
			
			break;

			case LP1_1:
			// Drive 12 inches forward
			move_forward_in_inches_p1_1(12);
			
			break;
			case LP1_2:
			// Drive 18 inches forward
			move_forward_in_inches_p1_1(18);
			
			break;
			
			case LP1_3:
			// Drive 24 inches forward
			move_forward_in_inches_p1_1(24);
			
			break;
			case NUM_LABPART1:
			default:
			// intentional fall through
			return;
			
		}
		
		// Delay 9 seconds.
		TMRSRVC_delay( TMR_SECS( 20 ) );

		// Proceed to next experiment
		lab_index++;
	}
	
}

void turn_90_degrees(bool turn_left)
{
	if (turn_left) 
	{
		// Turn left (~90 degrees)
		STEPPER_move_stwt(STEPPER_BOTH,
		STEPPER_REV, 129, 600, 400, STEPPER_BRK_OFF,  // left
		STEPPER_FWD, 115, 600, 400, STEPPER_BRK_OFF); // right
		
	} else {
		// Turn right (~90 degrees)
		STEPPER_move_stwt(STEPPER_BOTH,
		STEPPER_FWD, 115, 600, 400, STEPPER_BRK_OFF,  // left
		STEPPER_REV, 129, 600, 400, STEPPER_BRK_OFF); // right
	}
}

// Move robot in a 4ft by 4ft square.
void lab_p2_app(void)
{
	for (int i = 0; i < 3; i++)
	{
		// Drive forward by 4ft (48 inches)
		move_forward_in_inches_p1_1(48);

		// Turn right 90 degrees.
		turn_90_degrees(false);
		
		// Delay 20 seconds.
		TMRSRVC_delay( TMR_SECS( 6 ) );
		
		if (i == 2) {
			// last iteration completes the square extra 
			// turn not required
			move_forward_in_inches_p1_1(48);
		}
	}
}


// Drive the robot around a predetermined maze
// Maze course:
/*
	 __5ft___      ___2ft__
	|       |     |        |
 4ft|		| 2ft | 2ft    |
	^		|_____|        | 4ft
	|         2ft          |
    -----------------------
			9ft
*/
void lab_p3_app(void)
{
	// Go forward 4ft then turn right
	move_forward_in_inches_p1_1(48);
	turn_90_degrees(false);
	//TMRSRVC_delay( TMR_SECS( 3 ) );
		
	// Drive forward 5 ft then turn right
	move_forward_in_inches_p1_1(60);
	turn_90_degrees(false);
	//TMRSRVC_delay( TMR_SECS( 0.5 ) );
	
	// Drive 2 ft then turn left
	move_forward_in_inches_p1_1(24);
	turn_90_degrees(true);
	//TMRSRVC_delay( TMR_SECS( 0.5 ) );

	// Drive 2 ft then turn left
	move_forward_in_inches_p1_1(24);
	turn_90_degrees(true);
	//TMRSRVC_delay( TMR_SECS( 0.5 ) );
	
	// Drive 2 ft then turn right	
	move_forward_in_inches_p1_1(24);
	turn_90_degrees(false);
	//TMRSRVC_delay( TMR_SECS( 0.5 ) );
	
	// Drive 2 ft then turn right
	move_forward_in_inches_p1_1(24);
	turn_90_degrees(false);
	//TMRSRVC_delay( TMR_SECS( 0.5 ) );
	
	// Drive 4 ft then turn right
	move_forward_in_inches_p1_1(48);
	turn_90_degrees(false);
	//TMRSRVC_delay( TMR_SECS( 0.5 ) );
	
	// Drive 9 ft
	move_forward_in_inches_p1_1(108);
}


void CBOT_main(void)
{
	// Open STEPPER module for use
	STEPPER_open();

	// perform lab 2 p 1 tasks:
	//lab_1_app();
	
	// Lab 2 p 2:
	//lab_p2_app();
	
	// Lab 2 p 3:
	lab_p3_app();
	
	

	// moc def:
	// STEPPER_move_stwt(motors, motor_direction, num_steps, steps_per_sec, acceleration (in steps/sec^2), brake_on_or_off)

	//// Move Both wheels forward
	//STEPPER_move_stwt(STEPPER_BOTH,
		//STEPPER_FWD, 500, 200, 400, STEPPER_BRK_OFF,  // left
		//STEPPER_FWD, 500, 200, 400, STEPPER_BRK_OFF); // right
		//
	//// Turn right (~90 degrees)
	//STEPPER_move_stwt(STEPPER_BOTH,
		//STEPPER_FWD, 150, 200, 400, STEPPER_BRK_OFF,  // left
		//STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF); // right
//
	//// Move both wheels forward
	//STEPPER_move_stwt(STEPPER_BOTH,
		//STEPPER_FWD, 500, 200, 400, STEPPER_BRK_OFF,  // left
		//STEPPER_FWD, 500, 200, 400, STEPPER_BRK_OFF); // right


	

}


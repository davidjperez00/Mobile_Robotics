/*
 * Auth:   David Perez
 * File:   apps.c
 * Date:   2/18/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 2
 * Desc :  Provides application control for each lab path.
 */ 

#include "apps.h"
#include "motor_control.h"
#include "capi324v221.h"

// @brief: Performs part 1 of lab to to drive the robot 6,
//     12, 18, and 24 inches forward.
void app_lab_part1(void)
{
	// Index for each robot movement tasks
	LabPart1 lab_index = LP1_0;
	
	// Execute each movement for lab 2 part 1
	while (lab_index < NUM_LABPART1)
	{
		switch (lab_index) {
			case LP1_0:
			// Drive 6 inches forward
			move_forward_in_inches(6);
			
			break;

			case LP1_1:
			// Drive 12 inches forward
			move_forward_in_inches(12);
			
			break;
			case LP1_2:
			// Drive 18 inches forward
			move_forward_in_inches(18);
			
			break;
			
			case LP1_3:
			// Drive 24 inches forward
			move_forward_in_inches(24);
			
			break;
			case NUM_LABPART1:
			default:
			// intentional fall through
			return;
			
		}
		
		// Delay 10 seconds to allow setup for
		// next run
		TMRSRVC_delay( TMR_SECS( 10 ) );

		// Proceed to next experiment
		lab_index++;
	}
}

// @brief: Move robot in a 4ft by 4ft square.
void app_lab_part2(void)
{
	for (int i = 0; i < 3; i++)
	{
		// Drive forward by 4ft (48 inches)
		move_forward_in_inches(48);

		// Turn right 90 degrees.
		turn_90_degrees(false);
		
		// Delay 6 seconds.
		TMRSRVC_delay( TMR_SECS( 6 ) );
		
		if (i == 2) {
			// last iteration completes the square extra
			// turn not required
			move_forward_in_inches(48);
		}
	}
}

// @brief: Drive the robot around a predetermined the
//     following maze course:
/*
    __5ft___      ___2ft__
    |       |     |        |
 4ft|       | 2ft | 2ft    |
    ^       |_____|        | 4ft
    |         2ft          |
    -----------------------
            9ft
*/
void app_lab_part3(void)
{
	// Go forward 4ft then turn right
	move_forward_in_inches(48);
	turn_90_degrees(false);
		
	// Drive forward 5 ft then turn right
	move_forward_in_inches(60);
	turn_90_degrees(false);
	
	// Drive 2 ft then turn left
	move_forward_in_inches(24);
	turn_90_degrees(true);

	// Drive 2 ft then turn left
	move_forward_in_inches(24);
	turn_90_degrees(true);
	
	// Drive 2 ft then turn right	
	move_forward_in_inches(24);
	turn_90_degrees(false);
	
	// Drive 2 ft then turn right
	move_forward_in_inches(24);
	turn_90_degrees(false);
	
	// Drive 4 ft then turn right
	move_forward_in_inches(48);
	turn_90_degrees(false);
	
	// Drive 9 ft
	move_forward_in_inches(108);
}

// @brief: Drive the robot in a 2ft diameter circle.
void app_lab_part4(void)
{
	double circumference = 75.39822; // 2*pi*12(inches)
	
	// Dividing circumference by the number of
	// turns needed to go around the circle.
	double distance_per_step = circumference/20;
	
	for (int i = 0; i < 24; i++){
		// move forward by step distance
		move_forward_in_inches(distance_per_step);
		
		// turn wheels 18 degrees
		turn_right_18_degrees();
		
	}

}
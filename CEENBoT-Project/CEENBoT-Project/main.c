/*
 * Auth:   David Perez
 * File:   main.c
 * Date:   2/18/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 2
 * Desc :  Drives the CEENBoT robot through different
 *     paths using dead reckoning. The first of the four paths
 *     are a series of straight distances of 6, 12, 18, and 24 inches.
 *     The next path is a 4 ft by 4ft square, followed by
 *     a maze of multiple left and right turns. The last
 *     path is a circle with a 2 foot diameter.
 */ 

#include "src/apps.h"
#include "capi324v221.h"

void CBOT_main(void)
{
	// Open STEPPER module for use
	STEPPER_open();

	// Perform lab 2 part 1:
	app_lab_part1();
	
	// Perform lab 2 part 2:
	app_lab_part2();
	
	// Perform lab 2 part 3:
	app_lab_part3();
	
	// Perform lab 2 part 4:
	app_lab_part4();

	// Turn off the stepper motors
	STEPPER_close();
}


/*
 * Auth:   David Perez
 * File:   motor_control.h
 * Date:   2/18/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 2
 * Desc :  Header file for motor_control.c
 */  

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "stdbool.h"

// States for part 1 subparts
typedef enum{
	LP1_0,
	LP1_1,
	LP1_2,
	LP1_3,
	NUM_LABPART1,
} LabPart1;

double steps_per_distance(double distance_inches);
void move_forward_in_inches(double distance);
void turn_90_degrees(bool turn_left);
void turn_right_18_degrees (void);

#endif /* MOTOR_CONTROL_H_ */
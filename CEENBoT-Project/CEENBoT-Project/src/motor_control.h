/*
 * Auth:   David Perez
 * Date:   2/21/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 3
 * Desc :  Header file for motor_control.c
 */  

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "stdbool.h"

double steps_per_distance(double distance_inches);
void move_forward_in_inches_stwt(double distance);
void move_forward_in_inches_stnb(double distance, int velocity);
void move_backwards_in_inches_stwt(double distance);
void turn_90_degrees(bool turn_left);
void reverse_backup_60_degrees(bool turn_left);
void set_motor_speeds(float left_motor_speed, int right_motor_speed);

#endif /* MOTOR_CONTROL_H_ */
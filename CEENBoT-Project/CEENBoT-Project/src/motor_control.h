/*
 * Auth:   David Perez
 * File:   motor_control.h
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

#endif /* MOTOR_CONTROL_H_ */
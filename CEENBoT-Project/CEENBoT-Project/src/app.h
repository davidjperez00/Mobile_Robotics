/*
 * Auth:   David Perez
 * File:   app.h
 * Date:   2/18/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 3
 * Desc :  Header file for app.c
 */  


#ifndef APP_H_
#define APP_H_

typedef enum {
	SS_INHIBITORY,
	OS_INHIBITORY,
	SS_EXCITATORY,
	OS_EXCITATORY,
} ReactiveBehavior;

void IR_AVOID (void);
void CRUISE (void);
void app_main (ReactiveBehavior desired_behavior);

#endif /* APP_H_ */
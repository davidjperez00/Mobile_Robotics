/*
 * Auth:   David Perez
 * Date:   Today's Date
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 1
 * Desc :  Print text to CEENBoT LCD Display
 */ 

#include "capi324v221.h"

void CBOT_main(void)
{
	// Open the LCD subsystem module.
	LCD_open();
	
	// Clear the display.
	LCD_clear();
	
	// Say, "Hello"
	LCD_printf( "Hello, David Perez!\n");
	
	// Loop forever
    while (1);
}


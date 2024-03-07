/*
 * Auth:   David Perez
 * File:   app.c
 * Date:   2/21/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 3
 * Desc :  Provides application level control for IR sensor object avoidance.
 */ 

#include "stdio.h"

#include "app.h"
#include "motor_control.h"
#include "capi324v221.h"

float read_right_pr_sensor_voltage (void)
{
	// Set the channel we will sample from (right photo-resistor sensor)
	ADC_set_channel(ADC_CHAN3);

	// Sample the ADC
	ADC_SAMPLE sample = ADC_sample();

	// Convert the ADC sample to a voltage
	return ( ( sample * 5.0f) / 1024);
}

float read_left_pr_sensor_voltage (void)
{
	// Set the channel we will sample from (left photo-resistor sensor)
	ADC_set_channel(ADC_CHAN4);

	// Sample the ADC
	ADC_SAMPLE sample = ADC_sample();

	// Convert the ADC sample to a voltage
	return ( ( sample * 5.0f) / 1024);
}



void display_pr_readings(float left_sensor, float right_sensor)
{
	// Create a string with both values:
	char lcd_string[60];
	
	// Populate string with sensor voltages
	sprintf(lcd_string, "left_sensor = %.03f\r\n right_sensor = %.04f\r\n", left_sensor, right_sensor);

		
	// clear the display before printing to it:
	LCD_clear();

	// Put string on LCD display.
	LCD_printf(lcd_string);
}


// @brief: Provides higher level control between CRUISE()
//     and IR_AVOID().
void app_main (ReactiveBehavior desired_behavior)
{
	while ( 1 ) {
		
		// Read both sensors:
		float left_reading = read_left_pr_sensor_voltage();
		float right_reading = read_right_pr_sensor_voltage();

		// Print to the LCD screen
		display_pr_readings(left_reading, right_reading);

		// The right sensor reading is greater than the left motor
		// by about .250 voltz.
		if (right_reading >= 0.250)
		{
			right_reading -= 0.250;
		} else {
			right_reading = 0;
		}

		int speed_scale_constant = 75;

		switch(desired_behavior)
		{
			case(SS_INHIBITORY):{
				// Make sensor reading inversely proportional to 
				// motor speed
				float MAX_left_voltage = 4.40;
				float MAX_right_voltage = 4.74;

				float left_motor_speed = MAX_left_voltage - left_reading;
				float right_motor_speed = MAX_right_voltage - right_reading;

				set_motor_speeds(left_motor_speed * speed_scale_constant, right_motor_speed * speed_scale_constant);

				break;
			}
			case(OS_INHIBITORY):{
				// Make sensor reading inversely proportional to
				// motor speed
				float MAX_left_voltage = 4.40;
				float MAX_right_voltage = 4.74;

				float left_motor_speed = MAX_left_voltage - left_reading;
				float right_motor_speed = MAX_right_voltage - right_reading;

				// Intentionally have the left motor mapped to the right
				// sensor and the right motor mapped to the right sensor.
				set_motor_speeds(left_motor_speed * speed_scale_constant, right_motor_speed * speed_scale_constant);
			
				break;
			}
			case(SS_EXCITATORY):{
				set_motor_speeds((int)left_reading * speed_scale_constant, (int)right_reading * speed_scale_constant);	
	
				break;
			}
			case(OS_EXCITATORY):{
				// Intentionally have the left motor mapped to the right
				// sensor and the right motor mapped to the right sensor.
				set_motor_speeds((int)right_reading * speed_scale_constant, (int)left_reading * speed_scale_constant);

				break;
			} default: {
				break;
			}
		}
	}
}


// The sensor on the "same side" of the motor speeds up
// with greater light intensity (i.e., the right sensor influences
// the right motor and the left motor influence the left motor).
void ss_excititory_CRUISE (void)
{





	// Excititory (motor speeds up with light intensity
	
	
}


// @brief: Avoid obstacles using the IR sensors.
void IR_AVOID (void)
{
	// If both sensors detect object turn the robot
	// in an arbitrary direction (left is chosen)
	if (ATTINY_get_IR_state(ATTINY_IR_BOTH)){
		
		// Stop the robots forward motion
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);

		// Back up robot away from object
		move_backwards_in_inches_stwt(3);
		
		// Turn right
		reverse_backup_60_degrees(false);
		
		return;
	}
	
	// Check if object is detected to the left of the CEENBoT
	if (ATTINY_get_IR_state(ATTINY_IR_LEFT)){
		
		// Stop the robot
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
		
		// Back up robot away from object
		move_backwards_in_inches_stwt(3);
		
		// Turn right
		reverse_backup_60_degrees(false);
		
		return;
		
	}
	
	// Check if object is detected in to the right of the CEENBoT
	if (ATTINY_get_IR_state(ATTINY_IR_RIGHT)){
			
		// Stop the robot
		STEPPER_stop(STEPPER_BOTH, STEPPER_BRK_ON);
		
		// Back up robot away from object
		move_backwards_in_inches_stwt(3);
				
		// Turn right
		reverse_backup_60_degrees(true);	
		
		return;
	}
}




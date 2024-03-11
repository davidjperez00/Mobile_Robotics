/*
 * Auth:   David Perez
 * File:   app.c
 * Date:   3/10/2024
 * Course: ECEN3450-Mobile Robotics I
 * Lab#:   Lab 4
 * Desc :  Provides application level control for photo-resistor inhibitor or excitatory behavior.
 */ 

#include "stdio.h"

#include "app.h"
#include "motor_control.h"
#include "capi324v221.h"

// @brief: Read and return the front right mounted photo-resistor sensor voltage.
float read_right_pr_sensor_voltage (void)
{
	// Set the channel we will sample from (right photo-resistor sensor)
	ADC_set_channel(ADC_CHAN3);

	// Sample the ADC
	ADC_SAMPLE sample = ADC_sample();

	// Convert the ADC sample to a voltage
	return ( ( sample * 5.0f) / 1024);
}

// @brief: Read and return the front left mounted photo-resistor sensor voltage.
float read_left_pr_sensor_voltage (void)
{
	// Set the channel we will sample from (left photo-resistor sensor)
	ADC_set_channel(ADC_CHAN4);

	// Sample the ADC
	ADC_SAMPLE sample = ADC_sample();

	// Convert the ADC sample to a voltage
	return ( ( sample * 5.0f) / 1024);
}

// @brief: Display the front left and right photo-resistor voltage
//     readings to the LCD display.
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
		// by about .250 volts.
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

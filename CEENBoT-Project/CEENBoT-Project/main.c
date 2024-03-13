//
// Desc: Provides a C program structure that emulates multi-tasking and
// modularity for Behavior-based control with easy scalability.
//
// Supplied for: Students of Mobile Robotics I, Fall 2013.
// University of Nebraska-Lincoln Dept. of Computer & Electronics Engineering
// Alisa N. Gilmore, P.E., Instructor, Course Developer. Jose Santos, T.A.
// Version 1.3 Updated 10/11/2011.
// Version 1.4 Updated 12/2/2013
//
// - Updated __MOTOR_ACTION() macro-function to invoke new functions
// added to the API: `STEPPER_set_accel2()' and `STEPPER_run2()'.
// In particular `STEPPER_run2()' now takes positive or negative
// speed values to imply the direction of each wheel.
//
// Version 1.5 Updated 2/25/2015
//
// - Fixed an error in __MOTOR_ACTION() and __RESET_ACTION() macros
// where there was an extra curly brace ({) that should have been
// removed.
//
// Version 1.6 Updated 2/24/2016
//
// - In the 'IR_sense()' function, we now make use of the 'TIMER_ALARM()'
// and 'TIMER_SNOOZE()' macros that were introduced in API version 2.x,
// which makes usage of the timer object clear. Before, students
// manipulated the 'tc' flag inside the timer object directly, but this
// always caused confusion, the 'TIMER_ALARM()' and 'TIMER_SNOOZE()'
// macros achieve the same thing, but transparently.
//
// - Also fixed __MOTOR_ACTION() macro, which previously invoked
// 'STEPPER_run2()', but the API has been modified so that this same
// effect is now achieved with the function 'STEPPER_runn()', which
// means to run the stepper motors and allow negative values to mean
// reverse motion (that's what the second 'n' in 'runn' stands for).
// This might change again in the future, because for consistency in the
// API, it should have been called 'STEPPER_runn2()' since it takes two
// parameters and not just one.
//
#include "capi324v221.h"

// ---------------------- Defines:

#define DEG_90 133 /* Number of steps for a 90-degree (in place) turn. */
// Desc: This macro-function can be used to reset a motor-action structure
//		 easily. It is a helper macro-function.

#define DEG_180 266 /* Number of steps for a 180-degree (in place) turn. */
// Desc: This macro-function can be used to reset a motor-action structure
//		 easily. It is a helper macro-function.

// Define a threshold minimum value for light intensity
#define LIGHT_MIN_THRESHOLD 2.0f // Adjust this value as needed

// Threshold for determining if the robot is close to a light source.
#define MIN_OBSERVE_VOLTAGE 2.50

#define __RESET_ACTION( motor_action )	\
do {								\
	( motor_action ).speed_L = 0;		\
	( motor_action ).speed_R = 0;		\
	( motor_action ).accel_L = 0;		\
	( motor_action ).accel_R = 0;		\
	( motor_action ).state = STARTUP;   \
	} while( 0 ) /* end __RESET_ACTION() */
	
// Desc: This macro-fuction translates action to motion -- it is a helper
//		 macro-function.
#define __MOTOR_ACTION( motor_action )	\
do {								\
STEPPER_set_accel2( ( motor_action ).accel_L, ( motor_action ).accel_R );	\
STEPPER_runn( ( motor_action ).speed_L, ( motor_action ).speed_R );			\
} while( 0 ) /* end __MOTOR_ACTION() */
		
// Desc: This macro-function is used to set the action, in a more natural
//		 manner (as if it was a function).
// ---------------------- Type Declarations:
// Desc: The following custom enumerated type can be used to specify the
// current state of the robot. This parameter can be expanded upon
// as complexity grows without intefering with the 'act()' function.
// It is a new type which can take the values of 0, 1, or 2 using
// the SYMBOLIC representations of STARTUP, EXPLORING, etc.
typedef enum ROBOT_STATE_TYPE {
			
	STARTUP = 0,			// 'Startup' state -- initial state upon RESET.
	EXPLORING,				// 'Exploring' state -- the robot is 'roaming around'.
	AVOIDING,				// 'Avoiding' state -- the robot is avoiding a collision.
	HOMING,					// 'Homing' state -- The robot is following a light source.
	OBSERVE,          // 'Observe' state -- The robot admires a light at 4-5 inches from itself.
			 
} ROBOT_STATE;

// Desc: Structure encapsulates a 'motor' action. It contains parameters that
// controls the motors 'down the line' with information depicting the
// current state of the robot. The 'state' variable is useful to
// 'print' information on the LCD based on the current 'state', for
// example.
typedef struct MOTOR_ACTION_TYPE {
			
	ROBOT_STATE state;					// Holds the current STATE of the robot.
	signed short int speed_L;			// SPEED for LEFT motor.
	signed short int speed_R;			// SPEED for RIGHT motor.
	unsigned short int accel_L;			// ACCELERATION for LEFT motor.
	unsigned short int accel_R;			// ACCELERATION for RIGHT motor.
			
} MOTOR_ACTION;

// Desc: Structure encapsulates 'sensed' data. Right now that only consists
// of the state of the left & right IR sensors when queried. You can
// expand this structure and add additional custom fields as needed.
typedef struct SENSOR_DATA_TYPE {
			
	BOOL left_IR;			// Holds the state of the left IR.
	BOOL right_IR;			// Holds the state of the right IR.
			
	float voltage_right;		//Holds the state of the Left photo resistor
	float voltage_left;		//Holds the state of the Right photo resistor
			
			
	// *** Add your -own- parameters here.
			
} SENSOR_DATA;

// ---------------------- Globals:
volatile MOTOR_ACTION action;	// This variable holds parameters that determine
// the current action that is taking place.
// Here, a structure named "action" of type
// MOTOR_ACTION is declared.
		
// ---------------------- Prototypes:
void IR_sense( volatile SENSOR_DATA *pSensors, TIMER16 interval_ms );
void Photo_sense( volatile SENSOR_DATA *pLightSensors, TIMER16 interval_ms );
void explore( volatile MOTOR_ACTION *pAction );
void IR_avoid( volatile MOTOR_ACTION *pAction, volatile SENSOR_DATA *pSensors );
void act( volatile MOTOR_ACTION *pAction );
void info_display( volatile MOTOR_ACTION *pAction );
void LIGHT_OBSERVE(volatile MOTOR_ACTION *pAction, volatile SENSOR_DATA *pLightSensors);
BOOL compare_actions( volatile MOTOR_ACTION *a, volatile MOTOR_ACTION *b );

// ---------------------- Convenience Functions:
void info_display( volatile MOTOR_ACTION *pAction )
{
	// NOTE: We keep track of the 'previous' state to prevent the LCD
	// display from being needlessly written, if there's nothing
	// new to display. Otherwise, the screen will 'flicker' from
	// too many writes.
	static ROBOT_STATE previous_state = STARTUP;
			
	if ( ( pAction->state != previous_state ) || ( pAction->state == STARTUP ) )
	{
		LCD_clear();
				
		// Display information based on the current 'ROBOT STATE'.
		switch( pAction->state )
		{
					
			case STARTUP:
					
			LCD_printf("Starting the Program...\n");
					
			break;
					
			case EXPLORING:
					
			LCD_printf( "Exploring...\n" );
					
			break;
					
			case AVOIDING:
					
			LCD_printf("Avoiding...\n");
					
			break;
					
			case HOMING:
					
			LCD_printf("Homing...\n");
					
			break;
					
					
			default:
			LCD_printf( "Unknown state!\n" );
		} // end switch()
				
		// Note the new state in effect.
		previous_state = pAction->state;
				
	} // end if()
			
} // end info_display()

// ----------------------------------------------------- //
BOOL compare_actions( volatile MOTOR_ACTION *a, volatile MOTOR_ACTION *b )
{
			
	// NOTE: The 'sole' purpose of this function is to
	//		 compare the 'elements' of MOTOR_ACTION structures
	//		 'a' and 'b' and see if 'any' differ.
			
	// Assume these actions are equal.
	BOOL rval = TRUE;
			
	if ( ( a->state != b->state )	 ||
	( a->speed_L != b->speed_L ) ||
	( a->speed_R != b->speed_R ) ||
	( a->accel_L != b->accel_L ) ||
	( a->accel_R != b->accel_R ) )
			
	rval = FALSE;
			
	// Return comparison result.
	return rval;
			
} // end compare_actions()

// ---------------------- Top-Level Behaviorals:
void IR_sense( volatile SENSOR_DATA *pSensors, TIMER16 interval_ms )
{
	// Sense must know if it's already sensing.
	//
	// NOTE: 'BOOL' is a custom data type offered by the CEENBoT API.
	//
	static BOOL timer_started = FALSE;
			
	// The 'sense' timer is used to control how often gathering sensor
	// data takes place. The pace at which this happens needs to be
	// controlled. So we're forced to use TIMER OBJECTS along with the
	// TIMER SERVICE. It must be 'static' because the timer object must remain
	// 'alive' even when it is out of scope -- otherwise the program will crash.
	static TIMEROBJ sense_timer;
			
	// If this is the FIRST time that sense() is running, we need to start the
	// sense timer. We do this ONLY ONCE!
	if ( timer_started == FALSE )
	{
				
		// Start the 'sense timer' to tick on every 'interval_ms'.
		//
		// NOTE: You can adjust the delay value to suit your needs.
		//
		TMRSRVC_new( &sense_timer, TMRFLG_NOTIFY_FLAG, TMRTCM_RESTART,
		interval_ms );
				
		// Mark that the timer has already been started.
		timer_started = TRUE;
				
	} // end if()
			
	// Otherwise, just do the usual thing and just 'sense'.
	else
	{
				
		// Only read the sensors when it is time to do so (e.g., every
		// 125ms). Otherwise, do nothing.
		if ( TIMER_ALARM( sense_timer ) )
		{
					
			// NOTE: Just as a 'debugging' feature, let's also toggle the green LED
			// to know that this is working for sure. The LED will only
			// toggle when 'it's time'.
			LED_toggle( LED_Green );
					
			// Read the left and right sensors, and store this
			// data in the 'SENSOR_DATA' structure.
			pSensors->left_IR = ATTINY_get_IR_state( ATTINY_IR_LEFT );
			pSensors->right_IR = ATTINY_get_IR_state( ATTINY_IR_RIGHT );
					
			// NOTE: You can add more stuff to 'sense' here.
					
			// Snooze the alarm so it can trigger again.
			TIMER_SNOOZE( sense_timer );
					
		} // end if()
				
	} // end else.
			
} // end sense()

void Photo_sense( volatile SENSOR_DATA *pLightSensors, TIMER16 interval_ms )
{
	// Sense must know if it's already sensing.
	//
	// NOTE: 'BOOL' is a custom data type offered by the CEENBoT API.
	//
	static BOOL timer_started = FALSE;
			
	// The 'sense' timer is used to control how often gathering sensor
	// data takes place. The pace at which this happens needs to be
	// controlled. So we're forced to use TIMER OBJECTS along with the
	// TIMER SERVICE. It must be 'static' because the timer object must remain
	// 'alive' even when it is out of scope -- otherwise the program will crash.
	static TIMEROBJ sense_timer;
			
	// If this is the FIRST time that sense() is running, we need to start the
	// sense timer. We do this ONLY ONCE!
	if ( timer_started == FALSE )
	{
		// Start the 'sense timer' to tick on every 'interval_ms'.
		//
		// NOTE: You can adjust the delay value to suit your needs.
		//
		TMRSRVC_new( &sense_timer, TMRFLG_NOTIFY_FLAG, TMRTCM_RESTART,
		interval_ms );
		// Mark that the timer has already been started.
		timer_started = TRUE;
	} // end if()
	// Otherwise, just do the usual thing and just 'sense'.
	else
	{
		// Only read the sensors when it is time to do so (e.g., every
		// 125ms). Otherwise, do nothing.
		if ( TIMER_ALARM( sense_timer ) )
		{
			// NOTE: Just as a 'debugging' feature, let's also toggle the red LED
			// to know that this is working for sure. The LED will only
			// toggle when 'it's time'.
			LED_toggle( LED_RED );
					
			ADC_SAMPLE sample_R, sample_L; // Storage for ADC code.
			float voltage_R, voltage_L;    // Storage for 'voltage' representation of ADC code.
					
			// Set the voltage reference (we want 5V reference).
			ADC_set_VREF(ADC_VREF_AVCC);
					
			// Set the channel for Right sensor.
			ADC_set_channel(ADC_CHAN3);
					
			// SAMPLE the data from right sensor
			sample_R = ADC_sample();
					
			// Convert it to meaningful value for Right sensor.
			voltage_R = ((sample_R * 5.0f) / 1024);
					
			// Set the channel for Left sensor.
			ADC_set_channel(ADC_CHAN5);
					
			// SAMPLE the data from Left sensor
			sample_L = ADC_sample();
					
			// Convert it to meaningful value for Left sensor.
			voltage_L = ((sample_L * 5.0f) / 1024);
					
			// Store the voltage readings in the provided structure
			pLightSensors->voltage_right = voltage_R;
			pLightSensors->voltage_left = voltage_L;
					
			TIMER_SNOOZE( sense_timer );
		} // end if()
	} // end else.
} // end sense()


// -------------------------------------------- //
void explore( volatile MOTOR_ACTION *pAction )
{
	// Nothing to do, but set the parameters to explore. 'act()' will do
	// the rest down the line.
	pAction->state = EXPLORING;
	pAction->speed_L = 200;
	pAction->speed_R = 200;
	pAction->accel_L = 400;
	pAction->accel_R = 400;
	// That's it -- let 'act()' do the rest.
			
} // end explore()
// -------------------------------------------- //

void IR_avoid( volatile MOTOR_ACTION *pAction, volatile SENSOR_DATA *pSensors )
{
	// NOTE: Here we have NO CHOICE, but to do this 'ballistically'.
	//		 **NOTHING** else can happen while we're 'avoiding'.
			
	// Example of ONE case (you can expand on this idea):
			
	// If the LEFT sensor tripped...
	if( pSensors->left_IR == TRUE )
	{
				
		// Note that we're avoiding...
		pAction->state = AVOIDING;
				
		// STOP!
		STEPPER_stop( STEPPER_BOTH, STEPPER_BRK_OFF );
				
		// Back up...
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF );
				
		// ... and turn RIGHT ~90-deg.
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_FWD, DEG_90, 200, 400, STEPPER_BRK_OFF,
		STEPPER_REV, DEG_90, 200, 400, STEPPER_BRK_OFF );
				
		// ... and set the motor action structure with variables to move forward.
		pAction->state = AVOIDING;
		pAction->speed_L = 200;
		pAction->speed_R = 200;
		pAction->accel_L = 400;
		pAction->accel_R = 400;
				
	} // end if()
			
	// If the Right sensor tripped...
	if( pSensors->right_IR == TRUE )
	{
				
		// Note that we're avoiding...
		pAction->state = AVOIDING;
				
		// STOP!
		STEPPER_stop( STEPPER_BOTH, STEPPER_BRK_OFF );
				
		// Back up...
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF );
				
		// ... and turn Left ~90-deg.
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, DEG_90, 200, 400, STEPPER_BRK_OFF,
		STEPPER_FWD, DEG_90, 200, 400, STEPPER_BRK_OFF );
				
		// ... and set the motor action structure with variables to move forward.
		pAction->state = AVOIDING;
		pAction->speed_L = 200;
		pAction->speed_R = 200;
		pAction->accel_L = 400;
		pAction->accel_R = 400;
				
	} //end if()
			
	// If the BOTH sensors tripped...
	if( pSensors->right_IR == TRUE && pSensors->left_IR == TRUE )
	{
				
		// Note that we're avoiding...
		pAction->state = AVOIDING;
				
		// STOP!
		STEPPER_stop( STEPPER_BOTH, STEPPER_BRK_OFF );
				
		// Back up...
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF );
				
		// ... and turn 180-deg.
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, DEG_180, 200, 400, STEPPER_BRK_OFF,
		STEPPER_FWD, DEG_180, 200, 400, STEPPER_BRK_OFF );
				
		// ... and set the motor action structure with variables to move forward.
		pAction->state = AVOIDING;
		pAction->speed_L = 200;
		pAction->speed_R = 200;
		pAction->accel_L = 400;
		pAction->accel_R = 400;
				
	} // end if()
			
} // end avoid()

// Function to check if the average light intensity is greater than the threshold
BOOL isLightIntenseEnough(float left_intensity, float right_intensity) {
			
	float average_intensity = (left_intensity + right_intensity) / 2.0f;
	return (average_intensity > LIGHT_MIN_THRESHOLD);
			
}

// LIGHT_FOLLOW behavior
void LIGHT_FOLLOW(volatile MOTOR_ACTION *pAction, volatile SENSOR_DATA *pLightSensors)
{
	// Check if the average light intensity is greater than the threshold
	if (isLightIntenseEnough(pLightSensors->voltage_left, pLightSensors->voltage_right)) {
				
		// Calculate the difference in intensity between left and right sensors
		float intensity_difference = pLightSensors->voltage_left - pLightSensors->voltage_right;

		// Adjust motor speeds based on the intensity difference
		// Assume a linear relationship between intensity difference and motor speed
		pAction->speed_L = 200 - intensity_difference * 50; // Increase left motor speed if left is brighter
		pAction->speed_R = 200 + intensity_difference * 50; // Increase right motor speed if right is brighter

		// Set acceleration for smoother movement
		pAction->accel_L = 400;
		pAction->accel_R = 400;

		// Note that we're homing...
		pAction->state = HOMING;
	}
}
		
void LIGHT_OBSERVE(volatile MOTOR_ACTION *pAction, volatile SENSOR_DATA *pLightSensors)
{
	// Get the average light intensity
	float intensity_avg = (pLightSensors->voltage_left + pLightSensors->voltage_right) / 2.0f;			

	// Determine if we are 4-5 inches from the light source via thresholding
	if (intensity_avg > MIN_OBSERVE_VOLTAGE)
	{
		// Stop the robot
		STEPPER_stop( STEPPER_BOTH, STEPPER_BRK_OFF );
		
		// Wait some time to "observe"
		TMRSRVC_delay( TMR_SECS( 3 ) );
		
		// Back up...
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF,
		STEPPER_REV, 150, 200, 400, STEPPER_BRK_OFF );
				
		// ... and turn Left ~90-deg.
		STEPPER_move_stwt( STEPPER_BOTH,
		STEPPER_REV, DEG_90, 200, 400, STEPPER_BRK_OFF,
		STEPPER_FWD, DEG_90, 200, 400, STEPPER_BRK_OFF );
		
		// Update state, and set future motor actions
		pAction->state = OBSERVE;
		pAction->speed_L = 200;
		pAction->speed_R = 200;
		pAction->accel_L = 400;
		pAction->accel_R = 400;
	}
}


// -------------------------------------------- //
void act( volatile MOTOR_ACTION *pAction )
{
	// 'act()' always keeps track of the PREVIOUS action to determine
	// if a new action must be executed, and to execute such action ONLY
	// if any parameters in the 'MOTOR_ACTION' structure have changed.
	// This is necessary to prevent motor 'jitter'.
	static MOTOR_ACTION previous_action = {
				
		STARTUP, 0, 0, 0, 0
				
	};
			
	if( compare_actions( pAction, &previous_action ) == FALSE )
	{
				
		// Perform the action. Just call the 'free-running' version
		// of stepper move function and feed these same parameters.
		__MOTOR_ACTION( *pAction );
				
		// Save the previous action.
		previous_action = *pAction;
				
	} // end if()
			
} // end act()
// ---------------------- CBOT Main:
void CBOT_main( void )
{
	volatile SENSOR_DATA sensor_data;
			
	// ** Open the needed modules.
	LED_open();			// Open the LED subsystem module.
	LCD_open();			// Open the LCD subsystem module.
	STEPPER_open();		// Open the STEPPER subsystem module.
	ADC_open();			//Open the ADC subsystem module.
			
	// Reset the current motor action.
	__RESET_ACTION( action );
			
	// Notify program is about to start.
	LCD_printf( "Starting...\n" );
	// Wait 3 seconds or so.
			
	TMRSRVC_delay( TMR_SECS( 3 ) );
			
	// Clear the screen and enter the arbitration loop.
	LCD_clear();
			
	// Enter the 'arbitration' while() loop -- it is important that NONE
	// of the behavior functions listed in the arbitration loop BLOCK!
	// Behaviors are listed in increasing order of priority, with the last
	// behavior having the greatest priority (because it has the last 'say'
	// regarding motor action (or any action)).
	while( 1 )
	{
				
		// Sense must always happen first.
		// (IR sense happens every 125ms).
		IR_sense( &sensor_data, 125 );
				
		// (Light sense happens every 125ms).
		Photo_sense(&sensor_data, 125);
				
		// Behaviors.
		explore( &action );
				
		// Note that 'avoidance' relies on sensor data to determine
		// whether or not 'avoidance' is necessary.
		IR_avoid( &action, &sensor_data );
				
		// Perform light-following behavior
		LIGHT_FOLLOW(&action, &sensor_data);
				
		LIGHT_OBSERVE(&action, &sensor_data);
				
		// Perform the action of highest priority.
		act( &action );
				
		// Real-time display info, should happen last, if possible (
		// except for 'ballistic' behaviors). Technically this is sort of
		// 'optional' as it does not constitute a 'behavior'.
		info_display( &action );
				
	} // end while()
			
} // end CBOT_main()
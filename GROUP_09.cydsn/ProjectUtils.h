/* 
 *      /file ProjectUtils.h
 *  This file provides some utils for initialization and definition of constants
*/

// Define possible values for avriable STATE:
// we assigned random numbers to avoid ambiguity with other constants
#define IDLE 500
#define COLOR_SET 501
#define TIMEOUT_SET 502
#define TAIL 503
#define RESET 504

// Constants related to the protocol packets
#define HEADER_COLOR 160
#define HEADER_TIMEOUT 161
#define TAIL_BYTE 192
 
// Time constants
#define MIN_TIMEOUT 1
#define MAX_TIMEOUT 20
#define DEFAULT_TIMEOUT 5
#define INFINITE 30  
//Ideally the user should have infinite time to set the new 'timeout' seconds, but to avoid the block of the code,
//we selected a long fixed timeout of 30 secs, after which the protocol stops and the machine goes back to IDLE state

// Reference for TAIL STATE execution which specify from which STATE you come
#define FROM_COLOR 505
#define FROM_TIMEOUT 506

// Some constants
#define RESET_TIMER 0  // used to reset the counter of the timer to 0
#define SIZE_COLOR 3   // used to set the size of the vector storing the bytes related to colors of protocol packet



// Function to initialize PWM, timer and UART
void InitializeComponents(void);

// Function to activate the interrupts
void ActivateISR(void);




/* [] END OF FILE */

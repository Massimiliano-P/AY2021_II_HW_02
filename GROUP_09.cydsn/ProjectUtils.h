/* 
 *      /file ProjectUtils.h
 *  This header file provides some utils for initialization and definition of constants
*/

// Define possible values for avriable STATE

#define IDLE 0
#define COLOR_SET 1
#define TIMEOUT_SET 2
#define TAIL 3
#define RESET 4

// Define prossible values for the packet header variable

#define HEADER_COLOR 160
#define HEADER_TIMOUT 161

#define SIZE_COLOR 3
#define INFINITE 30
#define RESET_TIMER 0

#define FROM_COLOR 1
#define FROM_TIMEOUT 2


void InitializeComponents(void);

void ActivateISR(void);




/* [] END OF FILE */

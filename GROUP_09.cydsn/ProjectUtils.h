/* 
 *      /file ProjectUtils.h
 *  This file provides some utils for initialization and definition of constants
*/

// Define possible values for avriable STATE:
// we assigned random numbers to avoid ambiguity with other constants
#define IDLE 200
#define COLOR_SET 201
#define TIMEOUT_SET 202
#define TAIL 203

// Reference for TAIL STATE execution which specifies from which STATE you come 
#define UPDATE_COLOR 204
#define UPDATE_TIMEOUT 205

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

#define SIZE_COLOR 3   // used to set the size of the vector storing the bytes related to colors of protocol packet

/* GLOBAL VARIABLES DECLARATION */

char timeoutMax;    // variable to store the amount of time after which we stop the communication protocol
                    // for color setting

/* variables used to manage the project */
int index_parser; //used to scroll the vector in which we store the colors, to check when it is full
char STATE;        // used to drive the entire project
char source;       // used to update LEDs or timeout variable


/* GLOBAL FUNCTIONS */

// Function to initialize PWM, UART and variables
void InitializeComponents(void);

// Function to activate the interrupts
void ActivateISR(void);

// Function to check if the byte received is acceptable as header
void checkHeader(char byte_received);

// Function to check if the byte received is acceptable as tail, and if true it drives LEDs or sets new timeout
void checkTail(char byte_received, char colorArray[], char packetTimeout);

// Function used to reset the timer and the variable which counts seconds
void resetTimer(void);

// Function to store the byte received into the array of colors (RGB) and to check if that array is full
void storeColorByte(char byte_received, char colorArray[]);

// Function to check if we are exceeding the max delay (upperBound)
void checkTimeOverflow(char Seconds,char upperBound);

// Function to check if the new timeout inseted by terminal is acceptable according to specs
void checkTimeSelected(char packetTimeout);




/* [] END OF FILE */

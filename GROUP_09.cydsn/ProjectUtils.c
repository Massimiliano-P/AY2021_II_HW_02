/* ========================================
 *      /file ProjectUtils.c
 *
 * In this file the functions declared in ProjectUtils.h are defined
 * ========================================
*/

#include "ProjectUtils.h"
#include "Interrupts.h"
#include "project.h"
#include "LEDdriver.h" 




void InitializeComponents()
{
    LED_Start(); // start PWM
    UART_Start(); // start UART
    
    // Initialize STATE to IDLE
    STATE = IDLE;
    // Initialize flag to keep track of bytes received
    Received_Byte_Flag = WAITING_FOR_BYTE;
    // Initialize timeoutMax to the default 5 seconds (as indicated in specs) 
    timeoutMax = DEFAULT_TIMEOUT;
    // Initialize the integer variable to scroll the vector that keeps track of bytes
    // related to colors (RGB)
    index_parser = 0;
}

void ActivateISR()
{
    ISR_UART_StartEx(UART_ISR);
    ISR_TIMER_StartEx(TIMER_ISR);
}

void checkHeader(char byte_received)
{
    // Piece of code to check if the first byte is a "HEADER" byte
    switch(byte_received)
    {
        case HEADER_COLOR:
            /* if we receive the byte corresponding to the header of 
            the packet used to set the color of RGB we enter into the state
            we named COLOR_SET and reset the timer to start count seconds */
            STATE = COLOR_SET; // exit condition
            resetTimer();
            break;
        case HEADER_TIMEOUT:
            /* this is the same as above, changing the state to set the timeout
            seconds */
            STATE = TIMEOUT_SET; // exit condition
            resetTimer();
            break;
        
        case 'v':
            /* if we send 'v' char to the microcontroller we must send back
            the string written below, as per protocol */
            UART_PutString("RGB LED Program $$$");
            break;
        
        default:
            /* if the first byte we receive is not one of the "HEADER" 
            involved in protocols, we must tell the user his input was wrong*/
            /*IN THIS CASE WE REMAIN IN IDLE*/
        UART_PutString("Header not acceptable");
    
    }
}

void resetTimer()
{
    Seconds = 0; // this is the variable we use to count seconds
    Timer_Start();
}

static void returnToIDLE()
{
    Timer_Stop();   // since after the next acceptable header we remain in IDLE, we can save resources by stopping the timer
    index_parser = 0; // needed so that the next packet for color setting will overwrite the old array containing previous colors
    STATE = IDLE;
    
}

void storeColorByte(char byte_received, char colorArray[])
{
    colorArray[index_parser] = byte_received;
    index_parser++;
    if(index_parser == SIZE_COLOR) // as soon as the vector is full, it means that all bytes were received correctly
                                   // so we need to enter the next state TAIL. Until vector is not full we cannot enter the next state
    {
        source = UPDATE_COLOR;    // reference for the type of TAIL state we enter
        STATE = TAIL;             // enter next state
    }
}

void checkTimeOverflow(char Seconds,char upperBound)
{
    if(Seconds == upperBound) // if we exceed the Max delay we need to go back to IDLE
    {
        returnToIDLE();
        UART_PutString("Timeout, re-send the entire packet");
    }   
}

void checkTimeSelected(char byte_received)
{
    if(byte_received <= MAX_TIMEOUT && byte_received >= MIN_TIMEOUT)    //if the time inserted is in the range given by specs we can proceed to next state
    {
        source = UPDATE_TIMEOUT;  // reference for the type of TAIL state we enter
        STATE = TAIL;
    }
    else 
    {
        returnToIDLE(); // if time inserted is out of range we go back to IDLE
        UART_PutString("Time selected out of range [1-20]");
    }
}

void checkTail(char byte_received, char colorArray[], char packetTimeout)
{
    if(byte_received == TAIL_BYTE) // check if TAIL is acceptable (as per protocol)
    {
        switch(source){
            case UPDATE_COLOR:    // meaning we are setting the new color -> drive LEDs to update the color
                LED_UpdateColor(colorArray);
                break;
            case UPDATE_TIMEOUT:  // meaning we are setting the new timeoutMax -> update the timeoutMax variable
                timeoutMax = packetTimeout;
                break;
        }
    }
    else {
        UART_PutString("Tail not acceptable");
    }
    returnToIDLE();  // in any case we must to return to IDLE since TAIL is the last state of our machine
}
/* [] END OF FILE */

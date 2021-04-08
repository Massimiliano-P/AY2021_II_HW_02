/* ========================================
 *          /file main.c
 *  
 *   This file implements the STATE_MACHINE which controls the whole flow of bytes 
 *   characterizing packets of communication protocols. Concurrently this code drives LEDs using 
 *   provided information by calling external functions imported by means of header files
 * ========================================
*/
#include "project.h"
#include "Interrupts.h"
#include "ProjectUtils.h"
#include "LEDdriver.h"

// Service variable to store received bytes
static char packetColor[SIZE_COLOR] = {'\0'};
static char packetTimeout;

char byte_received, timeoutMax;
int index_parser, STATE, source;



int main(void)
{
    // Import from 'ProjectUtils.h' functions to initialize components and 
    // related interrupts
    ActivateISR();
    InitializeComponents();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Initialize other variables:*/
    
    // Initialize STATE to IDLE
    STATE = IDLE;
    // Initialize flag to keep track of bytes received
    Received_Byte_Flag = WAITING_FOR_BYTE;
    // Initialize timeoutMax to the default 5 seconds (as indicated in specs) 
    timeoutMax = DEFAULT_TIMEOUT;
    // Initialize the integer variable to scroll the vector that keeps track of bytes
    // related to colors (RGB)
    index_parser = 0;
    
    
    for(;;)
    {
        if(STATE == RESET)
        {
            /* Reset the index_parser to 0, so that next color packet will be 
            saved overwriting the previous one received */
            index_parser = 0; 
            STATE = IDLE;
        }
        
        
        if(STATE == IDLE)
        {
            if(Received_Byte_Flag == BYTE_RECEIVED) // When the byte has been received
            {
                Received_Byte_Flag = WAITING_FOR_BYTE; // Condition to allow the toggling of the variable at next byte
                byte_received = UART_ReadRxData();
                
                // Piece of code to check if the first byte is a "HEADER" byte
                switch(byte_received)
                {
                    case HEADER_COLOR:
                        /* if we receive the byte corresponding to the header of 
                        the packet used to set the color of RGB we enter into the state
                        we named COLOR_SET and reset the timer to start count seconds */
                        STATE = COLOR_SET;
                        //Timer_WriteCounter(RESET_TIMER);
                        Seconds = 0; // this is the variable we use to count seconds
                        break;
                    case HEADER_TIMEOUT:
                        /* this is the same as above, changing the state to set the timeout
                        seconds */
                        STATE = TIMEOUT_SET;
                        //Timer_WriteCounter(RESET_TIMER);
                        Seconds = 0;
                        break;
                    
                    case 'v':
                        /* if we send 'v' char to the microcontroller we must send back
                        the string written below, as per protocol */
                        UART_PutString("RGB LED Program $$$");
                        break;
                    /*
                    case 'c':
                        UART_PutString(&timeoutMax);
                        break;
                        
                    case 'd':
                        
                        UART_PutString(packetColor);
                        break;
                       */
                    
                    default:
                        /* if the first byte we receive is not one of the "HEADER" 
                        involved in protocols, we must tell the user his input was wrong*/
                    UART_PutString("Header not acceptable");
                
                }
            }    
        }
        
        
        if(STATE == COLOR_SET)
        {
            /* once we are in this state, we should epect 3 subsequent bytes, each one 
            coming after less than X seconds wrt the previous. X value is to be set by using the
            other packet composed by 3 bytes; by default X = 5 seconds*/
            if(Seconds == timeoutMax){
                STATE = RESET;
                UART_PutString("Timeout, re-send the entire packet");
            }            
            if(Received_Byte_Flag == BYTE_RECEIVED)
            {
                Received_Byte_Flag = WAITING_FOR_BYTE;
                //Timer_WriteCounter(RESET_TIMER); //each time we receive a byte we have to restart 
                Seconds = 0;                  // the timer to check if the next will be inside the interval
                byte_received = UART_ReadRxData();
                packetColor[index_parser] = byte_received;
                index_parser++;
                if(index_parser == SIZE_COLOR) // if the vector is full, 
                                               //it means that all bytes were received correctly
                {
                    source = FROM_COLOR;    // reference for the type of TAIL state we enter
                    STATE = TAIL;
                }
                
            }
            
        }
        
        
        if(STATE == TIMEOUT_SET)
        {
            /* once we are in this state, we should epect a byte reporting the new timeout time. 
            To complete the sending of this packet we let the user a fixed amount of time
            equal to 30 seconds (INFINITE), allowing the user to have time to type all bytes properly*/
            if(Seconds == INFINITE)  // exceeding 30 seconds, the communication protocol crashes
                                       // going back to IDLE state
            {
                STATE = RESET;
                UART_PutString("Timeout, re-send the entire packet");
            }
            if(Received_Byte_Flag == BYTE_RECEIVED)
            {
                Received_Byte_Flag = WAITING_FOR_BYTE;
                //Timer_WriteCounter(RESET_TIMER);
                Seconds = 0;
                packetTimeout = UART_ReadRxData();
                if(packetTimeout <= MAX_TIMEOUT && packetTimeout >= MIN_TIMEOUT)
                {
                    source = FROM_TIMEOUT;  // reference for the type of TAIL state we enter
                    STATE = TAIL;
                }
                else {
                    STATE = RESET;
                    UART_PutString("Time selected out of range [1-20]");
                }
            }
        }
        
        if(STATE == TAIL)
        {
            // if we are in TIME_SET protocol the TAIL byte can have a delay up to INFINITE seconds
            // while in COLOR_SET protocol the TAIL byte can have a delay up to timeoutMax seconds
            if((Seconds == timeoutMax && source == FROM_COLOR) ||(Seconds == INFINITE && source == FROM_TIMEOUT)){
                STATE = RESET;
                UART_PutString("Timeout, re-send the entire packet");
            }
            if(Received_Byte_Flag == BYTE_RECEIVED)
            {
                Received_Byte_Flag = WAITING_FOR_BYTE;
                byte_received = UART_ReadRxData();
                if(byte_received == TAIL_BYTE) // check if TAIL is acceptable (as per protocol)
                {
                    switch(source){
                        case FROM_COLOR:    // meaning we are setting the color -> drive RGB_LED
                        //driving PWM: fill the color struct and lighting leds 
                        // (functions from ProjectUtils.h)
                        LED_UpdateColor(packetColor);
                        break;
                        case FROM_TIMEOUT:  // menaing we are setting the new timeoutMax
                        timeoutMax = packetTimeout;
                        break;
                    }
                }
                else {
                    UART_PutString("Tail not acceptable");
                }
                STATE = RESET;
            }
        }
        
        
       
    }
}

/* [] END OF FILE */

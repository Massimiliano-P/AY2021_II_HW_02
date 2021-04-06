/* ========================================
 *          /file main.c
 *  
 *   In this file 
 * ========================================
*/
#include "project.h"
#include "Interrupts.h"
#include "ProjectUtils.h"
#include "LEDdriver.h"

static char packetColor[SIZE_COLOR] = {'\0'};
static char packetTimeout;

volatile char flag, count_time, byte_received, timeoutMax;
char message;

int index_parser;
int STATE;
char source;
Color color;


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
    flag = 0;
    // Initialize timeoutMax to the default 5 seconds (as indicated in specs) 
    timeoutMax = 5;
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
            if(flag == 1) // When the byte has been received
            {
                flag = 0; // Condition to allow the toggling of the variable at next byte
                byte_received = UART_ReadRxData();
                
                // Piece of code to check if the first byte is a "HEADER" byte
                switch(byte_received)
                {
                    case HEADER_COLOR:
                        /* if we receive the byte corresponding to the header of 
                        the packet used to set the color of RGB we enter into the state
                        we named COLOR_SET and reset the timer to start count seconds */
                        STATE = COLOR_SET;
                        Timer_WriteCounter(RESET_TIMER);
                        count_time = 0; // this is the variable we use to count seconds
                        break;
                    case HEADER_TIMEOUT:
                        /* this is the same as above, changing the state to set the timeout
                        seconds */
                        STATE = TIMEOUT_SET;
                        Timer_WriteCounter(RESET_TIMER);
                        count_time = 0;
                        break;
                    
                    case 'v':
                        /* if we send 'v' char to the microcontroller we must send back
                        the string written below, as per protocol */
                        UART_PutString("RGB LED Program $$$");
                        break;
                    /*
                    case 'c':
                        message = timeoutMax;
                        UART_PutString(&timeoutMax);
                        break;
                        
                    case 'd':
                        message = packetColor[0];
                        UART_PutString(packetColor);
                        break;
                       */
                    
                    default:
                        /* if the first byte we receive is not one of the "HEADER" 
                        involved in protocols, we must tell the user his input was wrong*/
                    UART_PutString("Header unaccepted");
                
                }
            }    
        }
        
        
        if(STATE == COLOR_SET)
        {
            /* once we are in this state, we should epect 3 subsequent bytes, each one 
            coming after less than X seconds wrt the previous. X value is to be set by using the
            other packet composed by 3 bytes; by default X = 5 seconds*/
            if(count_time > timeoutMax){
                STATE = RESET;
                UART_PutString("Timeout, re-send the entire packet");
            }            
            if(flag == 1)
            {
                flag = 0;
                Timer_WriteCounter(RESET_TIMER); //each time we receive a byte we have to restart 
                count_time = 0;                  // the timer to check if the next will be inside the interval
                byte_received = UART_ReadRxData();
                packetColor[index_parser] = byte_received;
                index_parser++;
                if(index_parser == SIZE_COLOR) // if the vector is full, 
                                               //it means that all bytes were received correctly
                {
                    source = FROM_COLOR;
                    STATE = TAIL;
                }
                
            }
            
        }
        
        
        if(STATE == TIMEOUT_SET)
        {
            if(count_time > INFINITE){
                STATE = RESET;
                UART_PutString("Timeout");
            }
            if(flag == 1)
            {
                flag = 0;
                Timer_WriteCounter(RESET_TIMER);
                count_time = 0;
                packetTimeout = UART_ReadRxData();
                if(packetTimeout <= 20 && packetTimeout >= 1)
                {
                    source = FROM_TIMEOUT;
                    STATE = TAIL;
                }
                else {
                    STATE = RESET;
                    UART_PutString("Timeout out of range");
                }
            }
        }
        
        if(STATE == TAIL)
        {
            if((count_time > timeoutMax && source == FROM_COLOR) ||(count_time > INFINITE && source == FROM_TIMEOUT)){
                STATE = RESET;
                UART_PutString("Timeout");
            }
            if(flag == 1)
            {
                flag = 0;
               // Timer_WriteCounter(RESET_TIMER);
               // count_time = 0;
                byte_received = UART_ReadRxData();
                if(byte_received == 192)
                {
                    switch(source){
                        case FROM_COLOR:
                        //driving PWM: fill the color struct and lighting leds
                        color = LED_GetColor(packetColor);
                        LED_WriteColor(color);
                        break;
                        case FROM_TIMEOUT:
                        timeoutMax = packetTimeout;
                        break;
                    }
                }
                else {
                    UART_PutString("Tail unaccepted");
                }
                STATE = RESET;
            }
        }
        
        
       
    }
}

/* [] END OF FILE */

/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
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
    //Add from Utils done
    ActivateISR();
    InitializeComponents();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Initialize STATE to IDLE */
    STATE = IDLE;
    flag = 0;
    timeoutMax = 5;
    index_parser = 0;
    
    
    for(;;)
    {
        if(STATE == RESET)
        {
            index_parser = 0; 
            STATE = IDLE;
        }
        /* Place your application code here. */
        if(STATE == IDLE)
        {
            if(flag == 1)
            {
                flag = 0;
                byte_received = UART_ReadRxData();
                switch(byte_received)
                {
                    case HEADER_COLOR:
                        STATE = COLOR_SET;
                        Timer_WriteCounter(RESET_TIMER);
                        count_time = 0;
                        break;
                    case HEADER_TIMOUT:
                        STATE = TIMEOUT_SET;
                        Timer_WriteCounter(RESET_TIMER);
                        count_time = 0;
                        break;
                    
                    case 'v':
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
                    UART_PutString("Header unaccepted");
                
                }
            }    
        }
        
        
        if(STATE == COLOR_SET)
        {
            if(count_time > timeoutMax){
                STATE = RESET;
                UART_PutString("Timeout");
            }            
            if(flag == 1)
            {
                flag = 0;
                Timer_WriteCounter(RESET_TIMER);
                count_time = 0;
                byte_received = UART_ReadRxData();
                packetColor[index_parser] = byte_received;
                index_parser++;
                if(index_parser == SIZE_COLOR)
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

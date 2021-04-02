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

static char packetColor[SIZE_COLOR] = {'\0'};
static char packetTimeout;

volatile char flag, count_time, byte_received, timeoutMax;

int index_parser;
int STATE;


int main(void)
{
    Timer_Start();
    UART_Start();
    ISR_UART_StartEx(UART_ISR);
    ISR_TIMER_StartEx(TIMER_ISR);
    //Initialize PWM
    
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
                        Timer_WriteCounter(Timer_ReadPeriod());
                        count_time = 0;
                        break;
                    case HEADER_TIMOUT:
                        STATE = TIMEOUT_SET;
                        Timer_WriteCounter(Timer_ReadPeriod());
                        count_time = 0;
                        break;
                    
                    case 'v':
                        UART_PutString("RGB LED Program $$$");
                        break;
                    
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
                Timer_WriteCounter(Timer_ReadPeriod());
                count_time = 0;
                byte_received = UART_ReadRxData();
                packetColor[index_parser] = byte_received;
                index_parser++;
                if(index_parser == SIZE_COLOR)
                {
                    STATE = TAIL;
                }
                
            }
            
        }
        
        
        if(STATE == TIMEOUT_SET)
        {
            if(count_time > timeoutMax){
                STATE = RESET;
                UART_PutString("Timeout");
            }
            if(flag == 1)
            {
                flag = 0;
                Timer_WriteCounter(Timer_ReadPeriod());
                count_time = 0;
                packetTimeout = UART_ReadRxData();
                if(packetTimeout <= 20 && packetTimeout >= 1)
                {
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
            if(count_time > timeoutMax){
                STATE = RESET;
                UART_PutString("Timeout");
            }
            if(flag == 1)
            {
                flag = 0;
               // Timer_WriteCounter(Timer_ReadPeriod());
               // count_time = 0;
                byte_received = UART_ReadRxData();
                if(byte_received == 192)
                {
                    timeoutMax = packetTimeout;
                    //driving PWM: fill the color struct and lighting leds
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

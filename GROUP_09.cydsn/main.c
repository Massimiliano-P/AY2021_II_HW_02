/* ========================================
 *          /file main.c
 *  
 *   This file implements the STATE_MACHINE which controls the whole flow of bytes 
 *   characterizing packets of communication protocols. Concurrently this code drives LEDs using 
 *   provided information by calling external functions imported by means of header files
 * ========================================
*/

#include "ProjectUtils.h"
#include "Interrupts.h"
#include "project.h"


int main(void)
{
    /* Import from 'ProjectUtils.h' functions to initialize variables, components and 
       related interrupts*/
    ActivateISR();
    InitializeComponents();
    
    /*SERVICE VARIABLES*/
    char packetColor[SIZE_COLOR] = {'\0'};  // this variable stores the 3 bytes of color inside the communication packet
    char packetTimeout = timeoutMax;        // this variable stores the byte indicating the new value of timeout 
    char byte_received;                     // this variable stores the generic byte read from UART data register 
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    for(;;)
    {
        if(STATE == IDLE)
        {
            if(Received_Byte_Flag == BYTE_RECEIVED) // When the byte has been received
            {
                Received_Byte_Flag = WAITING_FOR_BYTE; // Condition to allow the toggling of the variable at next byte
                byte_received = UART_ReadRxData();
                checkHeader(byte_received);            //checks is the byte received is acceptable as protocol header
            }    
        }
        if(STATE == COLOR_SET)
        {
            /* once we are in this state, we should epect 3 subsequent bytes, each one 
            coming after less than X seconds wrt the previous. X value is to be set by using the
            other protocol for timeout set; by default X = 5 seconds*/
            
            checkTimeOverflow(Seconds, timeoutMax);         // checks if our counting variable is exceeding the Max delay
            if(Received_Byte_Flag == BYTE_RECEIVED)
            {
                Received_Byte_Flag = WAITING_FOR_BYTE;
                resetTimer();
                byte_received = UART_ReadRxData();
                storeColorByte(byte_received, packetColor); // function which stores the byte received into the array,
                                                           // checking when it's full
            }
        }
        if(STATE == TIMEOUT_SET)
        {
            /* once we are in this state, we should expect a byte reporting the new timeout time. 
            To complete the sending of this packet we let the user a fixed amount of time
            equal to 30 seconds (INFINITE), allowing the user to have time to type all bytes properly*/
            
            checkTimeOverflow(Seconds,INFINITE);    // checks if our counting variable is exceeding the Max delay
            if(Received_Byte_Flag == BYTE_RECEIVED)
            {
                Received_Byte_Flag = WAITING_FOR_BYTE;
                resetTimer();
                packetTimeout = UART_ReadRxData();
                checkTimeSelected(packetTimeout);   // function which checks that the new timeout inserted is in the allowable range; 
                                                    // if true, time is stored into the packetTimout variable provided
            }
        }
        if(STATE == TAIL)
        {
            /* Depending on which protocol we are in, same TAIL can arrive with different max delays*/
            if(source == UPDATE_COLOR){
                checkTimeOverflow(Seconds,timeoutMax);
            }
            else if(source == UPDATE_TIMEOUT){
                checkTimeOverflow(Seconds,INFINITE);
            }
            
            if(Received_Byte_Flag == BYTE_RECEIVED)
            {
                Received_Byte_Flag = WAITING_FOR_BYTE;
                byte_received = UART_ReadRxData();
                checkTail(byte_received, packetColor, packetTimeout);   // check if the tail sent is acceptable;
                                                                        // if true, depending on which protocol we are in,
                                                                        // the function drives LEDs or updates the timeout
            }
        }
   
    }
}

/* [] END OF FILE */
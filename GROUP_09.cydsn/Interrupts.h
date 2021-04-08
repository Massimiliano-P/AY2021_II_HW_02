/* ========================================
 *          /file Interrupts.h
  *
 * In this file the interrupts routines are declared
 * ========================================
*/
#ifndef __INTERRUPTS_H
    #define __INTERRUPTS_H
    
    #include "cytypes.h"

    #define BYTE_RECEIVED 1
    #define WAITING_FOR_BYTE 0
    
    volatile char Received_Byte_Flag;  // varible to keep track of triggering of UART_ISR
    volatile char Seconds;             // variable to count seconds

    CY_ISR_PROTO(UART_ISR);
    CY_ISR_PROTO(TIMER_ISR);
#endif




/* [] END OF FILE */

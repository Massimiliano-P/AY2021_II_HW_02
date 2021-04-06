/* ========================================
 *          /file Interrupts.c
 *
 * In this file the interrupts routines are defined
 * ========================================
*/

#include "project.h"
#include "Interrupts.h"
#include "UART.h"

extern volatile char flag;
extern volatile char count_time;

//This routine is called whenever a byte is received by the UART
//and it simply rises a global flag
CY_ISR(UART_ISR){
     if(UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
    {
        flag = 1;
    }
}

//This routine is called whenever the timer counter goes in overflow (1 Hz)
//and it is used to measure time in seconds
CY_ISR(TIMER_ISR){
    
    count_time++;

}

/* [] END OF FILE */

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
#include "UART.h"

extern volatile char flag;
extern volatile char count_time;

CY_ISR(UART_ISR){
     if(UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
    {
        flag = 1;
    }
}

CY_ISR(TIMER_ISR){
    
    count_time++;

}

/* [] END OF FILE */

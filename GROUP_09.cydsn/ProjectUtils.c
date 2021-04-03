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

#include "ProjectUtils.h"
#include "Interrupts.h"
#include "project.h"
#include "LEDdriver.h" 

void InitializeComponents()
{
    //Initialize PWM: LED_Start();
    Timer_Start();
    UART_Start();
}

void ActivateISR()
{
    ISR_UART_StartEx(UART_ISR);
    ISR_TIMER_StartEx(TIMER_ISR);
}

/* [] END OF FILE */

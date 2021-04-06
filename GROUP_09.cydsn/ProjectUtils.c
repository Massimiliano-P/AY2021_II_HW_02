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
    LED_Start();
    Timer_Start();
    UART_Start();
}

void ActivateISR()
{
    ISR_UART_StartEx(UART_ISR);
    ISR_TIMER_StartEx(TIMER_ISR);
}

/* [] END OF FILE */

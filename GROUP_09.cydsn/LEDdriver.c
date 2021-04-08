/* ========================================
 *      /file LEDdriver.c
 *
 *  This file defines the driver functions
 * ========================================
*/
#include "LEDdriver.h"
#include "project.h"

void LED_Start()
{
    PWM_RG_Start();
    PWM_B_Start();
}

void LED_UpdateColor(char color_array[])
{
    PWM_RG_WriteCompare1(color_array[0]);
    PWM_RG_WriteCompare2(color_array[1]);
    PWM_B_WriteCompare(color_array[2]);
}
/* [] END OF FILE */

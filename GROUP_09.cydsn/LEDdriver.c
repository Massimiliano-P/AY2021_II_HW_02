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

void LED_WriteColor(Color c)
{
    PWM_RG_WriteCompare1(c.red);
    PWM_RG_WriteCompare2(c.green);
    PWM_B_WriteCompare(c.blue);
}

Color LED_GetColor(char color_array[])
{
    Color color;
    color.red = color_array[0];
    color.green = color_array[1];
    color.blue = color_array[2];
    return color;

}
/* [] END OF FILE */

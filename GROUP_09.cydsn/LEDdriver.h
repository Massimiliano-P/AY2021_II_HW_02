/* ========================================
 *      /file LEDdriver.h
 * 
 *  This file defines struct and functions for the LED driver
 * ========================================
*/

#ifndef __LED_DRIVER_H__
    #define __LED_DRIVER_H__
    
    //this function initializes the PWMs
    void LED_Start(void);
    
    //this function sets the PWM duty cycles to adjust the RGB LED color
    void LED_UpdateColor(char color_array[]);
    
#endif




/* [] END OF FILE */

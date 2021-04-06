/* ========================================
 *      /file LEDdriver.h
 * 
 *  This file defines struct and functions for the LED driver
 * ========================================
*/

#ifndef __LED_DRIVER_H__
    #define __LED_DRIVER_H__
    
    // we defined a struct to store the intensity values for each RGB channel
    typedef struct
    {
        char red;
        char green; 
        char blue;
    } Color;
    
    //this function initializes the PWMs
    void LED_Start(void);
    
    //this function sets the PWM duty cycles to adjust the RGB LED color
    void LED_WriteColor(Color c);
    
    //since in the main file the color bytes are stored in a vector using a for cycle,
    //this function gets those bytes and puts them in a Color type variable
    Color LED_GetColor(char color_array[]);
    
#endif




/* [] END OF FILE */

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

#ifndef __LED_DRIVER_H__
    #define __LED_DRIVER_H__
    
    typedef struct
    {
        char red;
        char green; 
        char blue;
    } Color;
    
    void LED_Start(void);
    void LED_WriteColor(Color c);
    Color LED_GetColor(char color_array[]);
    
#endif




/* [] END OF FILE */

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
#ifndef _LED_H
#define	_LED_H

#include <project.h>
    
    
typedef struct Led {
    uint16 high;
    uint16 low;
}Led_PWM;   
void LED_loop();
void LED_SET_NUM(uint8 num);
#endif
/* [] END OF FILE */

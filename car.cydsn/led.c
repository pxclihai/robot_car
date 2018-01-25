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

/* [] END OF FILE */
#include "led.h"
#include "car.h"

static uint8 led_togle;
static uint8 led_num;
static uint8 led_cur;
static uint8 led_per;

void led_init()
{
    
}
void LED_SET_NUM(uint8 num)
{
    led_num = num;
}
void LED_loop()
{
    int i ;
    
    
    if(led_num >0)
    {
        led_togle ++;
        if(led_togle == 1)
        {
          PWM_LED_1_WriteCompare(0);
          PWM_LED_2_WriteCompare(0);
          PWM_LED_3_WriteCompare(0);
          PWM_LED_4_WriteCompare(0);
        debug_time_Write(0);
        }  
        else if(led_togle == 2)
        {   
          PWM_LED_1_WriteCompare(10000);
          PWM_LED_2_WriteCompare(10000);
          PWM_LED_3_WriteCompare(10000);
          PWM_LED_4_WriteCompare(10000);
           debug_time_Write(1);
          led_togle = 0;
          led_num--;
        }
    }
    led_cur = g_Car.error;
    if(led_cur != led_per)
    {
       if(g_Car.error == 1)
       {
         LED_SET_NUM(30);
       }
       else
       {
         LED_SET_NUM(1);
       }
      led_per = led_cur;
    }

    

}

void LED_DIS_ERROR(uint8 num)
{
   

}
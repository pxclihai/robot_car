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
#ifndef _CAR_H
#define	_CAR_H
#include <project.h>
enum CAR_DIR{UP,DOWN,LEFT,RIGHT,
             UP_LEFT,UP_RIGHT,DOWN_LEFT,DOWN_RIGHT,
             STOP,WAIT_DIR,
            };

enum WHEEL_DIR{FORWARD,REVERSAL,WHEEL_STOP};

typedef struct 
{
   uint16 pwm;
   enum WHEEL_DIR dir;
}S_WHEEL;  
typedef struct 
{
    uint8 Car_lock;
    enum CAR_DIR set_dir;
    enum CAR_DIR pre_dir;
    uint16 set_car_speed;
    S_WHEEL  left_front_wheel;
    S_WHEEL  left_behind_wheel;
    S_WHEEL  right_front_wheel;
    S_WHEEL  right_behind_wheel;
    uint16 steering_cur_positon1;
    uint16 steering_set_positon1;
    uint16 steering_cur_positon2;
    uint16 steering_set_positon2;
    
}S_CAR;
S_CAR g_Car;
void Car_init();

void Control_Car();
#endif
/* [] END OF FILE */

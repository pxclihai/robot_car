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
    uint32    NULL_S;
    uint16    totol_run_count;
    uint16    cur_run_time;
    uint16    car_runing_time;
    uint16    last_run_time;
    uint32    totol_run_time;
    uint16    hardware_ver;
    uint16    software_ver;    
}S_SYSTEMINFO;
typedef struct 
{
    uint8 Car_lock;
    uint8 error;
    enum CAR_DIR set_dir;
    enum CAR_DIR set_ptz_dir;
    enum CAR_DIR pre_dir;
    uint16 set_car_speed;
    S_WHEEL  left_front_wheel;
    S_WHEEL  left_behind_wheel;
    S_WHEEL  right_front_wheel;
    S_WHEEL  right_behind_wheel;
    uint16   steering_cur_positon1;
    uint16   steering_set_positon1;
    uint16   steering_cur_positon2;
    uint16   steering_set_positon2;
    uint8    hearting;
    uint16   distance_front;  
    uint16   distance_back;
    uint8    measure_distance_flag;
    int16    Xkm;
    uint8    M_Command;
    S_SYSTEMINFO systeminfo;
}S_CAR;


S_CAR g_Car;
void Car_init();
void Command_Car_brake();
void Car_Control_Loop();
void Car_Hearting_Loop();
#endif
/* [] END OF FILE */

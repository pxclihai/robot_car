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
#include "car.h"

static void Command_Car_UP(uint16 speed);
static void Command_Car_Down(uint16 speed);
void Car_Dir(enum CAR_DIR car_dir,uint16 car_speed);
void Car_PTZ_DIR(S_CAR car);
void Car_PUSH_DIR(S_CAR car);
void Command_Car_brake();
void Command_Car_Left(uint16 speed);
void Command_Car_Right(uint16 speed);
void board_test();

static void Set_Left_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void Set_Left_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void Set_Right_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void Set_Right_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void ptz_control(uint8 choose,enum WHEEL_DIR dir);


void Car_init()
{
    
    g_Car.systeminfo.hardware_ver = 410;
    g_Car.systeminfo.software_ver = 110;
    
    g_Car.pre_dir = STOP;
    g_Car.set_car_speed = 990;
    g_Car.set_dir = STOP;
    g_Car.set_ptz_dir = STOP;  
    g_Car.set_push_dir = STOP;
    PTZ_UP_EN_Write(1);
    PTZ_UP_DIR_Write(0);
    PTZ_UP_STEP_Write(0); 
    PTZ_DOWN_EN_Write(1);
    PTZ_DOWN_DIR_Write(0);
    PTZ_DOWN_STEP_Write(0);
    
    PWM_LEFT_EN_Write(1);
    PWM_RIGHT_EN_Write(1);

}
void board_test()
{
    
    if(0)
    {
         ptz_control(0,REVERSAL);
    }
    else if(1)
    {
         ptz_control(0,FORWARD);
    }
    if(0)
    {
         ptz_control(1,REVERSAL);
    }
    else if(1)
    {
         ptz_control(1,FORWARD);
    }
    else 
    {
        ptz_control(0,WHEEL_STOP);
        ptz_control(1,WHEEL_STOP);
    }
    
}
void Car_Control_Loop()
{  
    if( g_Car.error == 0 )
    {
        Car_Dir(g_Car.set_dir,g_Car.set_car_speed); 
        Car_PTZ_DIR(g_Car); 
        Car_PUSH_DIR(g_Car);
        if(g_Car.liheqi == 1)
        {
            EM1_Write(1);
            EM2_Write(1); 
        }
        else
        {
            EM1_Write(0);
            EM2_Write(0);
        }
    }   
    else
    {
        g_Car.set_dir = STOP;
        Command_Car_brake();
        EM1_Write(1);
        EM2_Write(1);
    }

 // board_test();
}
void Car_Hearting_Loop()
{
    if(g_Car.hearting ==  1   )
    {
        g_Car.hearting = 0;
        g_Car.error = 0;
    }
    else
    {
        g_Car.error = 1;
    }  
}
void Car_PTZ_DIR(S_CAR car)
{
    if(car.set_ptz_dir == UP)
    {
         ptz_control(0,REVERSAL);
    }
    else if(car.set_ptz_dir == DOWN)
    {
         ptz_control(0,FORWARD);
    }
    else if(car.set_ptz_dir == LEFT)
    {
         ptz_control(1,REVERSAL);
    }
    else if(car.set_ptz_dir == RIGHT)
    {
         ptz_control(1,FORWARD);
    }
    else 
    {
        ptz_control(0,WHEEL_STOP);
        ptz_control(1,WHEEL_STOP);
    }
}
void Car_PUSH_DIR(S_CAR car)
{
    static uint8 togle_flag = 0;
    
    if(car.set_push_dir == UP)
    {
        PUSH_EN_Write(0);
        PUSH_DIR_Write(REVERSAL);
        if(togle_flag == 0)
        {
            PUSH_STEP_Write(1);
            togle_flag = 1;
        }
        else
        {
            PUSH_STEP_Write(0);
            togle_flag = 0;
        }
    }
    else if (car.set_push_dir == DOWN)
    {
        PUSH_EN_Write(0);
        PUSH_DIR_Write(FORWARD);
        if(togle_flag == 0)
        {
            PUSH_STEP_Write(1);
          
            togle_flag = 1;
        }
        else
        {
            PUSH_STEP_Write(0);
            togle_flag = 0;
        } 
    }
    else
    {
         PUSH_EN_Write(1);
    }

    
}
void ptz_control(uint8 choose,enum WHEEL_DIR dir)
{
    static uint8 togle_flag_1 = 0;
    static uint8 togle_flag_0 = 0;
    if(choose == 1 && dir != WHEEL_STOP)
    {
        PTZ_UP_EN_Write(0);
        PTZ_UP_DIR_Write(!dir);
        
        if(togle_flag_1 == 0)
        {
            PTZ_UP_STEP_Write(1);
            togle_flag_1 = 1;
        }
        else
        {
            PTZ_UP_STEP_Write(0);
            togle_flag_1 = 0;
        }
    }
    else if(choose == 0 && dir != WHEEL_STOP)
    {
        
        PTZ_DOWN_EN_Write(0);
        PTZ_DOWN_DIR_Write(dir);
        if(togle_flag_0 == 0)
        {
            PTZ_DOWN_STEP_Write(1);
          
            togle_flag_0 = 1;
        }
        else
        {
            PTZ_DOWN_STEP_Write(0);
            togle_flag_0 = 0;
        } 
    }
    else
    {
        PTZ_UP_EN_Write(1);
        PTZ_DOWN_EN_Write(1);
    }
}
uint8 deal_break(S_CAR car)
{
    static uint16 brake_time;
    Command_Car_brake();
       brake_time++;
       if(brake_time>20)
       {
         brake_time = 0;
         car.pre_dir = car.set_dir;
         return 0;
       }
    return 1;
}
void Car_Dir(enum CAR_DIR car_dir,uint16 car_speed)
{
 
    if(car_dir == UP)
    {
       
       Command_Car_UP(car_speed);
    }
    else if(car_dir == DOWN)
    {
       Command_Car_Down(car_speed); 
    }
    else if(car_dir == LEFT)
    {
       Command_Car_Left(car_speed); 
    }
    else if(car_dir == RIGHT)
    {
       Command_Car_Right(car_speed); 
    }
    else if(car_dir == STOP)
    {
       Command_Car_brake();
    }

}
void Command_Car_Down(uint16 speed)
{
  Set_Left_Front_Speed_Turn(speed,REVERSAL);
  Set_Left_Behind_Speed_Turn(speed,REVERSAL);
  Set_Right_Behind_Speed_Turn(speed,REVERSAL);
  Set_Right_Behind_Speed_Turn(speed,REVERSAL);

}

void Command_Car_UP(uint16 speed)
{
  Set_Left_Front_Speed_Turn  (speed,FORWARD);
  Set_Left_Behind_Speed_Turn (speed,FORWARD);
  Set_Right_Behind_Speed_Turn(speed,FORWARD);
  Set_Right_Behind_Speed_Turn(speed,FORWARD);
}

void Command_Car_Left(uint16 speed)
{
  Set_Left_Front_Speed_Turn  (speed/2,FORWARD);
  Set_Left_Behind_Speed_Turn (speed/2,FORWARD);
  Set_Right_Behind_Speed_Turn(speed,REVERSAL);
  Set_Right_Behind_Speed_Turn(speed,REVERSAL);
}

void Command_Car_Right(uint16 speed)
{
  Set_Left_Front_Speed_Turn  (speed,REVERSAL);
  Set_Left_Behind_Speed_Turn (speed,REVERSAL);
  Set_Right_Behind_Speed_Turn(speed/2,FORWARD);
  Set_Right_Behind_Speed_Turn(speed/2,FORWARD);
}

void Command_Car_brake()
{
  Set_Left_Front_Speed_Turn  (0,WHEEL_STOP);
  Set_Left_Behind_Speed_Turn (0,WHEEL_STOP);
  Set_Right_Behind_Speed_Turn(0,WHEEL_STOP);
  Set_Right_Behind_Speed_Turn(0,WHEEL_STOP);
}

//set every one motor
void Set_Left_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{
    if(speed > 990)
    {
        speed = 990;
    }
    if(turn == FORWARD)
        { PWM_MOTOR_LEFT_WriteCompare1(0);PWM_MOTOR_LEFT_WriteCompare2(speed);}
    else if(turn == REVERSAL)
        { PWM_MOTOR_LEFT_WriteCompare1(speed);PWM_MOTOR_LEFT_WriteCompare2(0);}
    else
         {PWM_MOTOR_LEFT_WriteCompare1(0);PWM_MOTOR_LEFT_WriteCompare2(0);}
}
void Set_Left_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{
    if(speed > 990)
    {
        speed = 990;
    }
    if(turn == FORWARD)
        {  PWM_MOTOR_RIGHT_WriteCompare1(0);PWM_MOTOR_RIGHT_WriteCompare2(speed);}
    else if(turn == REVERSAL)
        { PWM_MOTOR_RIGHT_WriteCompare1(speed);PWM_MOTOR_RIGHT_WriteCompare2(speed);}
    else
         {PWM_MOTOR_RIGHT_WriteCompare1(0);PWM_MOTOR_RIGHT_WriteCompare2(0);}
}
void Set_Right_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{    if(speed > 990)
    {
        speed = 990;
    }
  if(turn == FORWARD)
        {  PWM_MOTOR_RIGHT_WriteCompare1(0);PWM_MOTOR_RIGHT_WriteCompare2(speed);}
    else if(turn == REVERSAL)
        { PWM_MOTOR_RIGHT_WriteCompare1(speed);PWM_MOTOR_RIGHT_WriteCompare2(0);}
    else
         {PWM_MOTOR_RIGHT_WriteCompare1(0);PWM_MOTOR_RIGHT_WriteCompare2(0);}
}
void Set_Right_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{    if(speed > 990)
    {
        speed = 990;
    }
  if(turn == FORWARD)
        {  PWM_MOTOR_RIGHT_WriteCompare1(0);PWM_MOTOR_RIGHT_WriteCompare2(speed);}
    else if(turn == REVERSAL)
        { PWM_MOTOR_RIGHT_WriteCompare1(speed);PWM_MOTOR_RIGHT_WriteCompare2(0);}
    else
         {PWM_MOTOR_RIGHT_WriteCompare1(0);PWM_MOTOR_RIGHT_WriteCompare2(0);}
}
/* [] END OF FILE */

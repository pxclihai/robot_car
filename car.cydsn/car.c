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
void Car_Dir(S_CAR car);
void Car_PTZ_DIR(S_CAR car);
void Command_Car_brake();
void Command_Car_Left(uint16 speed);
void Command_Car_Right(uint16 speed);

static uint8 togle_flag;
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
    g_Car.set_car_speed = 14000;
    g_Car.set_dir = STOP;
    g_Car.set_ptz_dir = LEFT;  
    PTZ_UP_EN_Write(1);
    PTZ_UP_DIR_Write(0);
    PTZ_UP_STEP_Write(0);
    PTZ_DOWN_EN_Write(1);
    PTZ_DOWN_DIR_Write(0);
    PTZ_DOWN_STEP_Write(0);
}
void Car_Control_Loop()
{  
    if(g_Car.error == 0)
    {
    Car_Dir(g_Car); 
    Car_PTZ_DIR(g_Car); 
    }   
    else
    {
    g_Car.set_dir = STOP;
    Command_Car_brake();
    }

}
void Car_Hearting_Loop()
{
    if(g_Car.hearting == 1  )
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
         ptz_control(0,FORWARD);
    }
    else if(car.set_ptz_dir == DOWN)
    {
         ptz_control(0,REVERSAL);
    }
    else if(car.set_ptz_dir == LEFT)
    {
         ptz_control(1,FORWARD);
    }
    else if(car.set_ptz_dir == RIGHT)
    {
         ptz_control(1,REVERSAL);
    }
    else 
    {
        ptz_control(0,WHEEL_STOP);
        ptz_control(1,WHEEL_STOP);
    }

}
void ptz_control(uint8 choose,enum WHEEL_DIR dir)
{
    if(choose == 0 && dir != WHEEL_STOP)
    {
       PTZ_UP_EN_Write(0);
        PTZ_UP_DIR_Write(dir);
        
        if(togle_flag == 0)
        {
            PTZ_UP_STEP_Write(1);
            togle_flag = 1;
        }
        else
        {
            PTZ_UP_STEP_Write(0);
            togle_flag = 0;
        }
        
    }
    else if(choose == 1 && dir != WHEEL_STOP)
    {
        
        PTZ_DOWN_EN_Write(0);
        PTZ_DOWN_DIR_Write(dir);
        if(togle_flag == 0)
        {
            PTZ_DOWN_STEP_Write(1);
          
            togle_flag = 1;
        }
        else
        {
            PTZ_DOWN_STEP_Write(0);
            togle_flag = 0;
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
void Car_Dir(S_CAR car)
{
    if(car.set_dir == UP)
    {
       Command_Car_UP(car.set_car_speed);
    }
    if(car.set_dir == DOWN)
    {
       Command_Car_Down(car.set_car_speed); 
    }
    if(car.set_dir == LEFT)
    {
       Command_Car_Left(car.set_car_speed); 
    }
    if(car.set_dir == RIGHT)
    {
       Command_Car_Right(car.set_car_speed); 
    }
    if(car.set_dir == STOP)
    {
       Command_Car_brake();
    }
//    if(car.pre_dir != car.set_dir)
//    {
//        if(car.pre_dir == STOP)
//        {
//           if(car.set_dir == UP)
//           {
//            if(deal_break(car) == 0)
//            {
//              Command_Car_UP(car.set_car_speed);
//            }
//           }
//           if(car.set_dir == DOWN)
//           {
//            if(deal_break(car) == 0)
//            {
//              Command_Car_Down(car.set_car_speed);
//            }
//           }
//        }
//        if(car.pre_dir == UP )
//        {
//          if(car.set_dir == STOP)
//          {
//            if(deal_break(car) == 0)
//            {
//            }
//          }
//         
//          if( car.set_dir == DOWN)
//          {
//             if(deal_break(car) == 0)
//            {
//              Command_Car_Down(car.set_car_speed);
//            }
//          }
//        
//        }
//        if(car.pre_dir == DOWN )
//        {
//          if(car.set_dir == STOP)
//          {
//            if(deal_break(car) == 0)
//            {
//            }
//          }
//         
//          if( car.set_dir == UP)
//          {
//             if(deal_break(car) == 0)
//            {
//              Command_Car_UP(car.set_car_speed);
//            }
//          }
//        
//        }
//       
//    }
}
void Command_Car_UP(uint16 speed)
{
  Set_Left_Front_Speed_Turn(speed,FORWARD);
  Set_Left_Behind_Speed_Turn(speed,FORWARD);
  Set_Right_Behind_Speed_Turn(speed,FORWARD);
  Set_Right_Behind_Speed_Turn(speed,FORWARD);

}
void Command_Car_Down(uint16 speed)
{
  Set_Left_Front_Speed_Turn  (speed,REVERSAL);
  Set_Left_Behind_Speed_Turn (speed,REVERSAL);
  Set_Right_Behind_Speed_Turn(speed,REVERSAL);
  Set_Right_Behind_Speed_Turn(speed,REVERSAL);
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
    PWM_MOTOR_WriteCompare1(speed);
    if(turn == FORWARD)
        { L_IN_1_Write(1);L_IN_2_Write(0);}
    else if(turn == REVERSAL)
        { L_IN_1_Write(0);L_IN_2_Write(1);}
    else
         {L_IN_1_Write(0);L_IN_2_Write(0);}
}
void Set_Left_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{
    PWM_MOTOR_WriteCompare1(speed);
    if(turn == FORWARD)
        { L_IN_1_Write(1);L_IN_2_Write(0);}
    else if(turn == REVERSAL)
        { L_IN_1_Write(0);L_IN_2_Write(1);}
    else
         {L_IN_1_Write(0);L_IN_2_Write(0);}
}
void Set_Right_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{
    PWM_MOTOR_WriteCompare2(speed);
    if(turn == FORWARD)
        { R_IN_1_Write(1);R_IN_2_Write(0);}
    else if(turn == REVERSAL)
        { R_IN_1_Write(0);R_IN_2_Write(1);}
    else
         {R_IN_1_Write(0);R_IN_2_Write(0);}
}
void Set_Right_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn)
{
    PWM_MOTOR_WriteCompare2(speed);
    if(turn == FORWARD)
        { R_IN_1_Write(1);R_IN_2_Write(0);}
    else if(turn == REVERSAL)
        { R_IN_1_Write(0);R_IN_2_Write(1);}
    else
         {R_IN_1_Write(0);R_IN_2_Write(0);}
}
/* [] END OF FILE */

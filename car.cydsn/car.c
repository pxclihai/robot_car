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
static void Set_Left_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void Set_Left_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void Set_Right_Front_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
static void Set_Right_Behind_Speed_Turn(uint16 speed,enum WHEEL_DIR turn);
void Command_Car_brake();
void Command_Car_Left(uint16 speed);
void Command_Car_Right(uint16 speed);
void Control_Car()
{
    Car_Dir(g_Car); 
    
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

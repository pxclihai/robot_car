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
#include <project.h>
#include "Transfer.h"
#include "car.h"
#include "battery.h"
#include "Lidar.h"
int16 Get_Gap_ADvalue(void);
int16 contronl_steering(uint16 set_position,uint16 cur_postion,uint16 step);

extern uint8 time_5ms_state;
extern uint8 time_1s_state;
extern uint16 time_100ms_state;
    int16 temp;
float tset;
uint16 test;
uint16 send_count_test;
int main()
{
    


    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_net_Start();
    UART_JY_Start();
    UART_LIDAR_Start();
    isr_rx_net_Start();
    isr_tx_net_Start();
    isr_rx_jy_Start();
    isr_rx_LIDAR_Start();

    PWM_MOTOR_Init();
    PWM_MOTOR_Start();
    
    PWM_LED_Init();
    PWM_LED_Start();
    PWM_LED_1_Init();
    PWM_LED_1_Start();

    PWM_STEERING_Init();
    PWM_STEERING_Start();
    Timer_1_Start(); 
    isr_timer_Start();
  //  isr_net_Start();
  //  PWM_MOTOR_WriteCompare1(100);
    g_Car.pre_dir = STOP;
    g_Car.set_car_speed = 14000;
    g_Car.set_dir = STOP;
//    g_Car.steering_cur_positon = 1500;
//    g_Car.steering_set_positon = 1500;
    
    Monitor_Battery_Init();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    CyDelay(500);
    Lidar_init();
  //  UART_net_PutString("----------------------OK!");
    for(;;)
    {
        /* Place your application code here. */
      
     
     
        if(time_1s_state == 1)
        {
          time_1s_state = 0;

        } 
        
        //C_V = ((Get_C_Battery_ADvalue() * 825)>>10);
        if(time_5ms_state == 1)
        { 
        //    DT_Data_Exchange();
            Control_Car();
            time_5ms_state = 0;
// test = Get_Gap_ADvalue();//还没测试
//           temp = contronl_steering(g_Car.steering_set_positon1,g_Car.steering_cur_positon1,1);
//           PWM_STEERING_WriteCompare1(g_Car.steering_cur_positon1+temp);
//          temp = contronl_steering(g_Car.steering_set_positon2,g_Car.steering_cur_positon2,1);
//          PWM_STEERING_WriteCompare1(g_Car.steering_cur_positon2+temp);
//        if(send_count_test < 1000)
//        { 
//        send_count_test++;
//        DT_Send_Lidar_normal(net_nodeBuffer);
//        }
        }
        if(time_100ms_state == 1)
        {
            C_V = Get_Battery_ADvalue(C_CHANANL);
            D_V = Get_Battery_ADvalue(D_CHANANL);
            time_100ms_state = 0;        
        //   UART_LIDAR_PutChar('A');
        }

       // scan_lidar();
       if(rx_lidar_flag == 1)
    {
          debug_time_Write(1);
          DT_Send_Lidar_normal(net_nodeBuffer); 
          rx_lidar_flag = 0; 
          debug_time_Write(0);
     } 
 
    }   
}


int16 contronl_steering(uint16 set_position,uint16 cur_postion,uint16 step)
{
   if(set_position > cur_postion)
   {
        return -step;
   }
   else if(set_position <cur_postion)
  {
    return step;
  } 
  else 
  {
    return 0;
  } 
}

/* [] END OF FILE */

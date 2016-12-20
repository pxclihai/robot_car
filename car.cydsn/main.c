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
#include "multi_timer.h"
#include "systemInfo.h"
#include "led.h"
int16 Get_Gap_ADvalue(void);
int16 contronl_steering(uint16 set_position,uint16 cur_postion,uint16 step);

int16 temp;
float tset;
uint16 test;
uint16 send_count_test;
uint16 count;

int16 encoder;
uint8 event;

void Cal_time();
struct Timer Data_exchange_timer;
struct Timer Time_1s;
struct Timer Cal_battery_timer;
struct Timer Car_control_timer;
struct Timer Car_hearting_timer;
struct Timer SRF_timer;
struct Timer LED_timer;
void SRF_loop();


int main()
{
  
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    EEPROM_Start();
    SystemInfo_Init();

    UART_net_Start();
    UART_JY_Start();

    isr_rx_net_Start();
    isr_tx_net_Start();
    isr_rx_jy_Start();
    PWM_MOTOR_Init();
    PWM_MOTOR_Start();

    PWM_LED_1_Init();
    PWM_LED_1_Start();
    PWM_LED_2_Init();
    PWM_LED_2_Start();
    PWM_LED_3_Init();
    PWM_LED_3_Start();
    PWM_LED_4_Init();
    PWM_LED_4_Start();
    
    Timer_1_Start(); 
    isr_timer_Start();
    
    SRF_05_Timer_Start();
    SRF_05_Timer_1_Start();
    isr_srf_front_Start();
    isr_srf_back_Start();
    
    SRF_TRG_1_Write(0);
    SRF_TRG_2_Write(0);
    Monitor_Battery_Init(); 
    
    QuadDec_1_Start();
    ///////////////////
    ///////////////// 
   CONTRAL_LIDAR_Write(1);
   CyGlobalIntEnable; /* Enable global interrupts. */
  
   Lidar_init();
   Car_init();   

   timer_init(&Data_exchange_timer, DT_Data_Exchange, 1000, 10);
   timer_start(&Data_exchange_timer);

   timer_init (&Time_1s, Cal_time, 5000, 1000);
   timer_start(&Time_1s);
   timer_init (&Cal_battery_timer, Cal_Battery_loop, 100, 100);
   timer_start(&Cal_battery_timer);

   timer_init (&Car_hearting_timer, Car_Hearting_Loop, 0, 1000);
   timer_start(&Car_hearting_timer);
   
   timer_init (&Car_control_timer, Car_Control_Loop, 150, 3);
   timer_start(&Car_control_timer);

   timer_init (&SRF_timer, SRF_loop, 50, 90);
   timer_start(&SRF_timer);
   CyDelay(500);
   timer_init (&LED_timer, LED_loop, 1500, 500);
   timer_start(&LED_timer);
  
 //  LED_SET_NUM(3);
    for(;;)
    {
        /* Place your application code here. */
        timer_loop();
// scan_lidar();
//       if(rx_lidar_flag == 1)
//    {
//          debug_time_Write(1);
//          DT_Send_Lidar_normal(net_nodeBuffer); 
//          rx_lidar_flag = 0; 
//          debug_time_Write(0);
//     } 

        
    }   
}

void SRF_loop()
{
    SRF_TRG_1_Write(1);
    CyDelayUs(20);
    SRF_TRG_1_Write(0); 
    CyDelayUs(20);

    SRF_TRG_2_Write(1);
    CyDelayUs(20);
    SRF_TRG_2_Write(0);
}
/* [] END OF FILE */

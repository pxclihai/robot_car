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
#include "wave_distance.h"
#include "M_thickness.h"


int16 Get_Gap_ADvalue(void);
int16 contronl_steering(uint16 set_position,uint16 cur_postion,uint16 step);

int16 temp;
float tset;
uint16 test;
uint16 send_count_test;
uint16 count;

int16 encoder;
uint8 event;


struct Timer Data_exchange_timer;   
struct Timer Time_1s;              
struct Timer Cal_battery_timer;    
struct Timer Car_control_timer;    
struct Timer Car_hearting_timer;   
struct Timer WAVE_timer;          
struct Timer LED_timer;           
struct Timer ECDR_timer;          
struct Timer M_timer;              
struct Timer M_50s;
void WAVE_loop();               
void Cal_time();
void ECDR_loop();

void M_PWR(uint8 temp_switch);
extern uint16 time_55s_state;   


int main()
{
  
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    EEPROM_Start();
    SystemInfo_Init();

    UART_net_Start();
    UART_JY_Start();
    UART_CH_Start();
    
    isr_rx_net_Start();
    isr_tx_net_Start();
    isr_rx_jy_Start();
    isr_rx_ch_Start();
    PWM_MOTOR_LEFT_Init();
    PWM_MOTOR_LEFT_Start();
    
    
    PWM_MOTOR_RIGHT_Init();
    PWM_MOTOR_RIGHT_Start();
    
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
    
    Monitor_Battery_Init(); 
    
    ECDR_Start();
    ///////////////////
    ///////////////// 
    CONTRAL_LIDAR_Write(1);
//    WAVE_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
 
    Lidar_init();
    Car_init();   
    ////test
//    PWM1_Write(1);
//    CyDelay(500);
//    IN2_Write(0);
//    CyDelay(500);
//    PWM2_Write(1);
//    IN4_Write(0);
//
//    CyDelay(500);
// 
//    CyDelay(500);
//    IN4_Write(0);
    
    
    
    ////
    timer_init (&Data_exchange_timer, DT_Data_Exchange, 1000, 10);
    timer_start(&Data_exchange_timer);
    
    timer_init (&Time_1s, Cal_time, 5000, 1000);
    timer_start(&Time_1s);
    
    timer_init (&Cal_battery_timer, Cal_Battery_loop, 100, 100);
    timer_start(&Cal_battery_timer);
    
    timer_init (&Car_hearting_timer, Car_Hearting_Loop, 0, 1000);
    timer_start(&Car_hearting_timer);
    
    timer_init (&Car_control_timer, Car_Control_Loop, 150,2);
    timer_start(&Car_control_timer);
    
//    timer_init (&WAVE_timer, WAVE_loop, 50, 200);
//    timer_start(&WAVE_timer);
    CyDelay(500);
    timer_init (&LED_timer, LED_loop, 1500, 500);
    timer_start(&LED_timer);
    
    timer_init (&ECDR_timer, ECDR_loop, 100, 500);
    timer_start(&ECDR_timer);
    timer_init (&M_timer, Control_M_loop, 500, 1000);
    timer_start(&M_timer);

    
    g_Car.M_Command = M_POWER_ON;
    
    for(;;)
    {
        /* Place your application code here. */
        timer_loop();  
    }   
}
void ECDR_loop()
{
    g_Car.Xkm = ECDR_GetCounter();
}
void WAVE_loop()
{
   // K103_read_distance();
}


/* [] END OF FILE */

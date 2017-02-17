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
#include "M_thickness.h"
#include "Car.h"
#define NONE      0
#define POWER_ON  1
#define SHUT_DOWN 2
#define UP        3
#define DOWN      4
#define ZERO      5

uint8 change_flag = 0;

void Control_M_loop()
{
     if(g_Car.M_Command == POWER_ON)
    {
        if(change_flag == 0) 
        {
            M_PWR_Write(1) ;//导通继电器输出5v
            M_DOWN_Write(0);
            M_UP_Write(0)  ;
            M_ZERO_Write(0);
            
            change_flag = 1;
        }
        else
        {
            M_PWR_Write(0);
            change_flag = 0; 
            g_Car.M_Command = NONE;
        }
      
    }
    else if(g_Car.M_Command == SHUT_DOWN)
    {
        if(change_flag == 0) 
        {
            M_PWR_Write(1) ;//导通继电器输出5v
            
            
            change_flag = 1;
        }
        else
        { 
            M_PWR_Write(0);
            M_DOWN_Write(1);
            M_UP_Write(1)  ;
            M_ZERO_Write(1);
            change_flag = 0; 
            g_Car.M_Command = NONE;
        }
    }
    else if (g_Car.M_Command == UP)
    {
        if(change_flag == 0) 
        {
            M_UP_Write(1) ;//导通继电器输出5v
            change_flag = 1;
        }
        else
        { 
            M_UP_Write(0);
       
            change_flag = 0; 
            g_Car.M_Command = NONE;
        }
    }
      else if (g_Car.M_Command == DOWN)
    {
        if(change_flag == 0) 
        {
            M_DOWN_Write(1) ;//导通继电器输出5v
            change_flag = 1;
        }
        else
        { 
            M_DOWN_Write(0);
       
            change_flag = 0; 
            g_Car.M_Command = NONE;
        }
    }
    else if (g_Car.M_Command == ZERO)
    {
        if(change_flag == 0) 
        {
            M_ZERO_Write(1) ;//导通继电器输出5v
            change_flag = 1;
        }
        else
        { 
            M_ZERO_Write(0);
       
            change_flag = 0; 
            g_Car.M_Command = NONE;
        }
    }
    
}


void M_Data_Receive_Prepare(uint8 data)
{
	static uint8 RxBuffer[50];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	
	if(state==0&&data==16)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==8)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data==7) //fuciton
	{
		state=3;
		RxBuffer[2]=data;
        _data_len = 7;
		_data_cnt = 0;
	}
	else if(state==3&&_data_len==18)//data
	{
		_data_len--;
		RxBuffer[3+_data_cnt++]=data;
		if(_data_len==0)
		   state = 4;
	}
	else if(state==4)//sum
	{
		state = 0;
		RxBuffer[3+_data_cnt]=data;
		//DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
    

}
/* [] END OF FILE */

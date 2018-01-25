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


uint8 change_flag = 0;
void M_Data_Receive_Anl(uint8 *data_buf,uint8 num);
void shutdown_50s()
{
 
     M_DOWN_Write(1);
     M_UP_Write(1);
     M_ZERO_Write(1);
}
void Control_M_loop()
{
    if(g_Car.M_Command == M_POWER_ON)
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
            g_Car.M_Command = M_NONE;
            g_Car.shutdown_flag = 1;
        }
        
    }
    else if(g_Car.M_Command == M_SHUT_DOWN)
    {
        if(change_flag == 0) 
        {
        
            change_flag = 1;
        }
        else
        { 
    
            
            M_DOWN_Write(1);
            M_UP_Write(1);
            M_ZERO_Write(1);
          
            change_flag = 0; 
            g_Car.M_Command = M_NONE;
        }
    }
    else if (g_Car.M_Command == M_UP)
    {
        if(change_flag == 0) 
        {
            M_UP_Write(1) ;
            change_flag = 1;
        }
        else
        { 
            M_UP_Write(0);
       
            change_flag = 0; 
            g_Car.M_Command  =  M_NONE;
             g_Car.shutdown_flag = 1;
        }
    }
    else if (g_Car.M_Command == M_DOWN)
    {
        if(change_flag == 0) 
        {
            M_DOWN_Write(1) ;
            change_flag = 1;
        }
        else
        { 
            M_DOWN_Write(0);
       
            change_flag = 0; 
            g_Car.M_Command = M_NONE;
             g_Car.shutdown_flag = 1;
        }
    }
    else if (g_Car.M_Command == M_ZERO)
    {
        if(change_flag == 0) 
        {
            M_ZERO_Write(1) ;
            change_flag = 1;
        }
        else
        { 
            M_ZERO_Write(0);
       
            change_flag = 0; 
            g_Car.M_Command = M_NONE;
             g_Car.shutdown_flag = 1;
        }
    }
    
}
 static uint8 temp[50];
    static uint16 count;

void M_Data_Receive_Prepare(uint8 data)
{
	static uint8 RxBuffer[50];
   
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
    
	temp[count] = data;
    count++;
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
	else if(state==3&&_data_len>0)//data
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
		M_Data_Receive_Anl(RxBuffer,_data_cnt+4);
        count = 0;
	}
	else
		state = 0;
    

}
void M_Data_Receive_Anl(uint8 *data_buf,uint8 num)
{
    uint8 sum = 0;
    uint8 temp = 0;
    
    uint8 i=0;
	for( i=1;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		                
    g_Car.M_value = *(data_buf+7)*10+*(data_buf+8);
    g_Car.M_radix = *(data_buf+9);
    
    g_Car.shutdown_flag = 1;

}
/* [] END OF FILE */

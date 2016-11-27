#include "Transfer.h"
#include "car.h"
#include "battery.h"
#include "Lidar.h"

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;					//需要发送数据的标志
uint8 data_to_send[50];     	//发送数据缓存


void DT_Data_Exchange(void)
{
    static float temp_pitch;
    static float tmep_roll;
    static float temp_yaw;
    
	static uint8 cnt = 0;
	static uint8 senser_cnt 	=   5;
	static uint8 status_cnt 	=   11;
	static uint8 distance_cnt 	=   13;
	static uint8 motopwm_cnt	=   20;
	static uint8 power_cnt		=	50;
	
	if((cnt % senser_cnt) == (senser_cnt-1))
		f.send_senser = 1;	
	
	if((cnt % status_cnt) == (status_cnt-1))
		f.send_status = 1;	
	
	if((cnt % distance_cnt) == (distance_cnt-1))
		f.send_distance = 1;	
	
	if((cnt % motopwm_cnt) == (motopwm_cnt-1))
		f.send_motopwm = 1;	
    
	if((cnt % power_cnt) == (power_cnt-1))
		f.send_power = 1;		
    
	cnt++;
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_version)
	{
		f.send_version = 0;
	  //DT_Send_Version(4,300,100,400,0);
	}
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_status)
	{
		f.send_status = 0;
    	DT_Send_Status(0,g_Car.set_dir,g_Car.set_car_speed,P_V);
//        if(rx_lidar_flag == 1)
//        {
//            DT_Send_Lidar_normal(net_nodeBuffer);
//            rx_lidar_flag = 0;
//        }
	}	
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_senser)
	{
		f.send_senser = 0;
        temp_pitch =  (S_IMU.pitch/32768.0*180);
        tmep_roll =   (S_IMU.roll/32768.0*180);
        temp_yaw   =  (S_IMU.yaw/32768.0*180);
      
		DT_Send_Senser(temp_pitch*1000,tmep_roll*1000,temp_yaw*1000,0,0,0);
	}	
///////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////// 
    else if(f.send_distance)
	{
	   f.send_distance = 0;
	   DT_Send_Distance(g_Car.distance_front,g_Car.distance_back);
	}
	else if(f.send_power)
	{
		f.send_power = 0;
		DT_Send_Power(C_V,D_V);
	}

///////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////

void DT_Send_Data(uint8 *dataToSend , uint8 length)
{
    UART_net_PutArray(dataToSend,length);
}

static void DT_Send_Check(uint8 head, uint8 check_sum)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	uint8 sum = 0;
    uint8 i=0;
	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	DT_Send_Data(data_to_send, 7);
}

void DT_Data_Receive_Prepare(uint8 data)
{
	static uint8 RxBuffer[50];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1) //fuciton
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)   //len
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)//data
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)//sum
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
    

}

void DT_Data_Receive_Anl(uint8 *data_buf,uint8 num)
{
	uint8 sum = 0;
    uint8 temp = 0;
    uint8 i=0;
	for( i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		                //判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
	
	if(*(data_buf+2)==0X01)
	{
   
	}
	
	if(*(data_buf+2)==0X02)
	{
        g_Car.set_dir = *(data_buf+4);
       
	}
   	if(*(data_buf+2)==0X03)
	{
        g_Car.set_car_speed  = (int16)(*(data_buf+4)<<8)|*(data_buf+5);
        if(g_Car.set_car_speed > 20000)
        {
            g_Car.set_car_speed = 0;
        }
    }
    if(*(data_buf+2)==0x04)
    {
        temp = 100-*(data_buf+4); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_WriteCompare1(temp*100); 
        
        temp = 100-*(data_buf+5); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_WriteCompare2(temp*100);
        
        temp = 100-*(data_buf+6); 
        if(temp>=100)
        {
            temp = 100;
        }
        if(temp<=0)
        {
            temp= 0;
        }
        PWM_LED_1_WriteCompare1(temp*100);
        
        temp = 100-*(data_buf+7); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_1_WriteCompare2(temp*100);
        //led5678
    }
	if(*(data_buf+2)==0X5)								
    {
       g_Car.set_ptz_dir = *(data_buf+4);
    } 
    
    if(*(data_buf+2)==0X6)								
    {	
      if(*(data_buf+4) == 1)
      {
           CHANGE_PWR_Write(1);
      }
      else
      {
           CHANGE_PWR_Write(0);
      }
    }
    if(*(data_buf+2)==0X9)								
    {
        if(*(data_buf+4) == 99)
        {
            g_Car.hearting = 1;
        }
    }
   
	if(*(data_buf+2)==0X13)								//PID4
	{
		DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
		DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		DT_Send_Check(*(data_buf+2),sum);
	}
}

void DT_Send_Version(uint8 hardware_type, uint16 hardware_ver,uint16 software_ver,uint16 protocol_ver,uint16 bootloader_ver)
{
	uint8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=hardware_type;
	data_to_send[_cnt++]=BYTE1(hardware_ver);
	data_to_send[_cnt++]=BYTE0(hardware_ver);
	data_to_send[_cnt++]=BYTE1(software_ver);
	data_to_send[_cnt++]=BYTE0(software_ver);
	data_to_send[_cnt++]=BYTE1(protocol_ver);
	data_to_send[_cnt++]=BYTE0(protocol_ver);
	data_to_send[_cnt++]=BYTE1(bootloader_ver);
	data_to_send[_cnt++]=BYTE0(bootloader_ver);
	
	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_Status(uint8 car_lock,enum CAR_DIR car_dir,uint16 car_speed,uint16 car_pull)
{
	uint8 _cnt=0;
    uint8 _temp;
	uint16 _temp1;

	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
    
	_temp = car_lock;
	data_to_send[_cnt++]=_temp;
	_temp = car_dir;
	data_to_send[_cnt++]=_temp;

	_temp1 =  car_speed;
	data_to_send[_cnt++]=BYTE1(_temp1);
	data_to_send[_cnt++]=BYTE0(_temp1);
	
    _temp1 =  car_pull;
	data_to_send[_cnt++]=BYTE1(_temp1);
	data_to_send[_cnt++]=BYTE0(_temp1);
	
	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_Senser(int32 pitch,int32 roll,int32 yaw,int32 a_x,int32 a_y,int32 a_z)
{
	uint8 _cnt=0;
	int32 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = pitch;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
    data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = roll;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
    data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
    _temp = yaw;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
    data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
        _temp = a_x;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
    data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
        _temp = a_y;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
    data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
        _temp = a_z;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
    data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_Distance(uint16 front_distance,uint16 back_distance)
{
	uint8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(front_distance);
	data_to_send[_cnt++]=BYTE0(front_distance);
    
	data_to_send[_cnt++]=BYTE1(back_distance);
	data_to_send[_cnt++]=BYTE0(back_distance);

	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_Power(uint16 C_votage, uint16 D_votage )
{
	uint8 _cnt=0;
	uint16 temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0;
	
	temp = C_votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	temp = D_votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;  
	DT_Send_Data(data_to_send, _cnt);
    
}
void DT_Send_MotoPWM(uint16 m_1,uint16 m_2,uint16 m_3,uint16 m_4,uint16 m_5,uint16 m_6,uint16 m_7,uint16 m_8)
{
	uint8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	
	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_PID(uint8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	uint8 _cnt=0;
	int16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = p1_p * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_Lidar_normal(uint8 * temp_nodebuf)
{
	uint8 _cnt=0;
	int16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x04;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=temp_nodebuf[0];
    data_to_send[_cnt++]=temp_nodebuf[2];
    data_to_send[_cnt++]=temp_nodebuf[1];
    data_to_send[_cnt++]=temp_nodebuf[4];
    data_to_send[_cnt++]=temp_nodebuf[3];
    
    data_to_send[_cnt++]=temp_nodebuf[5];
    data_to_send[_cnt++]=temp_nodebuf[7];
    data_to_send[_cnt++]=temp_nodebuf[6];
    data_to_send[_cnt++]=temp_nodebuf[9];
    data_to_send[_cnt++]=temp_nodebuf[8];
    
    data_to_send[_cnt++]=temp_nodebuf[10];
    data_to_send[_cnt++]=temp_nodebuf[12];
    data_to_send[_cnt++]=temp_nodebuf[11];
    data_to_send[_cnt++]=temp_nodebuf[14];
    data_to_send[_cnt++]=temp_nodebuf[13];
    
    data_to_send[_cnt++]=temp_nodebuf[15];
    data_to_send[_cnt++]=temp_nodebuf[17];
    data_to_send[_cnt++]=temp_nodebuf[16];
    data_to_send[_cnt++]=temp_nodebuf[19];
    data_to_send[_cnt++]=temp_nodebuf[18];
    
    data_to_send[_cnt++]=temp_nodebuf[20];
    data_to_send[_cnt++]=temp_nodebuf[22];
    data_to_send[_cnt++]=temp_nodebuf[21];
    data_to_send[_cnt++]=temp_nodebuf[24];
    data_to_send[_cnt++]=temp_nodebuf[23];
    
    

	data_to_send[3] = _cnt-4;
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	DT_Send_Data(data_to_send, _cnt);
}
void DT_Send_Lidar(uint8 * temp_nodebuf)
{
	uint8 _cnt=0;
	int16 _temp;
	
	temp_nodebuf[0]=0xAA;
	temp_nodebuf[1]=0xAA;
	temp_nodebuf[2]=0x04;
	temp_nodebuf[3]=0;
    
 
	
	uint8 sum = 0;
    uint8 i=0;
	for( i=0;i<85;i++)
		sum += temp_nodebuf[i];
	
	temp_nodebuf[86]=sum;

	DT_Send_Data(temp_nodebuf, 87);
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

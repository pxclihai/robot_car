#include "Transfer.h"
#include "car.h"
#include "battery.h"
#include "Lidar.h"
/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;					//需要发送数据的标志
uint8 data_to_send[50];	//发送数据缓存

/////////////////////////////////////////////////////////////////////////////////////
//Data_Exchange函数处理各种数据发送请求，比如想实现每5ms发送一次传感器数据至上位机，即在此函数内实现
//此函数应由用户每1ms调用一次
void DT_Data_Exchange(void)
{
    static float temp_pitch;
    static float tmep_roll;
    static float temp_yaw;
    
	static uint8 cnt = 0;
	static uint8 senser_cnt 	=   10;
	static uint8 status_cnt 	=   11;
	static uint8 rcdata_cnt 	=   20;
	static uint8 motopwm_cnt	=   20;
	static uint8 power_cnt		=	50;
	
	if((cnt % senser_cnt) == (senser_cnt-1))
		f.send_senser = 1;	
	
	if((cnt % status_cnt) == (status_cnt-1))
		f.send_status = 1;	
	
	if((cnt % rcdata_cnt) == (rcdata_cnt-1))
		f.send_rcdata = 1;	
	
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
    	DT_Send_Status(0,g_Car.set_dir,g_Car.set_car_speed);
//        if(rx_lidar_flag == 1)
//        {
//            DT_Send_Lidar_normal(net_nodeBuffer);
//            rx_lidar_flag = 0;
//        }
	}	
/////////////////////////////////////////////////////////////////////////////////////
    if(f.send_senser)
	{
		f.send_senser = 0;
        
//        S_IMU.acc_x = S_IMU.acc_x/32768.0*16;
//        a[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*16;
//        a[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*16;
//   
//      
//        w[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*2000;
//        w[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*2000;
//        w[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*2000;
     
       
        temp_pitch =  (S_IMU.pitch/32768.0*180);
        tmep_roll =   (S_IMU.roll/32768.0*180);
        temp_yaw   =  (S_IMU.yaw/32768.0*180);
      
		DT_Send_Senser(temp_pitch*1000,tmep_roll*1000,temp_yaw*1000,0,0,0);
	}	
///////////////////////////////////////////////////////////////////////////////////////
//	else if(f.send_rcdata)
//	{
//		f.send_rcdata = 0;
//		DT_Send_RCData(Rc_Pwm_In[0],Rc_Pwm_In[1],Rc_Pwm_In[2],Rc_Pwm_In[3],Rc_Pwm_In[4],Rc_Pwm_In[5],Rc_Pwm_In[6],Rc_Pwm_In[7],0,0);
//	}	
/////////////////////////////////////////////////////////////////////////////////////	
//	else if(f.send_motopwm)
//	{
//		f.send_motopwm = 0;
//		DT_Send_MotoPWM(1,2,3,4,5,6,7,8);
//	}	
///////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_power)
	{
		f.send_power = 0;
		DT_Send_Power(C_V,D_V);
	}
///////////////////////////////////////////////////////////////////////////////////////
//	else if(f.send_pid1)
//	{
//		f.send_pid1 = 0;
//		DT_Send_PID(1,ctrl_1.PID[PIDROLL].kp,ctrl_1.PID[PIDROLL].ki,ctrl_1.PID[PIDROLL].kd,
//											ctrl_1.PID[PIDPITCH].kp,ctrl_1.PID[PIDPITCH].ki,ctrl_1.PID[PIDPITCH].kd,
//											ctrl_1.PID[PIDYAW].kp,ctrl_1.PID[PIDYAW].ki,ctrl_1.PID[PIDYAW].kd);
//	}	
///////////////////////////////////////////////////////////////////////////////////////
//	else if(f.send_pid2)
//	{
//		f.send_pid2 = 0;
//		DT_Send_PID(2,ctrl_1.PID[PID4].kp,ctrl_1.PID[PID4].ki,ctrl_1.PID[PID4].kd,
//											ctrl_1.PID[PID5].kp,ctrl_1.PID[PID5].ki,ctrl_1.PID[PID5].kd,
//											ctrl_1.PID[PID6].kp,ctrl_1.PID[PID6].ki,ctrl_1.PID[PID6].kd);
//	}
///////////////////////////////////////////////////////////////////////////////////////
//	else if(f.send_pid3)
//	{
//		f.send_pid3 = 0;
//		DT_Send_PID(3,ctrl_2.PID[PIDROLL].kp,ctrl_2.PID[PIDROLL].ki,ctrl_2.PID[PIDROLL].kd,
//											ctrl_2.PID[PIDPITCH].kp,ctrl_2.PID[PIDPITCH].ki,ctrl_2.PID[PIDPITCH].kd,
//											ctrl_2.PID[PIDYAW].kp,ctrl_2.PID[PIDYAW].ki,ctrl_2.PID[PIDYAW].kd);
//	}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//	Usb_Hid_Send();					
/////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////
//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
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

/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare函数是协议预解析，根据协议的格式，将收到的数据进行一次格式性解析，格式正确的话再进行数据解析
//移植时，此函数应由用户根据自身使用的通信方式自行调用，比如串口每收到一字节数据，则调用此函数一次
//此函数解析出符合格式的数据帧后，会自行调用数据解析函数
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
	else if(state==3&&data<50)//len
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
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
//校验通过后对数据进行解析，实现相应功能
//此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用
void DT_Data_Receive_Anl(uint8 *data_buf,uint8 num)
{
	uint8 sum = 0;
    uint8 temp = 0;
    uint8 i=0;
	for( i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
	
	if(*(data_buf+2)==0X01)
	{
//		if(*(data_buf+4)==0X01)
//			mpu6050.Acc_CALIBRATE = 1;
//		if(*(data_buf+4)==0X02)
//			mpu6050.Gyro_CALIBRATE = 1;
//		if(*(data_buf+4)==0X03)
//		{
//			mpu6050.Acc_CALIBRATE = 1;		
//			mpu6050.Gyro_CALIBRATE = 1;			
//		}
	}
	
	if(*(data_buf+2)==0X02)
	{
        g_Car.set_dir = *(data_buf+4);
       
	}
   	if(*(data_buf+2)==0X03)
	{
        g_Car.set_car_speed += (int16)(*(data_buf+4)<<8)|*(data_buf+5);
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
        PWM_LED_WriteCompare1(temp*100); 
     
         temp = 100-*(data_buf+6); 
        if(temp>=100)
        {
            temp = 100;
        }
        if(temp<=0)
        {
            temp= 0;
        }
        PWM_LED_WriteCompare2(temp*100);
        
        temp = 100-*(data_buf+7); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_WriteCompare2(temp*100);
        //led5678
       temp = 100-*(data_buf+8); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_1_WriteCompare1(temp*100);
        
        temp = 100-*(data_buf+9); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_1_WriteCompare1(temp*100);
        
         temp = 100-*(data_buf+10); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_1_WriteCompare2(temp*100);
        
             temp = 100-*(data_buf+11); 
        if(temp>=100)
        {
            temp =100;
        }
        if(temp<=0)
        {
            temp=0;
        }
        PWM_LED_1_WriteCompare2(temp*100);
//        else
//        {
//            LED1_Write(0);
//        }
//        if(((*(data_buf+4))&0x02) == 0x02)
//        {
//            LED2_Write(1);
//        }
//        else
//        {
//            LED1_Write(0);
//        }
//        if(((*(data_buf+4))&0x04) == 0x04)
//        {
//            LED3_Write(1);
//        }
//        if(((*(data_buf+4))&0x08) == 0x08)
//        {
//            LED4_Write(1);
//        }
    }
	if(*(data_buf+2)==0X10)								//PID1
    {
//        ctrl_1.PID[PIDROLL].kp  = 0.001*( (int16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_1.PID[PIDROLL].ki  = 0.001*( (int16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_1.PID[PIDROLL].kd  = 0.001*( (int16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_1.PID[PIDPITCH].kp = 0.001*( (int16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_1.PID[PIDPITCH].ki = 0.001*( (int16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_1.PID[PIDPITCH].kd = 0.001*( (int16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_1.PID[PIDYAW].kp 	= 0.001*( (int16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_1.PID[PIDYAW].ki 	= 0.001*( (int16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_1.PID[PIDYAW].kd 	= 0.001*( (int16)(*(data_buf+20)<<8)|*(data_buf+21) );
        DT_Send_Check(*(data_buf+2),sum);
//				Param_SavePID();
    }
    if(*(data_buf+2)==0X11)								//PID2
    {
//        ctrl_1.PID[PID4].kp 	= 0.001*( (int16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_1.PID[PID4].ki 	= 0.001*( (int16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_1.PID[PID4].kd 	= 0.001*( (int16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_1.PID[PID5].kp 	= 0.001*( (int16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_1.PID[PID5].ki 	= 0.001*( (int16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_1.PID[PID5].kd 	= 0.001*( (int16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_1.PID[PID6].kp	  = 0.001*( (int16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_1.PID[PID6].ki 	= 0.001*( (int16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_1.PID[PID6].kd 	= 0.001*( (int16)(*(data_buf+20)<<8)|*(data_buf+21) );
        DT_Send_Check(*(data_buf+2),sum);
		//		Param_SavePID();
    }
    if(*(data_buf+2)==0X12)								//PID3
    {	
//        ctrl_2.PID[PIDROLL].kp  = 0.001*( (int16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_2.PID[PIDROLL].ki  = 0.001*( (int16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_2.PID[PIDROLL].kd  = 0.001*( (int16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_2.PID[PIDPITCH].kp = 0.001*( (int16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_2.PID[PIDPITCH].ki = 0.001*( (int16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_2.PID[PIDPITCH].kd = 0.001*( (int16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_2.PID[PIDYAW].kp 	= 0.001*( (int16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_2.PID[PIDYAW].ki 	= 0.001*( (int16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_2.PID[PIDYAW].kd 	= 0.001*( (int16)(*(data_buf+20)<<8)|*(data_buf+21) );
        DT_Send_Check(*(data_buf+2),sum);
		//		Param_SavePID();
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
void DT_Send_Status(uint8 car_lock,enum CAR_DIR car_dir,uint16 car_speed)
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
void DT_Send_RCData(uint16 thr,uint16 yaw,uint16 rol,uint16 pit,uint16 aux1,uint16 aux2,uint16 aux3,uint16 aux4,uint16 aux5,uint16 aux6)
{
	uint8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

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

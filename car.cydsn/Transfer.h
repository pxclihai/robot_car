#ifndef _DATA_TRANSFER_H
#define	_DATA_TRANSFER_H


#include <project.h>
#include "car.h"
typedef struct 
{
		uint8 send_version;
		uint8 send_status;
		uint8 send_senser;
		uint8 send_pid1;
		uint8 send_pid2;
		uint8 send_pid3;
		uint8 send_pid4;
		uint8 send_pid5;
		uint8 send_pid6;
		uint8 send_distance;
		uint8 send_offset;
		uint8 send_motopwm;
		uint8 send_power;

}dt_flag_t;
typedef struct 
{
    int16 acc_x;
    int16 acc_y;
    int16 acc_z;
    int16 gyr_x;
    int16 gyr_y;
    int16 gyr_z;
    int16 pitch;
    int16 roll;
    int16 yaw;
    
    int16 T;
}IMU;
IMU S_IMU;
extern dt_flag_t f;

void DT_Data_Exchange(void);
void DT_Data_Receive_Prepare(uint8 data);
void DT_Data_Receive_Anl(uint8 *data_buf,uint8 num);
void DT_Send_Version(uint8 hardware_type, uint16 hardware_ver,uint16 software_ver,uint16 protocol_ver,uint16 bootloader_ver);
void DT_Send_Status(uint8 car_lock,enum CAR_DIR car_dir,uint16 car_speed,uint16 car_pull);
void DT_Send_Senser(int32 pitch,int32 roll,int32 yaw,int32 a_x,int32 a_y,int32 a_z);
void DT_Send_RCData(uint16 thr,uint16 yaw,uint16 rol,uint16 pit,uint16 aux1,uint16 aux2,uint16 aux3,uint16 aux4,uint16 aux5,uint16 aux6);
void DT_Send_Power(uint16 votage, uint16 current);
void DT_Send_MotoPWM(uint16 m_1,uint16 m_2,uint16 m_3,uint16 m_4,uint16 m_5,uint16 m_6,uint16 m_7,uint16 m_8);
void DT_Send_PID(uint8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);
void DT_Send_Lidar(uint8 * nodebuf);
void DT_Send_Lidar_normal(uint8 * temp_nodebuf);
void DT_Send_Distance(uint16 front_distance,uint16 back_distance);
#endif


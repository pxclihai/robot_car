///* ========================================
// *
// * Copyright YOUR COMPANY, THE YEAR
// * All Rights Reserved
// * UNPUBLISHED, LICENSED SOFTWARE.
// *
// * CONFIDENTIAL AND PROPRIETARY INFORMATION
// * WHICH IS THE PROPERTY OF your company.
// *
// * ========================================
//*/
//
//#include "wave.h"
//#include "car.h"
//uint16 distance;
//static uint8 write_flag=0;
//void K103_write(uint16 addr,uint8 reg,uint8 Command)
//{
//    uint8 status = 0;
//	uint8 Addr = addr;
//
//	
//    status = WAVE_MasterSendStart(Addr,0);
//    if(!status)
//    {
//      
//            status = WAVE_MasterWriteByte(reg);//写输出端口
//            if(!status)
//            {
//              status = WAVE_MasterWriteByte(Command);//写输出端口
//              if(!status)
//              {
//              
//              }
//            }
//    }
//    status = WAVE_MasterSendStop();
//}
//uint8 K103_read(uint16 addr,uint8 reg)
//{
//
//	uint8 dat = 0,status = 0;
//    
//    status = WAVE_MasterSendStart(addr,0);
//    if(!status)
//    {
//       
//            status = WAVE_MasterWriteByte(reg);//写输出端口
//               if(status)
//              {
//                 
//              }
//    }
//    status = WAVE_MasterSendRestart(addr,1);
//    CyDelayUs(10);
//    if(!status)
//    {
//       dat = WAVE_MasterReadByte(0);//有ACK
//       if(status)
//        {
//            return 0;
//        }
//    }
//	status = WAVE_MasterSendStop();
//	return dat;
//
//}
//
//void K103_read_distance(void)
//{
//        if(write_flag == 0)
//        {
//          K103_write(0x68,2,0xb4);
//          K103_write(0xf4,2,0xb4);
//          write_flag = 1;
//        }
//        else
//        {
//         distance  = K103_read(0x68,2);
//         distance <<= 8;
//         distance += K103_read(0x68,3);
//         g_Car.distance_back = distance;
//         distance  =K103_read(0xf4,2);
//         distance <<= 8;
//         distance += K103_read(0xf4,3);
//         g_Car.distance_front = distance;
//         write_flag = 0 ;
//        }
//}
///* [] END OF FILE */
//
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
#include "Lidar.h"
#include "uart_net.h"
#include "UART_LIDAR.h"
#include "Transfer.h"
#include <stdio.h>
uint8 nodeBuffer[100];
uint8 net_nodeBuffer[100];
uint8 arrtest[5] = {0x01,0x02,0x03,0x04,0x05};
uint8 EXPRESS_SCAN[9] = {0xA5,0x82,0x05,00,00,00,00,00,0x22};
uint8 NORMAL_SCAN[2] =  {0xA5,0x20};
uint8 GET_HEALTH[2] =  {0xA5,0x52};
uint8  _is_previous_capsuledataRdy = true;
uint8 rx_lidar_flag =0;
uint8 rx_lidar_count = 0;
rplidar_response_capsule_measurement_nodes_t *node;
uint8 start;
uint8 end;
uint8 normal_size ;
uint8 output_t;
rplidar_response_measurement_node_t test1[500];
rplidar_response_measurement_node_t test2[500];
rplidar_response_measurement_node_t test3[500];
float test_f_suzu[500];
static uint8 test_i = 0;
static uint16 count;
uint8 ascendScanData(rplidar_response_measurement_node_t * nodebuffer, size_t count);
void Lidar_init()
{
    start = offsetof(rplidar_response_capsule_measurement_nodes_t, start_angle_sync_q6);
    end   =sizeof(rplidar_response_capsule_measurement_nodes_t);
    normal_size = sizeof(rplidar_response_measurement_node_t);
   // UART_LIDAR_PutArray(GET_HEALTH,sizeof(GET_HEALTH));
    
//    UART_LIDAR_PutArray(NORMAL_SCAN,sizeof(NORMAL_SCAN));
    
}
int  recvPos = 0;
#define RPLIDAR_RESP_MEASUREMENT_SYNCBIT        (0x1<<0)
#define RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT  2
#define RPLIDAR_RESP_MEASUREMENT_CHECKBIT       (0x1<<0)
#define RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT    1
rplidar_response_measurement_node_t nodeBuffer1;
void Lidar_Normal_Data_Receive_Prepare_4(uint8 data)
{
    uint8 tmp;
    uint16 test;
    float test_f;
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
    
    tmp  = (data>>1);
  
	if(state==0&&(tmp ^ data) & 0x1)  //sys1
	{
        debug_time_Write(1);
		state=1;
		nodeBuffer[0]=data;
       //debug_time_Write(1);
	}
    else if(state == 1 && ((data&0x1) == 1)) //
    {
        test = (tmp&0x1);
        state=2;
		nodeBuffer[1]= data;
        _data_cnt = 0;
        _data_len = normal_size-2;
    }
	else if(state== 2 && _data_len>0) //
	{
		_data_len--;
		nodeBuffer[2+_data_cnt++] = data;
   
		if(_data_len == 0)
        {
            count++;
            if((nodeBuffer[0] & 0x1) ==1)
            {
                 count =0;
            }
//            test = ((nodeBuffer[2]<<8)|(nodeBuffer[1]))>>1;
//            test_f = ((float)test)/64.0f;
//            test_f_suzu[test_i] = test_f; 
//            if(test_i == 200)
//            {
//                test_i =0;
//                rx_lidar_flag = 1;
//            } 
       
            // if(rx_lidar_flag == 0)
            //  {
              
//           test1[test_i].angle_q6_checkbit = (((nodeBuffer[2]<<8)|(nodeBuffer[1])));
//           test1[test_i].sync_quality      =  nodeBuffer[0];
//           test1[test_i].distance_q2       = ((nodeBuffer[4]<<8)|(nodeBuffer[3]));               
//           printf("%d\n   ",test);
// 
              memcpy(net_nodeBuffer+test_i*5,nodeBuffer,5);
              test_i ++;
              if(test_i == 5)
              {  
                
                   rx_lidar_flag = 1; 
                 
                   test_i = 0;
              }
              //   
                

            // }
         
            
            state = 0;
        }
	}  
  
 //                      if(output_t == 0)
//                   {
//                   debug_time_Write(1);
//                   output_t++;
//                   }
//                   else
//                  {
//                   debug_time_Write(0);
//                    output_t = 0;
//                  }
    else
    state = 0;
     
}
//void Lidar_Normal_pc(uint8 data)
//{
//       uint8 currentByte = data;
//    uint8 *nodeBuffer_p ;
//     nodeBuffer_p = &nodeBuffer1;
//      for (size_t pos = 0; pos < 1; ++pos)
//      {
//            switch (recvPos) {
//            case 0: // expect the sync bit and its reverse in this byte
//                {
//                    uint8 tmp = (currentByte>>1);
//                    if ( (tmp ^ currentByte) & 0x1 ) {
//                        // pass
//                    } else {
//                        continue;
//                    }
//
//                }
//                break;
//            case 1: // expect the highest bit to be 1
//                {
//                    if (currentByte & RPLIDAR_RESP_MEASUREMENT_CHECKBIT) {
//                        // pass
//                    } else {
//                        recvPos = 0;
//                        continue;
//                    }
//                }
//                break;
//            }
//            nodeBuffer_p[recvPos++] = currentByte;
//
//            if (recvPos == sizeof(rplidar_response_measurement_node_t)) {
//               // return 1;
//            }
//    }
//}




static int count_time_1ms = 0;
void Lidar_Normal_Data_Receive_Prepare(uint8 data)
{
    uint8 tmp;
    uint16 test;
    float test_f;
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
    
    tmp  = (data>>1);
  
	if(state==0&&(tmp ^ data) & 0x1)  //sys1
	{
		state=1;
		nodeBuffer[0]=data;
       //debug_time_Write(1);
	}
    else if(state == 1 && ((data&0x1) == 1)) //
    {
        test = (tmp&0x1);
        state=2;
		nodeBuffer[1]= data;
        _data_cnt = 0;
        _data_len = normal_size-2;
    }
	else if(state== 2 && _data_len>0) //
	{
		_data_len--;
		nodeBuffer[2+_data_cnt++] = data;
   
		if(_data_len == 0)
        {
          
//            if((nodeBuffer[0] & 0x1) ==1)
//            {
//                 test_i =0;
//            }
//            test = ((nodeBuffer[2]<<8)|(nodeBuffer[1]))>>1;
//            test_f = ((float)test)/64.0f;
//            test_f_suzu[test_i] = test_f; 
//            if(test_i == 200)
//            {
//                test_i =0;
//                rx_lidar_flag = 1;
//            } 
          
            if(rx_lidar_flag == 0)
            {
              
//           test1[test_i].angle_q6_checkbit = (((nodeBuffer[2]<<8)|(nodeBuffer[1])));
//           test1[test_i].sync_quality      =  nodeBuffer[0];
//           test1[test_i].distance_q2       = ((nodeBuffer[4]<<8)|(nodeBuffer[3]));               
          //      printf("%d\n   ",test);
                 //memcpy(net_nodeBuffer,nodeBuffer,5);
              //  if(test_i < 100)
              //  { 
                   test_i ++;
                   debug_time_Write(1);
                   DT_Send_Lidar_normal(nodeBuffer); 
                   debug_time_Write(0);
            //    }
              //   rx_lidar_flag = 1; 
                

             }
            
            state = 0;
        }
	}  
  
 //                      if(output_t == 0)
//                   {
//                   debug_time_Write(1);
//                   output_t++;
//                   }
//                   else
//                  {
//                   debug_time_Write(0);
//                    output_t = 0;
//                  }
    else
    state = 0;
     
}
void Lidar_Normal_Data_Receive_Prepare_2(uint8 data)
{
    uint8 tmp;
    uint16 test;
    float test_f;
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
    
     tmp  = (data>>1);
    
	if(state==0&&(tmp ^ data) & 0x1)  //sys1
	{
		state=1;
		nodeBuffer[0]=data;
      
	}
    else if(state == 1 && ((data&0x1) == 1)) //
    {
        test = (tmp&0x1);
        state=2;
		nodeBuffer[1]= data;
        _data_cnt = 0;
        _data_len = normal_size-2;
    }
	else if(state== 2 && _data_len>0) //
	{
		_data_len--;
		nodeBuffer[2+_data_cnt++] = data;
        state = 3;
	} 
    else if(state == 3 && (tmp ^ data) & 0x1)  //sys1
	{
		state = 4;
		nodeBuffer[5]=data;
      
	}
    else if(state == 4 && ((data&0x1) == 1)) //
    {
        test = (tmp&0x1);
        state=5;
		nodeBuffer[6]= data;
        _data_cnt = 0;
        _data_len = normal_size-2;
    }
	else if(state== 5 && _data_len>0) //
	{
		_data_len--;
		nodeBuffer[7+_data_cnt++] = data;
   
		if(_data_len == 0)
        {
            if(rx_lidar_flag == 0)
            {		  
                  if(rx_lidar_flag == 0)
                  {
                   memcpy(net_nodeBuffer,nodeBuffer,10); 
                   rx_lidar_flag = 1;
                   
                   if(output_t == 0)
                   {
                   debug_time_Write(1);
                   output_t++;
                   }
                   else
                  {
                    debug_time_Write(0);
                    output_t = 0;
                  }
                   
                  }
            }
            state = 0;
        }
	} 

    else
    state = 0;
}
void scan_lidar()
{
    uint16 scan_count = 0;
    uint8 pos;
    uint8 i;
    uint8 tttt;
    if(rx_lidar_flag == 1)
    {
          for ( pos = 0; pos < 200; ++pos)
        {
            if (test1[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT)
            {
                // only publish the data when it contains a full 360 degree scan 
                
                if ((test2[0].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT)) {
                    
		
                  //  memcpy(test3, test1, scan_count*sizeof(rplidar_response_measurement_node_t));
                    //_cached_scan_node_count = 128;
                  //  _dataEvt.set();
                  //  _lock.unlock();
                  //ascendScanData(test2,scan_count);
                    for(i = 0;i<scan_count;i++)
                    {
                    test_f_suzu[i]= (test2[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
                    tttt =1;
                    }
                }
                scan_count = 0;
            }
            test2[scan_count++] = test1[pos];
//			test[scan_count++].angle_q6_checkbit = (local_scan[0].angle_q6_checkbit >>1)/64.0f;
            if (scan_count >200) scan_count-=1; // prevent overflow
        }
    }
    rx_lidar_flag = 0;
}
uint8 ascendScanData(rplidar_response_measurement_node_t * nodebuffer, size_t count)
{
    float inc_origin_angle = 360.0/count;
    size_t i = 0;
    size_t j;
    //Tune head
    for (i = 0; i < count; i++) {
        if(nodebuffer[i].distance_q2 == 0) {
            continue;
        } else {
            while(i != 0) {
                i--;
                float expect_angle = (nodebuffer[i+1].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f - inc_origin_angle;
                if (expect_angle < 0.0f) expect_angle = 0.0f;
                uint16 checkbit = nodebuffer[i].angle_q6_checkbit & RPLIDAR_RESP_MEASUREMENT_CHECKBIT;
                nodebuffer[i].angle_q6_checkbit = (((uint16)(expect_angle * 64.0f)) << RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) + checkbit;
            }
            break;
        }
    }

    // all the data is invalid
    if (i == count) return 2;

    //Tune tail
    for (i = count - 1; i > 0; i--) {
        if(nodebuffer[i].distance_q2 == 0) {
            continue;
        } else {
            while(i != (count - 1)) {
                i++;
                float expect_angle = (nodebuffer[i-1].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f + inc_origin_angle;
                if (expect_angle > 360.0f) expect_angle -= 360.0f;
                uint16 checkbit = nodebuffer[i].angle_q6_checkbit & RPLIDAR_RESP_MEASUREMENT_CHECKBIT;
                nodebuffer[i].angle_q6_checkbit = (((uint16)(expect_angle * 64.0f)) << RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) + checkbit;
            }
            break;
        }
    }

    //Fill invalid angle in the scan
    float frontAngle = (nodebuffer[0].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
    for (i = 1; i < count; i++) {
        if(nodebuffer[i].distance_q2 == 0) {
            float expect_angle =  frontAngle + i * inc_origin_angle;
            if (expect_angle > 360.0f) expect_angle -= 360.0f;
            uint16 checkbit = nodebuffer[i].angle_q6_checkbit & RPLIDAR_RESP_MEASUREMENT_CHECKBIT;
            nodebuffer[i].angle_q6_checkbit = (((uint16)(expect_angle * 64.0f)) << RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) + checkbit;
        }
    }

    // Reorder the scan according to the angle value
    for (i = 0; i < (count-1); i++){
        for (j = (i+1); j < count; j++){
            if(nodebuffer[i].angle_q6_checkbit > nodebuffer[j].angle_q6_checkbit){
                rplidar_response_measurement_node_t temp = nodebuffer[i];
                nodebuffer[i] = nodebuffer[j];
                nodebuffer[j] = temp;
            }
        }
    }

    return 1;
}

void Lidar_Data_Receive_Prepare(uint8 data)
{
 
    uint8 temp_sys;
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
    temp_sys= offsetof(rplidar_response_capsule_measurement_nodes_t, start_angle_sync_q6);
	temp_sys = data>>4;
    
	if(state==0&&temp_sys==0xA)  //sys1
	{
		state=1;
		nodeBuffer[0]=data;
	}

	else if(state==1&&temp_sys==0x5) //sys2
	{
		state=2;
		nodeBuffer[1]=data>>1;
        _data_cnt = 0;
        _data_len = sizeof(rplidar_response_capsule_measurement_nodes_t);
	}
	else if(state == 2&&_data_len>0)//data
	{
		_data_len--;
		nodeBuffer[2+_data_cnt++] = data;
		if(_data_len == 0)
		state = 3;

	}
	else if(state == 3)//sum
	{
     //  UART_net_PutChar(0);
		state = 0;
        size_t cpos; // = offsetof(rplidar_response_capsule_measurement_nodes_t, start_angle_sync_q6);
        
        uint8 checksum = 0;
        node = (rplidar_response_capsule_measurement_nodes_t *)nodeBuffer;
        
        uint8 recvChecksum = ((node->s_checksum_1 & 0xF) | (node->s_checksum_2<<4)); //取得较验位
        for ( cpos = 2 ;cpos < end; ++cpos)
            {
                checksum ^= nodeBuffer[cpos];
               
            }
            if (recvChecksum == checksum)
            {
                
                if (node->start_angle_sync_q6 & RPLIDAR_RESP_MEASUREMENT_EXP_SYNCBIT) 
                {
                    // this is the first capsule frame in logic, discard the previous cached data...
                //    _is_previous_capsuledataRdy = false;
                    
                }
                if(rx_lidar_flag == 0)
                {
                 memcpy(net_nodeBuffer,nodeBuffer,84);
                }
                 rx_lidar_flag = 1;
                 rx_lidar_count = cpos;
               //UART_net_PutChar('A');
            }
	}
	else
		state = 0;
}

/* [] END OF FILE */

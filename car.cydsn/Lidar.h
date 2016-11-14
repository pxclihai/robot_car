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
#ifndef _LIDAR_H
#define	_LIDAR_H

#include <project.h>
    
#define false 1
#define true  0;
    
#define RPLIDAR_RESP_MEASUREMENT_EXP_SYNCBIT              (0x1<<15)
extern uint8 nodeBuffer[100];
extern uint8 net_nodeBuffer[100];
extern uint8 rx_lidar_flag ;
extern uint8 rx_lidar_count;
typedef struct _rplidar_response_measurement_node_t {
    uint8    sync_quality;      // syncbit:1;syncbit_inverse:1;quality:6;
    uint16   angle_q6_checkbit; // check_bit:1;angle_q6:15;
    uint16   distance_q2;
} __attribute__((packed)) rplidar_response_measurement_node_t;
typedef struct _rplidar_response_cabin_nodes_t {
    uint16   distance_angle_1; // see [distance_sync flags]
    uint16   distance_angle_2; // see [distance_sync flags]
    uint8    offset_angles_q3;  
} __attribute__((packed)) rplidar_response_cabin_nodes_t;   
typedef struct _rplidar_response_capsule_measurement_nodes_t {
    uint8                             s_checksum_1; // see [s_checksum_1]
    uint8                             s_checksum_2; // see [s_checksum_1]
    uint16                            start_angle_sync_q6;
    rplidar_response_cabin_nodes_t  cabins[16];
} __attribute__((packed)) rplidar_response_capsule_measurement_nodes_t;

void Lidar_init();
void Lidar_Normal_pc(uint8 data) ;
void Lidar_Data_Receive_Prepare(uint8 data)  ;  
void Lidar_Normal_Data_Receive_Prepare(uint8 data);
void Lidar_Normal_Data_Receive_Prepare_2(uint8 data);
void scan_lidar();
#endif
/* [] END OF FILE */

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

#ifndef _M_THICKNESS_H_
#define _M_THICKNESS_H_
 void Control_M_loop();
void M_Data_Receive_Prepare(uint8 data);
void shutdown_50s();
#define M_NONE      0
#define M_POWER_ON  1
#define M_SHUT_DOWN 2
#define M_UP        3
#define M_DOWN      4
#define M_ZERO      5
#endif
/* [] END OF FILE */

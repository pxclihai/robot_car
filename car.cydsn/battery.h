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
#ifndef _BATTERY_H
#define	_BATTERY_H

#include <project.h>
    
#define SampleNum  100
#define C_CHANANL  0  
#define D_CHANANL  1
#define P_CHANANL  2
    
int16 C_V;
int16 D_V;
int16 P_V;
int16 Get_Battery_ADvalue(uint8 channal);
void Monitor_Battery_Init();

#endif
/* [] END OF FILE */

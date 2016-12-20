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
#include "systemInfo.h"
#include "car.h"
#include "led.h"
uint16  time_count;
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
void SystemInfo_Init()
{
    uint16 temp = 0x00;
   // temp = EEPROM_Array_Read(&g_Car.systeminfo,sizeof(g_Car.systeminfo));
    g_Car.systeminfo.totol_run_count = 0;
    g_Car.systeminfo.totol_run_count = (uint16)(EEPROM_ReadByte(1)<<8|EEPROM_ReadByte(2)); 
    g_Car.systeminfo.totol_run_count++;
    EEPROM_WriteByte(g_Car.systeminfo.totol_run_count>>8,1);
    EEPROM_WriteByte(g_Car.systeminfo.totol_run_count,2); 
}
void Cal_time()
{
   g_Car.systeminfo.cur_run_time++;
   time_count++;
   if(g_Car.set_dir != STOP)
   {
      g_Car.systeminfo.car_runing_time ++;
   }
}
void EEPROM_Array_Read(uint8 *buf,uint16 len)
{
	reg8 * RegPointer;
	uint16 i;
    
	for(i = 0; i < len; i++)
	{
		buf[i] = EEPROM_ReadByte(i);
	}
}
void System_check()
{
}
/* [] END OF FILE */

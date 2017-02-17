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
#include "battery.h"




int16 retADCValue1[SampleNum] = {0};
int16 retADCValue2[SampleNum] = {0};

void Monitor_Battery_Init()
{
    ADC_Start();
    ADC_StartConvert();
    

}
//int16 Get_C_Battery_ADvalue(void)
//{
//
//    uint8 i = 0;
//    int16 retval = 0;
//    int32 sum = 0;
//    for(i = 0;i < SampleNum;i++)
//    {
//        retADCValue2[i] = C_Battery_GetResult16();
//    
//    }
//
//    for(i = 0;i < SampleNum;i++)
//    {
//        sum += retADCValue2[i];
//    }
//    retval = sum/SampleNum;
//    return retval;
//    
//}
int16 Get_Battery_ADvalue(uint8 channal)
{

    uint8 i = 0;
    int16 retval = 0;
    int32 sum = 0;
    for(i = 0;i < SampleNum;i++)
    {
        retADCValue2[i] = ADC_GetResult16(channal);
    
    }

    for(i = 0;i < SampleNum;i++)
    {
        sum += retADCValue2[i];
    }
    retval = sum/SampleNum;
    return retval;
    
}

void Cal_Battery_loop()
{
    C_V = Get_Battery_ADvalue(C_CHANANL);
    D_V = Get_Battery_ADvalue(D_CHANANL);
    P_V = Get_Battery_ADvalue(P_CHANANL);   
}
///* [] END OF FILE */
//
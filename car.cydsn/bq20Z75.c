#include <stdio.h>
#include "i2c_core.h"
#include "bq20Z75.h"
uint8_t GetCrc8(uint8_t chkSum, uint8_t crcData);
#define POLYNOME 0x07
uint8_t GetCrc8(uint8_t chkSum, uint8_t crcData)
{
    uint8_t j = 8; // Counter for 8 shifts
    chkSum ^= crcData; // Initial XOR
    do
    {
        if (!(chkSum& 0x80)) // Check MSB
        {
        chkSum = chkSum<< 1; // If MSB = 0, shift left
        }
        else
        {
        chkSum = (chkSum<< 1) ^ POLYNOME; // If MSB = 1, shift and XOR
        }
    }while (--j); // Continue for 8 bitsreturnchkSum; // Return final value
    return chkSum;
}
int bq20Z75_read_in( unsigned char addr )
{
		unsigned char I2CData[2];
		int temp;
		int size;
		
		size = I2C_Read( addr, 6, I2CData, 2 );
		
		if( size != 2 )
		{
			//	printf("mlx90614 read failed!\r\n");
				return -1;
		}
		temp = (I2CData[1] << 8 | I2CData[0])*2;
		temp -= 27315;
	//    UART_net_PutArray(I2CData,2);
		return temp;
        
}

 uint8 PEC;
int bq20Z75_read_out( unsigned char addr ,unsigned char command )
{
    
		unsigned char I2CData[3];
		int size;
		int temp;
       
		size = I2C_Read( addr, command, I2CData, 3 );
		
		if( size != 3 )
		{
			//	printf("mlx90614 read failed!\r\n");
				return -1;
		}
        PEC=0;
        PEC = GetCrc8(PEC,0x16);
        PEC = GetCrc8(PEC,command);
        PEC = GetCrc8(PEC,0x16|0x01);
        PEC = GetCrc8(PEC,I2CData[0]);
        PEC = GetCrc8(PEC,I2CData[1]);
        temp = (I2CData[1] << 8 | I2CData[0]);
        if(PEC != I2CData[2])
        {
            return -1;
        }
		//temp = (I2CData[1] << 8 | I2CData[0])*2;
        
        
      //  PEC = I2CData[2];
		//temp -= 27315;
		
		return temp;
        
}

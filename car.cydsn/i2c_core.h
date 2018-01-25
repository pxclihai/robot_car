#ifndef __I2C_CORE_H__
#define __I2C_CORE_H__
#include <project.h>
void SYS_I2C_Init(void);

unsigned char I2C_Write( unsigned char slave_add, unsigned char I2CAddress, unsigned char *I2CData, unsigned char size );

unsigned char I2C_Read( unsigned char slave_add, unsigned char I2CAddress, unsigned char *I2CData, unsigned char size );

#endif

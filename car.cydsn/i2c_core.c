
#include "i2c_core.h"

#define I2C_CLK_HIGH() SCL_Write(1)
#define I2C_CLK_LOW()  SCL_Write(0)
#define I2C_DATA_HIGH() SDL_Write(1)
#define I2C_DATA_LOW()   SDL_Write(0)

#define I2C_DATA_IN() SDL_Read()

static void delay_us( int t )
{
		volatile int tt;
		for( ; t; t -- )
			for( tt = 0 ; tt < 1; tt ++ )
			;
}
void SYS_I2C_Init(void)
{
  I2C_CLK_HIGH();
  delay_us(20);
  I2C_CLK_LOW(); 
  delay_us(2000);
  I2C_CLK_HIGH();


}
static void I2C_Start(void)
{
    I2C_CLK_HIGH();
    delay_us(20);
    I2C_DATA_LOW();
    delay_us(20);
    I2C_CLK_LOW();
    delay_us(20);
    I2C_DATA_HIGH();
    delay_us(20);
}

static void I2C_Stop(void)
{
    I2C_DATA_LOW();
    delay_us(20);
    I2C_CLK_HIGH();
    delay_us(20);
    I2C_DATA_HIGH();
    delay_us(20);
}

static void I2C_nack( void )
{
    I2C_CLK_LOW();
    delay_us(20);
    I2C_DATA_HIGH();
    delay_us(20);
    I2C_CLK_HIGH();
    delay_us(20);
    I2C_CLK_LOW();
    delay_us(20);
}

static void I2C_ack( void )
{
    I2C_CLK_LOW();
    delay_us(20);
    I2C_DATA_LOW();
    delay_us(20);
    I2C_CLK_HIGH();
    delay_us(20);
    I2C_CLK_LOW();
    delay_us(20);
}

static void I2C_sendbyte( unsigned char  I2CSendData )
{
    unsigned char  i;
    
    for (i = 0;i < 8;i++)
    {
        if ( I2CSendData & 0x80 )
            I2C_DATA_HIGH();
        else
            I2C_DATA_LOW();
        delay_us(20);
        I2C_CLK_HIGH();
        delay_us(20);
        
        I2CSendData <<= 1; 
        I2C_CLK_LOW();
        delay_us(20);
    }
}
static unsigned char I2C_readbyte( void )
{
    unsigned char i;
    unsigned char ucRDData = 0;

    I2C_DATA_HIGH();

    for (i = 0;i < 8;i++)
    {
        I2C_CLK_HIGH();
        ucRDData <<= 1;
        delay_us(20);
        if (I2C_DATA_IN())
            ucRDData++;

        I2C_CLK_LOW();
        delay_us(20);
    }
    return(ucRDData);
}
static unsigned char ReceiveACK( void )
{
    unsigned char nakflag;
    I2C_DATA_HIGH();
    delay_us(20);
    I2C_CLK_HIGH();
    delay_us(20);
    nakflag = I2C_DATA_IN();
    I2C_CLK_LOW();
    delay_us(20);
    return nakflag;
}
uint8 a_SM_PecTable[16]={0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D};
uint8 SMbus_GetCrc8(uint8 previous, uint8  data)
{
	uint8 i, crc=0;
	crc = 0;
  data = previous ^ data;
  crc = (crc << 4) ^ a_SM_PecTable[((crc ^ data) >> 4) & 0x0F];
  data <<= 4;//准备下一个位域,域宽4
  crc = (crc << 4) ^ a_SM_PecTable[((crc ^ data) >> 4) & 0x0F];
  return crc;
}
unsigned char I2C_Write( unsigned char slave_add, unsigned char I2CAddress, unsigned char *I2CData, unsigned char size )
{
    unsigned char i;
    
    uint8 pec;
	pec = 0;
	pec = SMbus_GetCrc8(pec,slave_add);
	pec = SMbus_GetCrc8(pec,I2CAddress);
	pec = SMbus_GetCrc8(pec,*I2CData);
    pec = SMbus_GetCrc8(pec,0);
    I2C_Start();

    I2C_sendbyte( slave_add << 1 );
    if( ReceiveACK() )
    {
      I2C_Stop();
      return 0;
    };

    I2C_sendbyte(I2CAddress); 
    if( ReceiveACK() )
    {
      I2C_Stop();
      return 0;
    };

		for( i = 0 ; i < size; i ++ )
		{
			I2C_sendbyte(*I2CData++);
			if( ReceiveACK() )
					break;
            
		}
    I2C_sendbyte(pec);//pec
    if( ReceiveACK()
    )
    {
      I2C_Stop();
      return 0;
    };
    I2C_Stop(); 
    return i;
}

unsigned char I2C_Read( unsigned char slave_add, unsigned char I2CAddress, unsigned char *I2CData, unsigned char size )
{
	unsigned char i;

    I2C_Start(); 

    I2C_sendbyte( slave_add  );
    if( ReceiveACK() )
    {
      I2C_Stop();
      return 0;
    };
  //  CyDelay(50);
    I2C_sendbyte(I2CAddress); 
    if( ReceiveACK() )
    {
      I2C_Stop();
      return 0;
    };

    I2C_Start();

    I2C_sendbyte( (slave_add) | 1 );
    if( ReceiveACK() )
    {
      I2C_Stop();
      return 0;
    };

		for( i = 0 ; i< size; i ++ )
		{
			*I2CData++ = I2C_readbyte();
			
			if( i+1 == size )
				I2C_nack();
			else
				I2C_ack();
		}

    I2C_Stop();

    return  i;
}


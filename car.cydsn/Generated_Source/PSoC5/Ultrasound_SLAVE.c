/*******************************************************************************
* File Name: Ultrasound_SLAVE.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component Slave mode.
*
* Note:
*
*******************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Ultrasound_PVT.h"

#if (Ultrasound_MODE_SLAVE_ENABLED)

/**********************************
*      System variables
**********************************/

volatile uint8 Ultrasound_slStatus;   /* Slave Status  */

/* Transmit buffer variables */
volatile uint8 * Ultrasound_slRdBufPtr;   /* Pointer to Transmit buffer  */
volatile uint8   Ultrasound_slRdBufSize;  /* Slave Transmit buffer size  */
volatile uint8   Ultrasound_slRdBufIndex; /* Slave Transmit buffer Index */

/* Receive buffer variables */
volatile uint8 * Ultrasound_slWrBufPtr;   /* Pointer to Receive buffer  */
volatile uint8   Ultrasound_slWrBufSize;  /* Slave Receive buffer size  */
volatile uint8   Ultrasound_slWrBufIndex; /* Slave Receive buffer Index */

#if (Ultrasound_SW_ADRR_DECODE)
    volatile uint8 Ultrasound_slAddress;  /* Software address variable */
#endif /* (Ultrasound_SW_ADRR_DECODE) */


/*******************************************************************************
* Function Name: Ultrasound_SlaveStatus
********************************************************************************
*
* Summary:
*  Returns I2C slave's communication status.
*
* Parameters:
*  None.
*
* Return:
*  Current status of I2C slave.
*
* Global variables:
*  Ultrasound_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
*******************************************************************************/
uint8 Ultrasound_SlaveStatus(void) 
{
    return (Ultrasound_slStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveClearReadStatus
********************************************************************************
*
* Summary:
*  Clears the read status flags and returns they values.
*  The Ultrasound_SSTAT_RD_BUSY flag is not effected by clear.
*
* Parameters:
*  None.
*
* Return:
*  Current read status of I2C slave.
*
* Global variables:
*  Ultrasound_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_SlaveClearReadStatus(void) 
{
    uint8 status;

    Ultrasound_DisableInt(); /* Lock from interrupt */

    /* Mask of transfer complete flag and Error status */
    status = (Ultrasound_slStatus & Ultrasound_SSTAT_RD_MASK);
    Ultrasound_slStatus &= (uint8) ~Ultrasound_SSTAT_RD_CLEAR;

    Ultrasound_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveClearWriteStatus
********************************************************************************
*
* Summary:
*  Clears the write status flags and returns they values.
*  The Ultrasound_SSTAT_WR_BUSY flag is not effected by clear.
*
* Parameters:
*  None.
*
* Return:
*  Current write status of I2C slave.
*
* Global variables:
*  Ultrasound_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_SlaveClearWriteStatus(void) 
{
    uint8 status;

    Ultrasound_DisableInt(); /* Lock from interrupt */

    /* Mask of transfer complete flag and Error status */
    status = (Ultrasound_slStatus & Ultrasound_SSTAT_WR_MASK);
    Ultrasound_slStatus &= (uint8) ~Ultrasound_SSTAT_WR_CLEAR;

    Ultrasound_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveSetAddress
********************************************************************************
*
* Summary:
*  Sets the I2C slave address.
*
* Parameters:
*  address: I2C slave address for the primary device. This value may be any
*  address between 0 and 127.
*
* Return:
*  None.
*
* Global variables:
*  Ultrasound_Address  - The global variable used to store an I2C slave
*                              address for the primary device when the software
*                              address detect feature is used.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_SlaveSetAddress(uint8 address) 
{
#if (Ultrasound_SW_ADRR_DECODE)
    Ultrasound_slAddress = (address & Ultrasound_SLAVE_ADDR_MASK);
#else
    Ultrasound_ADDR_REG  = (address & Ultrasound_SLAVE_ADDR_MASK);
#endif /* (Ultrasound_SW_ADRR_DECODE) */
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveInitReadBuf
********************************************************************************
*
* Summary:
*  Sets the buffer pointer and size of the read buffer. This function also
*  resets the transfer count returned with the I2C_SlaveGetReadBufSize function.
*
* Parameters:
*  readBuf:  Pointer to the data buffer to be read by the master.
*  bufSize:  Size of the read buffer exposed to the I2C master.
*
* Return:
*  None.
*
* Global variables:
*  Ultrasound_slRdBufPtr   - The global variable used to store a pointer
*                                  to the slave read buffer.
*  Ultrasound_slRdBufSize  - The global variable used to store a slave
*                                  read buffer size.
*  Ultrasound_slRdBufIndex - The global variable used to store a current
*                                  index within the slave read buffer.
*
* Side Effects:
*  If this function is called during a bus transaction, data from the previous
*  buffer location and the beginning of the current buffer may be transmitted.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_SlaveInitReadBuf(uint8 * rdBuf, uint8 bufSize)
     
{
    if (NULL != rdBuf)
    {
        Ultrasound_DisableInt(); /* Lock from interrupt */

        /* Set buffer pointer */
        Ultrasound_slRdBufPtr   = (volatile uint8 *) rdBuf;
        Ultrasound_slRdBufSize  = bufSize;    /* Set buffer size     */
        Ultrasound_slRdBufIndex = 0u;         /* Clears buffer index */

        Ultrasound_EnableInt(); /* Release lock */
    }
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveInitWriteBuf
********************************************************************************
*
* Summary:
*  Sets the buffer pointer and size of the read buffer. This function also
*  resets the transfer count returned with the I2C_SlaveGetReadBufSize function.
*
* Parameters:
*  writeBuf:  Pointer to the data buffer to be read by the master.
*  bufSize:  Size of the buffer exposed to the I2C master.
*
* Return:
*  None.
*
* Global variables:
*  Ultrasound_slWrBufPtr   - The global variable used to store a pointer
*                                  to the slave write buffer.
*  Ultrasound_slWrBufSize  - The global variable used to store a slave
*                                  write buffer size.
*  Ultrasound_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
* Side Effects:
*  If this function is called during a bus transaction, data from the previous
*  buffer location and the beginning of the current buffer may be transmitted.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_SlaveInitWriteBuf(uint8 * wrBuf, uint8 bufSize)
     
{
    if (NULL != wrBuf)
    {
        Ultrasound_DisableInt(); /* Lock from interrupt */

         /* Set buffer pointer */
        Ultrasound_slWrBufPtr   = (volatile uint8 *) wrBuf;
        Ultrasound_slWrBufSize  = bufSize;    /* Set buffer size     */
        Ultrasound_slWrBufIndex = 0u;         /* Clears buffer index */

        Ultrasound_EnableInt(); /* Release lock */
    }
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes read by the I2C master since an
*  I2C_SlaveInitReadBuf or I2C_SlaveClearReadBuf function was executed.
*  The maximum return value will be the size of the read buffer.
*
* Parameters:
*  None.
*
* Return:
*  Bytes read by master.
*
* Global variables:
*  Ultrasound_slRdBufIndex - The global variable used to store a current
*                                  index within the slave read buffer.
*
*******************************************************************************/
uint8 Ultrasound_SlaveGetReadBufSize(void) 
{
    return (Ultrasound_slRdBufIndex);
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes written by the I2C master since an
*  I2C_SlaveInitWriteBuf or I2C_SlaveClearWriteBuf function was executed.
*  The maximum return value will be the size of the write buffer.
*
* Parameters:
*  None.
*
* Return:
*  Bytes written by master.
*
* Global variables:
*  Ultrasound_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
*******************************************************************************/
uint8 Ultrasound_SlaveGetWriteBufSize(void) 
{
    return (Ultrasound_slWrBufIndex);
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read pointer to the first byte in the read buffer. The next byte
*  read by the master will be the first byte in the read buffer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Ultrasound_slRdBufIndex - The global variable used to store a current
*                                  index the within slave read buffer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_SlaveClearReadBuf(void) 
{
    Ultrasound_slRdBufIndex = 0u;
}


/*******************************************************************************
* Function Name: Ultrasound_SlaveClearRxBuf
********************************************************************************
*
* Summary:
*  Resets the write pointer to the first byte in the write buffer. The next byte
*  written by the master will be the first byte in the write buffer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Ultrasound_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_SlaveClearWriteBuf(void) 
{
    Ultrasound_slWrBufIndex = 0u;
}

#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */


/* [] END OF FILE */

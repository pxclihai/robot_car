/*******************************************************************************
* File Name: WAVE_SLAVE.c
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

#include "WAVE_PVT.h"

#if (WAVE_MODE_SLAVE_ENABLED)

/**********************************
*      System variables
**********************************/

volatile uint8 WAVE_slStatus;   /* Slave Status  */

/* Transmit buffer variables */
volatile uint8 * WAVE_slRdBufPtr;   /* Pointer to Transmit buffer  */
volatile uint8   WAVE_slRdBufSize;  /* Slave Transmit buffer size  */
volatile uint8   WAVE_slRdBufIndex; /* Slave Transmit buffer Index */

/* Receive buffer variables */
volatile uint8 * WAVE_slWrBufPtr;   /* Pointer to Receive buffer  */
volatile uint8   WAVE_slWrBufSize;  /* Slave Receive buffer size  */
volatile uint8   WAVE_slWrBufIndex; /* Slave Receive buffer Index */

#if (WAVE_SW_ADRR_DECODE)
    volatile uint8 WAVE_slAddress;  /* Software address variable */
#endif /* (WAVE_SW_ADRR_DECODE) */


/*******************************************************************************
* Function Name: WAVE_SlaveStatus
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
*  WAVE_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
*******************************************************************************/
uint8 WAVE_SlaveStatus(void) 
{
    return (WAVE_slStatus);
}


/*******************************************************************************
* Function Name: WAVE_SlaveClearReadStatus
********************************************************************************
*
* Summary:
*  Clears the read status flags and returns they values.
*  The WAVE_SSTAT_RD_BUSY flag is not effected by clear.
*
* Parameters:
*  None.
*
* Return:
*  Current read status of I2C slave.
*
* Global variables:
*  WAVE_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_SlaveClearReadStatus(void) 
{
    uint8 status;

    WAVE_DisableInt(); /* Lock from interrupt */

    /* Mask of transfer complete flag and Error status */
    status = (WAVE_slStatus & WAVE_SSTAT_RD_MASK);
    WAVE_slStatus &= (uint8) ~WAVE_SSTAT_RD_CLEAR;

    WAVE_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: WAVE_SlaveClearWriteStatus
********************************************************************************
*
* Summary:
*  Clears the write status flags and returns they values.
*  The WAVE_SSTAT_WR_BUSY flag is not effected by clear.
*
* Parameters:
*  None.
*
* Return:
*  Current write status of I2C slave.
*
* Global variables:
*  WAVE_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_SlaveClearWriteStatus(void) 
{
    uint8 status;

    WAVE_DisableInt(); /* Lock from interrupt */

    /* Mask of transfer complete flag and Error status */
    status = (WAVE_slStatus & WAVE_SSTAT_WR_MASK);
    WAVE_slStatus &= (uint8) ~WAVE_SSTAT_WR_CLEAR;

    WAVE_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: WAVE_SlaveSetAddress
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
*  WAVE_Address  - The global variable used to store an I2C slave
*                              address for the primary device when the software
*                              address detect feature is used.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_SlaveSetAddress(uint8 address) 
{
#if (WAVE_SW_ADRR_DECODE)
    WAVE_slAddress = (address & WAVE_SLAVE_ADDR_MASK);
#else
    WAVE_ADDR_REG  = (address & WAVE_SLAVE_ADDR_MASK);
#endif /* (WAVE_SW_ADRR_DECODE) */
}


/*******************************************************************************
* Function Name: WAVE_SlaveInitReadBuf
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
*  WAVE_slRdBufPtr   - The global variable used to store a pointer
*                                  to the slave read buffer.
*  WAVE_slRdBufSize  - The global variable used to store a slave
*                                  read buffer size.
*  WAVE_slRdBufIndex - The global variable used to store a current
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
void WAVE_SlaveInitReadBuf(uint8 * rdBuf, uint8 bufSize)
     
{
    if (NULL != rdBuf)
    {
        WAVE_DisableInt(); /* Lock from interrupt */

        /* Set buffer pointer */
        WAVE_slRdBufPtr   = (volatile uint8 *) rdBuf;
        WAVE_slRdBufSize  = bufSize;    /* Set buffer size     */
        WAVE_slRdBufIndex = 0u;         /* Clears buffer index */

        WAVE_EnableInt(); /* Release lock */
    }
}


/*******************************************************************************
* Function Name: WAVE_SlaveInitWriteBuf
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
*  WAVE_slWrBufPtr   - The global variable used to store a pointer
*                                  to the slave write buffer.
*  WAVE_slWrBufSize  - The global variable used to store a slave
*                                  write buffer size.
*  WAVE_slWrBufIndex - The global variable used to store a current
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
void WAVE_SlaveInitWriteBuf(uint8 * wrBuf, uint8 bufSize)
     
{
    if (NULL != wrBuf)
    {
        WAVE_DisableInt(); /* Lock from interrupt */

         /* Set buffer pointer */
        WAVE_slWrBufPtr   = (volatile uint8 *) wrBuf;
        WAVE_slWrBufSize  = bufSize;    /* Set buffer size     */
        WAVE_slWrBufIndex = 0u;         /* Clears buffer index */

        WAVE_EnableInt(); /* Release lock */
    }
}


/*******************************************************************************
* Function Name: WAVE_SlaveGetReadBufSize
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
*  WAVE_slRdBufIndex - The global variable used to store a current
*                                  index within the slave read buffer.
*
*******************************************************************************/
uint8 WAVE_SlaveGetReadBufSize(void) 
{
    return (WAVE_slRdBufIndex);
}


/*******************************************************************************
* Function Name: WAVE_SlaveGetWriteBufSize
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
*  WAVE_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
*******************************************************************************/
uint8 WAVE_SlaveGetWriteBufSize(void) 
{
    return (WAVE_slWrBufIndex);
}


/*******************************************************************************
* Function Name: WAVE_SlaveClearReadBuf
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
*  WAVE_slRdBufIndex - The global variable used to store a current
*                                  index the within slave read buffer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_SlaveClearReadBuf(void) 
{
    WAVE_slRdBufIndex = 0u;
}


/*******************************************************************************
* Function Name: WAVE_SlaveClearRxBuf
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
*  WAVE_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_SlaveClearWriteBuf(void) 
{
    WAVE_slWrBufIndex = 0u;
}

#endif /* (WAVE_MODE_SLAVE_ENABLED) */


/* [] END OF FILE */
/*******************************************************************************
* File Name: WAVE_MASTER.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component master mode.
*
*******************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WAVE_PVT.h"

#if(WAVE_MODE_MASTER_ENABLED)

/**********************************
*      System variables
**********************************/

volatile uint8 WAVE_mstrStatus;     /* Master Status byte  */
volatile uint8 WAVE_mstrControl;    /* Master Control byte */

/* Transmit buffer variables */
volatile uint8 * WAVE_mstrRdBufPtr;     /* Pointer to Master Read buffer */
volatile uint8   WAVE_mstrRdBufSize;    /* Master Read buffer size       */
volatile uint8   WAVE_mstrRdBufIndex;   /* Master Read buffer Index      */

/* Receive buffer variables */
volatile uint8 * WAVE_mstrWrBufPtr;     /* Pointer to Master Write buffer */
volatile uint8   WAVE_mstrWrBufSize;    /* Master Write buffer size       */
volatile uint8   WAVE_mstrWrBufIndex;   /* Master Write buffer Index      */


/*******************************************************************************
* Function Name: WAVE_MasterWriteBuf
********************************************************************************
*
* Summary:
*  Automatically writes an entire buffer of data to a slave device. Once the
*  data transfer is initiated by this function, further data transfer is handled
*  by the included ISR in byte by byte mode.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer of data to be sent.
*  cnt:       Size of buffer to send.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Status error - Zero means no errors.
*
* Side Effects:
*  The included ISR will start a transfer after a start or restart condition is
*  generated.
*
* Global variables:
*  WAVE_mstrStatus  - The global variable used to store a current
*                                 status of the I2C Master.
*  WAVE_state       - The global variable used to store a current
*                                 state of the software FSM.
*  WAVE_mstrControl - The global variable used to control the master
*                                 end of a transaction with or without Stop
*                                 generation.
*  WAVE_mstrWrBufPtr - The global variable used to store a pointer
*                                  to the master write buffer.
*  WAVE_mstrWrBufIndex - The global variable used to store current
*                                    index within the master write buffer.
*  WAVE_mstrWrBufSize - The global variable used to store a master
*                                   write buffer size.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 WAVE_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode)
      
{
    uint8 errStatus;

    errStatus = WAVE_MSTR_NOT_READY;

    if(NULL != wrData)
    {
        /* Check I2C state to allow transfer: valid states are IDLE or HALT */
        if(WAVE_SM_IDLE == WAVE_state)
        {
            /* Master is ready for transaction: check if bus is free */
            if(WAVE_CHECK_BUS_FREE(WAVE_MCSR_REG))
            {
                errStatus = WAVE_MSTR_NO_ERROR;
            }
            else
            {
                errStatus = WAVE_MSTR_BUS_BUSY;
            }
        }
        else if(WAVE_SM_MSTR_HALT == WAVE_state)
        {
            /* Master is ready and waiting for ReStart */
            errStatus = WAVE_MSTR_NO_ERROR;

            WAVE_ClearPendingInt();
            WAVE_mstrStatus &= (uint8) ~WAVE_MSTAT_XFER_HALT;
        }
        else
        {
            /* errStatus = WAVE_MSTR_NOT_READY was send before */
        }

        if(WAVE_MSTR_NO_ERROR == errStatus)
        {
            /* Set state to start write transaction */
            WAVE_state = WAVE_SM_MSTR_WR_ADDR;

            /* Prepare write buffer */
            WAVE_mstrWrBufIndex = 0u;
            WAVE_mstrWrBufSize  = cnt;
            WAVE_mstrWrBufPtr   = (volatile uint8 *) wrData;

            /* Set end of transaction flag: Stop or Halt (following ReStart) */
            WAVE_mstrControl = mode;

            /* Clear write status history */
            WAVE_mstrStatus &= (uint8) ~WAVE_MSTAT_WR_CMPLT;

            /* Hardware actions: write address and generate Start or ReStart */
            WAVE_DATA_REG = (uint8) (slaveAddress << WAVE_SLAVE_ADDR_SHIFT);

            if(WAVE_CHECK_RESTART(mode))
            {
                WAVE_GENERATE_RESTART;
            }
            else
            {
                WAVE_GENERATE_START;
            }

            /* Enable interrupt to complete transfer */
            WAVE_EnableInt();
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: WAVE_MasterReadBuf
********************************************************************************
*
* Summary:
*  Automatically writes an entire buffer of data to a slave device. Once the
*  data transfer is initiated by this function, further data transfer is handled
*  by the included ISR in byte by byte mode.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer where to put data from slave.
*  cnt:       Size of buffer to read.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Status error - Zero means no errors.
*
* Side Effects:
*  The included ISR will start a transfer after start or restart condition is
*  generated.
*
* Global variables:
*  WAVE_mstrStatus  - The global variable used to store a current
*                                 status of the I2C Master.
*  WAVE_state       - The global variable used to store a current
*                                 state of the software FSM.
*  WAVE_mstrControl - The global variable used to control the master
*                                 end of a transaction with or without
*                                 Stop generation.
*  WAVE_mstrRdBufPtr - The global variable used to store a pointer
*                                  to the master write buffer.
*  WAVE_mstrRdBufIndex - The global variable  used to store a
*                                    current index within the master
*                                    write buffer.
*  WAVE_mstrRdBufSize - The global variable used to store a master
*                                   write buffer size.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode)
      
{
    uint8 errStatus;

    errStatus = WAVE_MSTR_NOT_READY;

    if(NULL != rdData)
    {
        /* Check I2C state to allow transfer: valid states are IDLE or HALT */
        if(WAVE_SM_IDLE == WAVE_state)
        {
            /* Master is ready to transaction: check if bus is free */
            if(WAVE_CHECK_BUS_FREE(WAVE_MCSR_REG))
            {
                errStatus = WAVE_MSTR_NO_ERROR;
            }
            else
            {
                errStatus = WAVE_MSTR_BUS_BUSY;
            }
        }
        else if(WAVE_SM_MSTR_HALT == WAVE_state)
        {
            /* Master is ready and waiting for ReStart */
            errStatus = WAVE_MSTR_NO_ERROR;

            WAVE_ClearPendingInt();
            WAVE_mstrStatus &= (uint8) ~WAVE_MSTAT_XFER_HALT;
        }
        else
        {
            /* errStatus = WAVE_MSTR_NOT_READY was set before */
        }

        if(WAVE_MSTR_NO_ERROR == errStatus)
        {
            /* Set state to start write transaction */
            WAVE_state = WAVE_SM_MSTR_RD_ADDR;

            /* Prepare read buffer */
            WAVE_mstrRdBufIndex  = 0u;
            WAVE_mstrRdBufSize   = cnt;
            WAVE_mstrRdBufPtr    = (volatile uint8 *) rdData;

            /* Set end of transaction flag: Stop or Halt (following ReStart) */
            WAVE_mstrControl = mode;

            /* Clear read status history */
            WAVE_mstrStatus &= (uint8) ~WAVE_MSTAT_RD_CMPLT;

            /* Hardware actions: write address and generate Start or ReStart */
            WAVE_DATA_REG = ((uint8) (slaveAddress << WAVE_SLAVE_ADDR_SHIFT) |
                                                  WAVE_READ_FLAG);

            if(WAVE_CHECK_RESTART(mode))
            {
                WAVE_GENERATE_RESTART;
            }
            else
            {
                WAVE_GENERATE_START;
            }

            /* Enable interrupt to complete transfer */
            WAVE_EnableInt();
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: WAVE_MasterSendStart
********************************************************************************
*
* Summary:
*  Generates Start condition and sends slave address with read/write bit.
*
* Parameters:
*  slaveAddress:  7-bit slave address.
*  R_nW:          Zero, send write command, non-zero send read command.
*
* Return:
*  Status error - Zero means no errors.
*
* Side Effects:
*  This function is entered without a "byte complete" bit set in the I2C_CSR
*  register. It does not exit until it is set.
*
* Global variables:
*  WAVE_state - The global variable used to store a current state of
*                           the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_MasterSendStart(uint8 slaveAddress, uint8 R_nW)
      
{
    uint8 errStatus;

    errStatus = WAVE_MSTR_NOT_READY;

    /* If IDLE, check if bus is free */
    if(WAVE_SM_IDLE == WAVE_state)
    {
        /* If bus is free, generate Start condition */
        if(WAVE_CHECK_BUS_FREE(WAVE_MCSR_REG))
        {
            /* Disable interrupt for manual master operation */
            WAVE_DisableInt();

            /* Set address and read/write flag */
            slaveAddress = (uint8) (slaveAddress << WAVE_SLAVE_ADDR_SHIFT);
            if(0u != R_nW)
            {
                slaveAddress |= WAVE_READ_FLAG;
                WAVE_state = WAVE_SM_MSTR_RD_ADDR;
            }
            else
            {
                WAVE_state = WAVE_SM_MSTR_WR_ADDR;
            }

            /* Hardware actions: write address and generate Start */
            WAVE_DATA_REG = slaveAddress;
            WAVE_GENERATE_START_MANUAL;

            /* Wait until address is transferred */
            while(WAVE_WAIT_BYTE_COMPLETE(WAVE_CSR_REG))
            {
            }

        #if(WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(WAVE_CHECK_START_GEN(WAVE_MCSR_REG))
            {
                WAVE_CLEAR_START_GEN;

                /* Start condition was not generated: reset FSM to IDLE */
                WAVE_state = WAVE_SM_IDLE;
                errStatus = WAVE_MSTR_ERR_ABORT_START_GEN;
            }
            else
        #endif /* (WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED) */

        #if(WAVE_MODE_MULTI_MASTER_ENABLED)
            if(WAVE_CHECK_LOST_ARB(WAVE_CSR_REG))
            {
                WAVE_BUS_RELEASE_MANUAL;

                /* Master lost arbitrage: reset FSM to IDLE */
                WAVE_state = WAVE_SM_IDLE;
                errStatus = WAVE_MSTR_ERR_ARB_LOST;
            }
            else
        #endif /* (WAVE_MODE_MULTI_MASTER_ENABLED) */

            if(WAVE_CHECK_ADDR_NAK(WAVE_CSR_REG))
            {
                /* Address has been NACKed: reset FSM to IDLE */
                WAVE_state = WAVE_SM_IDLE;
                errStatus = WAVE_MSTR_ERR_LB_NAK;
            }
            else
            {
                /* Start was sent without errors */
                errStatus = WAVE_MSTR_NO_ERROR;
            }
        }
        else
        {
            errStatus = WAVE_MSTR_BUS_BUSY;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: WAVE_MasterSendRestart
********************************************************************************
*
* Summary:
*  Generates ReStart condition and sends slave address with read/write bit.
*
* Parameters:
*  slaveAddress:  7-bit slave address.
*  R_nW:          Zero, send write command, non-zero send read command.
*
* Return:
*  Status error - Zero means no errors.
*
* Side Effects:
*  This function is entered without a "byte complete" bit set in the I2C_CSR
*  register. It does not exit until it is set.
*
* Global variables:
*  WAVE_state - The global variable used to store a current state of
*                           the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_MasterSendRestart(uint8 slaveAddress, uint8 R_nW)
      
{
    uint8 errStatus;

    errStatus = WAVE_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(WAVE_CHECK_MASTER_MODE(WAVE_MCSR_REG))
    {
        /* Set address and read/write flag */
        slaveAddress = (uint8) (slaveAddress << WAVE_SLAVE_ADDR_SHIFT);
        if(0u != R_nW)
        {
            slaveAddress |= WAVE_READ_FLAG;
            WAVE_state = WAVE_SM_MSTR_RD_ADDR;
        }
        else
        {
            WAVE_state = WAVE_SM_MSTR_WR_ADDR;
        }

        /* Hardware actions: write address and generate ReStart */
        WAVE_DATA_REG = slaveAddress;
        WAVE_GENERATE_RESTART_MANUAL;

        /* Wait until address has been transferred */
        while(WAVE_WAIT_BYTE_COMPLETE(WAVE_CSR_REG))
        {
        }

    #if(WAVE_MODE_MULTI_MASTER_ENABLED)
        if(WAVE_CHECK_LOST_ARB(WAVE_CSR_REG))
        {
            WAVE_BUS_RELEASE_MANUAL;

            /* Master lost arbitrage: reset FSM to IDLE */
            WAVE_state = WAVE_SM_IDLE;
            errStatus = WAVE_MSTR_ERR_ARB_LOST;
        }
        else
    #endif /* (WAVE_MODE_MULTI_MASTER_ENABLED) */

        if(WAVE_CHECK_ADDR_NAK(WAVE_CSR_REG))
        {
            /* Address has been NACKed: reset FSM to IDLE */
            WAVE_state = WAVE_SM_IDLE;
            errStatus = WAVE_MSTR_ERR_LB_NAK;
        }
        else
        {
            /* ReStart was sent without errors */
            errStatus = WAVE_MSTR_NO_ERROR;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: WAVE_MasterSendStop
********************************************************************************
*
* Summary:
*  Generates I2C Stop condition on bus. Function do nothing if Start or Restart
*  condition was failed before call this function.
*
* Parameters:
*  None.
*
* Return:
*  Status error - Zero means no errors.
*
* Side Effects:
*  Stop generation is required to complete the transaction.
*  This function does not wait until a Stop condition is generated.
*
* Global variables:
*  WAVE_state - The global variable used to store a current state of
*                           the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_MasterSendStop(void) 
{
    uint8 errStatus;

    errStatus = WAVE_MSTR_NOT_READY;

    /* Check if master is active on bus */
    if(WAVE_CHECK_MASTER_MODE(WAVE_MCSR_REG))
    {
        WAVE_GENERATE_STOP_MANUAL;
        WAVE_state = WAVE_SM_IDLE;

        /* Wait until stop has been generated */
        while(WAVE_WAIT_STOP_COMPLETE(WAVE_CSR_REG))
        {
        }

        errStatus = WAVE_MSTR_NO_ERROR;

    #if(WAVE_MODE_MULTI_MASTER_ENABLED)
        if(WAVE_CHECK_LOST_ARB(WAVE_CSR_REG))
        {
            WAVE_BUS_RELEASE_MANUAL;

            /* NACK was generated by instead Stop */
            errStatus = WAVE_MSTR_ERR_ARB_LOST;
        }
    #endif /* (WAVE_MODE_MULTI_MASTER_ENABLED) */
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: WAVE_MasterWriteByte
********************************************************************************
*
* Summary:
*  Sends one byte to a slave. A valid Start or ReStart condition must be
*  generated before this call this function. Function do nothing if Start or
*  Restart condition was failed before call this function.
*
* Parameters:
*  data:  The data byte to send to the slave.
*
* Return:
*  Status error - Zero means no errors.
*
* Side Effects:
*  This function is entered without a "byte complete" bit set in the I2C_CSR
*  register. It does not exit until it is set.
*
* Global variables:
*  WAVE_state - The global variable used to store a current state of
*                           the software FSM.
*
*******************************************************************************/
uint8 WAVE_MasterWriteByte(uint8 theByte) 
{
    uint8 errStatus;

    errStatus = WAVE_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(WAVE_CHECK_MASTER_MODE(WAVE_MCSR_REG))
    {
        WAVE_DATA_REG = theByte;   /* Write DATA register */
        WAVE_TRANSMIT_DATA_MANUAL; /* Set transmit mode   */
        WAVE_state = WAVE_SM_MSTR_WR_DATA;

        /* Wait until data byte has been transmitted */
        while(WAVE_WAIT_BYTE_COMPLETE(WAVE_CSR_REG))
        {
        }

    #if(WAVE_MODE_MULTI_MASTER_ENABLED)
        if(WAVE_CHECK_LOST_ARB(WAVE_CSR_REG))
        {
            WAVE_BUS_RELEASE_MANUAL;

            /* Master lost arbitrage: reset FSM to IDLE */
            WAVE_state = WAVE_SM_IDLE;
            errStatus = WAVE_MSTR_ERR_ARB_LOST;
        }
        /* Check LRB bit */
        else
    #endif /* (WAVE_MODE_MULTI_MASTER_ENABLED) */

        if(WAVE_CHECK_DATA_ACK(WAVE_CSR_REG))
        {
            WAVE_state = WAVE_SM_MSTR_HALT;
            errStatus = WAVE_MSTR_NO_ERROR;
        }
        else
        {
            WAVE_state = WAVE_SM_MSTR_HALT;
            errStatus = WAVE_MSTR_ERR_LB_NAK;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: WAVE_MasterReadByte
********************************************************************************
*
* Summary:
*  Reads one byte from a slave and ACK or NACK the transfer. A valid Start or
*  ReStart condition must be generated before this call this function. Function
*  do nothing if Start or Restart condition was failed before call this
*  function.
*
* Parameters:
*  acknNack:  Zero, response with NACK, if non-zero response with ACK.
*
* Return:
*  Byte read from slave.
*
* Side Effects:
*  This function is entered without a "byte complete" bit set in the I2C_CSR
*  register. It does not exit until it is set.
*
* Global variables:
*  WAVE_state - The global variable used to store a current
*                           state of the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_MasterReadByte(uint8 acknNak) 
{
    uint8 theByte;

    theByte = 0u;

    /* Check if START condition was generated */
    if(WAVE_CHECK_MASTER_MODE(WAVE_MCSR_REG))
    {
        /* When address phase needs to release bus and receive byte,
        * then decide ACK or NACK
        */
        if(WAVE_SM_MSTR_RD_ADDR == WAVE_state)
        {
            WAVE_READY_TO_READ_MANUAL;
            WAVE_state = WAVE_SM_MSTR_RD_DATA;
        }

        /* Wait until data byte has been received */
        while(WAVE_WAIT_BYTE_COMPLETE(WAVE_CSR_REG))
        {
        }

        theByte = WAVE_DATA_REG;

        /* Command ACK to receive next byte and continue transfer.
        *  Do nothing for NACK. The NACK will be generated by
        *  Stop or ReStart routine.
        */
        if(acknNak != 0u) /* Generate ACK */
        {
            WAVE_ACK_AND_RECEIVE_MANUAL;
        }
        else              /* Do nothing for the follwong NACK */
        {
            WAVE_state = WAVE_SM_MSTR_HALT;
        }
    }

    return(theByte);
}


/*******************************************************************************
* Function Name: WAVE_MasterStatus
********************************************************************************
*
* Summary:
*  Returns the master's communication status.
*
* Parameters:
*  None.
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  WAVE_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
*******************************************************************************/
uint8 WAVE_MasterStatus(void) 
{
    uint8 status;

    WAVE_DisableInt(); /* Lock from interrupt */

    /* Read master status */
    status = WAVE_mstrStatus;

    if (WAVE_CHECK_SM_MASTER)
    {
        /* Set transfer in progress flag in status */
        status |= WAVE_MSTAT_XFER_INP;
    }

    WAVE_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: WAVE_MasterClearStatus
********************************************************************************
*
* Summary:
*  Clears all status flags and returns the master status.
*
* Parameters:
*  None.
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  WAVE_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 WAVE_MasterClearStatus(void) 
{
    uint8 status;

    WAVE_DisableInt(); /* Lock from interrupt */

    /* Read and clear master status */
    status = WAVE_mstrStatus;
    WAVE_mstrStatus = WAVE_MSTAT_CLEAR;

    WAVE_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: WAVE_MasterGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the amount of bytes that has been transferred with an
*  I2C_MasterReadBuf command.
*
* Parameters:
*  None.
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it will return
*  the byte count transferred so far.
*
* Global variables:
*  WAVE_mstrRdBufIndex - The global variable stores current index
*                                    within the master read buffer.
*
*******************************************************************************/
uint8 WAVE_MasterGetReadBufSize(void) 
{
    return (WAVE_mstrRdBufIndex);
}


/*******************************************************************************
* Function Name: WAVE_MasterGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the amount of bytes that has been transferred with an
*  I2C_MasterWriteBuf command.
*
* Parameters:
*  None.
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it will return
*  the byte count transferred so far.
*
* Global variables:
*  WAVE_mstrWrBufIndex -  The global variable used to stores current
*                                     index within master write buffer.
*
*******************************************************************************/
uint8 WAVE_MasterGetWriteBufSize(void) 
{
    return (WAVE_mstrWrBufIndex);
}


/*******************************************************************************
* Function Name: WAVE_MasterClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  WAVE_mstrRdBufIndex - The global variable used to stores current
*                                    index within master read buffer.
*  WAVE_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_MasterClearReadBuf(void) 
{
    WAVE_DisableInt(); /* Lock from interrupt */

    WAVE_mstrRdBufIndex = 0u;
    WAVE_mstrStatus    &= (uint8) ~WAVE_MSTAT_RD_CMPLT;

    WAVE_EnableInt(); /* Release lock */
}


/*******************************************************************************
* Function Name: WAVE_MasterClearWriteBuf
********************************************************************************
*
* Summary:
*  Resets the write buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  WAVE_mstrRdBufIndex - The global variable used to stote current
*                                    index within master read buffer.
*  WAVE_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_MasterClearWriteBuf(void) 
{
    WAVE_DisableInt(); /* Lock from interrupt */

    WAVE_mstrWrBufIndex = 0u;
    WAVE_mstrStatus    &= (uint8) ~WAVE_MSTAT_WR_CMPLT;

    WAVE_EnableInt(); /* Release lock */
}

#endif /* (WAVE_MODE_MASTER_ENABLED) */


/* [] END OF FILE */

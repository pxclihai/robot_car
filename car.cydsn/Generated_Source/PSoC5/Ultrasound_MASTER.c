/*******************************************************************************
* File Name: Ultrasound_MASTER.c
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

#include "Ultrasound_PVT.h"

#if(Ultrasound_MODE_MASTER_ENABLED)

/**********************************
*      System variables
**********************************/

volatile uint8 Ultrasound_mstrStatus;     /* Master Status byte  */
volatile uint8 Ultrasound_mstrControl;    /* Master Control byte */

/* Transmit buffer variables */
volatile uint8 * Ultrasound_mstrRdBufPtr;     /* Pointer to Master Read buffer */
volatile uint8   Ultrasound_mstrRdBufSize;    /* Master Read buffer size       */
volatile uint8   Ultrasound_mstrRdBufIndex;   /* Master Read buffer Index      */

/* Receive buffer variables */
volatile uint8 * Ultrasound_mstrWrBufPtr;     /* Pointer to Master Write buffer */
volatile uint8   Ultrasound_mstrWrBufSize;    /* Master Write buffer size       */
volatile uint8   Ultrasound_mstrWrBufIndex;   /* Master Write buffer Index      */


/*******************************************************************************
* Function Name: Ultrasound_MasterWriteBuf
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
*  Ultrasound_mstrStatus  - The global variable used to store a current
*                                 status of the I2C Master.
*  Ultrasound_state       - The global variable used to store a current
*                                 state of the software FSM.
*  Ultrasound_mstrControl - The global variable used to control the master
*                                 end of a transaction with or without Stop
*                                 generation.
*  Ultrasound_mstrWrBufPtr - The global variable used to store a pointer
*                                  to the master write buffer.
*  Ultrasound_mstrWrBufIndex - The global variable used to store current
*                                    index within the master write buffer.
*  Ultrasound_mstrWrBufSize - The global variable used to store a master
*                                   write buffer size.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 Ultrasound_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode)
      
{
    uint8 errStatus;

    errStatus = Ultrasound_MSTR_NOT_READY;

    if(NULL != wrData)
    {
        /* Check I2C state to allow transfer: valid states are IDLE or HALT */
        if(Ultrasound_SM_IDLE == Ultrasound_state)
        {
            /* Master is ready for transaction: check if bus is free */
            if(Ultrasound_CHECK_BUS_FREE(Ultrasound_MCSR_REG))
            {
                errStatus = Ultrasound_MSTR_NO_ERROR;
            }
            else
            {
                errStatus = Ultrasound_MSTR_BUS_BUSY;
            }
        }
        else if(Ultrasound_SM_MSTR_HALT == Ultrasound_state)
        {
            /* Master is ready and waiting for ReStart */
            errStatus = Ultrasound_MSTR_NO_ERROR;

            Ultrasound_ClearPendingInt();
            Ultrasound_mstrStatus &= (uint8) ~Ultrasound_MSTAT_XFER_HALT;
        }
        else
        {
            /* errStatus = Ultrasound_MSTR_NOT_READY was send before */
        }

        if(Ultrasound_MSTR_NO_ERROR == errStatus)
        {
            /* Set state to start write transaction */
            Ultrasound_state = Ultrasound_SM_MSTR_WR_ADDR;

            /* Prepare write buffer */
            Ultrasound_mstrWrBufIndex = 0u;
            Ultrasound_mstrWrBufSize  = cnt;
            Ultrasound_mstrWrBufPtr   = (volatile uint8 *) wrData;

            /* Set end of transaction flag: Stop or Halt (following ReStart) */
            Ultrasound_mstrControl = mode;

            /* Clear write status history */
            Ultrasound_mstrStatus &= (uint8) ~Ultrasound_MSTAT_WR_CMPLT;

            /* Hardware actions: write address and generate Start or ReStart */
            Ultrasound_DATA_REG = (uint8) (slaveAddress << Ultrasound_SLAVE_ADDR_SHIFT);

            if(Ultrasound_CHECK_RESTART(mode))
            {
                Ultrasound_GENERATE_RESTART;
            }
            else
            {
                Ultrasound_GENERATE_START;
            }

            /* Enable interrupt to complete transfer */
            Ultrasound_EnableInt();
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterReadBuf
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
*  Ultrasound_mstrStatus  - The global variable used to store a current
*                                 status of the I2C Master.
*  Ultrasound_state       - The global variable used to store a current
*                                 state of the software FSM.
*  Ultrasound_mstrControl - The global variable used to control the master
*                                 end of a transaction with or without
*                                 Stop generation.
*  Ultrasound_mstrRdBufPtr - The global variable used to store a pointer
*                                  to the master write buffer.
*  Ultrasound_mstrRdBufIndex - The global variable  used to store a
*                                    current index within the master
*                                    write buffer.
*  Ultrasound_mstrRdBufSize - The global variable used to store a master
*                                   write buffer size.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode)
      
{
    uint8 errStatus;

    errStatus = Ultrasound_MSTR_NOT_READY;

    if(NULL != rdData)
    {
        /* Check I2C state to allow transfer: valid states are IDLE or HALT */
        if(Ultrasound_SM_IDLE == Ultrasound_state)
        {
            /* Master is ready to transaction: check if bus is free */
            if(Ultrasound_CHECK_BUS_FREE(Ultrasound_MCSR_REG))
            {
                errStatus = Ultrasound_MSTR_NO_ERROR;
            }
            else
            {
                errStatus = Ultrasound_MSTR_BUS_BUSY;
            }
        }
        else if(Ultrasound_SM_MSTR_HALT == Ultrasound_state)
        {
            /* Master is ready and waiting for ReStart */
            errStatus = Ultrasound_MSTR_NO_ERROR;

            Ultrasound_ClearPendingInt();
            Ultrasound_mstrStatus &= (uint8) ~Ultrasound_MSTAT_XFER_HALT;
        }
        else
        {
            /* errStatus = Ultrasound_MSTR_NOT_READY was set before */
        }

        if(Ultrasound_MSTR_NO_ERROR == errStatus)
        {
            /* Set state to start write transaction */
            Ultrasound_state = Ultrasound_SM_MSTR_RD_ADDR;

            /* Prepare read buffer */
            Ultrasound_mstrRdBufIndex  = 0u;
            Ultrasound_mstrRdBufSize   = cnt;
            Ultrasound_mstrRdBufPtr    = (volatile uint8 *) rdData;

            /* Set end of transaction flag: Stop or Halt (following ReStart) */
            Ultrasound_mstrControl = mode;

            /* Clear read status history */
            Ultrasound_mstrStatus &= (uint8) ~Ultrasound_MSTAT_RD_CMPLT;

            /* Hardware actions: write address and generate Start or ReStart */
            Ultrasound_DATA_REG = ((uint8) (slaveAddress << Ultrasound_SLAVE_ADDR_SHIFT) |
                                                  Ultrasound_READ_FLAG);

            if(Ultrasound_CHECK_RESTART(mode))
            {
                Ultrasound_GENERATE_RESTART;
            }
            else
            {
                Ultrasound_GENERATE_START;
            }

            /* Enable interrupt to complete transfer */
            Ultrasound_EnableInt();
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterSendStart
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
*  Ultrasound_state - The global variable used to store a current state of
*                           the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_MasterSendStart(uint8 slaveAddress, uint8 R_nW)
      
{
    uint8 errStatus;

    errStatus = Ultrasound_MSTR_NOT_READY;

    /* If IDLE, check if bus is free */
    if(Ultrasound_SM_IDLE == Ultrasound_state)
    {
        /* If bus is free, generate Start condition */
        if(Ultrasound_CHECK_BUS_FREE(Ultrasound_MCSR_REG))
        {
            /* Disable interrupt for manual master operation */
            Ultrasound_DisableInt();

            /* Set address and read/write flag */
            slaveAddress = (uint8) (slaveAddress << Ultrasound_SLAVE_ADDR_SHIFT);
            if(0u != R_nW)
            {
                slaveAddress |= Ultrasound_READ_FLAG;
                Ultrasound_state = Ultrasound_SM_MSTR_RD_ADDR;
            }
            else
            {
                Ultrasound_state = Ultrasound_SM_MSTR_WR_ADDR;
            }

            /* Hardware actions: write address and generate Start */
            Ultrasound_DATA_REG = slaveAddress;
            Ultrasound_GENERATE_START_MANUAL;

            /* Wait until address is transferred */
            while(Ultrasound_WAIT_BYTE_COMPLETE(Ultrasound_CSR_REG))
            {
            }

        #if(Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(Ultrasound_CHECK_START_GEN(Ultrasound_MCSR_REG))
            {
                Ultrasound_CLEAR_START_GEN;

                /* Start condition was not generated: reset FSM to IDLE */
                Ultrasound_state = Ultrasound_SM_IDLE;
                errStatus = Ultrasound_MSTR_ERR_ABORT_START_GEN;
            }
            else
        #endif /* (Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED) */

        #if(Ultrasound_MODE_MULTI_MASTER_ENABLED)
            if(Ultrasound_CHECK_LOST_ARB(Ultrasound_CSR_REG))
            {
                Ultrasound_BUS_RELEASE_MANUAL;

                /* Master lost arbitrage: reset FSM to IDLE */
                Ultrasound_state = Ultrasound_SM_IDLE;
                errStatus = Ultrasound_MSTR_ERR_ARB_LOST;
            }
            else
        #endif /* (Ultrasound_MODE_MULTI_MASTER_ENABLED) */

            if(Ultrasound_CHECK_ADDR_NAK(Ultrasound_CSR_REG))
            {
                /* Address has been NACKed: reset FSM to IDLE */
                Ultrasound_state = Ultrasound_SM_IDLE;
                errStatus = Ultrasound_MSTR_ERR_LB_NAK;
            }
            else
            {
                /* Start was sent without errors */
                errStatus = Ultrasound_MSTR_NO_ERROR;
            }
        }
        else
        {
            errStatus = Ultrasound_MSTR_BUS_BUSY;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterSendRestart
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
*  Ultrasound_state - The global variable used to store a current state of
*                           the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_MasterSendRestart(uint8 slaveAddress, uint8 R_nW)
      
{
    uint8 errStatus;

    errStatus = Ultrasound_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(Ultrasound_CHECK_MASTER_MODE(Ultrasound_MCSR_REG))
    {
        /* Set address and read/write flag */
        slaveAddress = (uint8) (slaveAddress << Ultrasound_SLAVE_ADDR_SHIFT);
        if(0u != R_nW)
        {
            slaveAddress |= Ultrasound_READ_FLAG;
            Ultrasound_state = Ultrasound_SM_MSTR_RD_ADDR;
        }
        else
        {
            Ultrasound_state = Ultrasound_SM_MSTR_WR_ADDR;
        }

        /* Hardware actions: write address and generate ReStart */
        Ultrasound_DATA_REG = slaveAddress;
        Ultrasound_GENERATE_RESTART_MANUAL;

        /* Wait until address has been transferred */
        while(Ultrasound_WAIT_BYTE_COMPLETE(Ultrasound_CSR_REG))
        {
        }

    #if(Ultrasound_MODE_MULTI_MASTER_ENABLED)
        if(Ultrasound_CHECK_LOST_ARB(Ultrasound_CSR_REG))
        {
            Ultrasound_BUS_RELEASE_MANUAL;

            /* Master lost arbitrage: reset FSM to IDLE */
            Ultrasound_state = Ultrasound_SM_IDLE;
            errStatus = Ultrasound_MSTR_ERR_ARB_LOST;
        }
        else
    #endif /* (Ultrasound_MODE_MULTI_MASTER_ENABLED) */

        if(Ultrasound_CHECK_ADDR_NAK(Ultrasound_CSR_REG))
        {
            /* Address has been NACKed: reset FSM to IDLE */
            Ultrasound_state = Ultrasound_SM_IDLE;
            errStatus = Ultrasound_MSTR_ERR_LB_NAK;
        }
        else
        {
            /* ReStart was sent without errors */
            errStatus = Ultrasound_MSTR_NO_ERROR;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterSendStop
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
*  Ultrasound_state - The global variable used to store a current state of
*                           the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_MasterSendStop(void) 
{
    uint8 errStatus;

    errStatus = Ultrasound_MSTR_NOT_READY;

    /* Check if master is active on bus */
    if(Ultrasound_CHECK_MASTER_MODE(Ultrasound_MCSR_REG))
    {
        Ultrasound_GENERATE_STOP_MANUAL;
        Ultrasound_state = Ultrasound_SM_IDLE;

        /* Wait until stop has been generated */
        while(Ultrasound_WAIT_STOP_COMPLETE(Ultrasound_CSR_REG))
        {
        }

        errStatus = Ultrasound_MSTR_NO_ERROR;

    #if(Ultrasound_MODE_MULTI_MASTER_ENABLED)
        if(Ultrasound_CHECK_LOST_ARB(Ultrasound_CSR_REG))
        {
            Ultrasound_BUS_RELEASE_MANUAL;

            /* NACK was generated by instead Stop */
            errStatus = Ultrasound_MSTR_ERR_ARB_LOST;
        }
    #endif /* (Ultrasound_MODE_MULTI_MASTER_ENABLED) */
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterWriteByte
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
*  Ultrasound_state - The global variable used to store a current state of
*                           the software FSM.
*
*******************************************************************************/
uint8 Ultrasound_MasterWriteByte(uint8 theByte) 
{
    uint8 errStatus;

    errStatus = Ultrasound_MSTR_NOT_READY;

    /* Check if START condition was generated */
    if(Ultrasound_CHECK_MASTER_MODE(Ultrasound_MCSR_REG))
    {
        Ultrasound_DATA_REG = theByte;   /* Write DATA register */
        Ultrasound_TRANSMIT_DATA_MANUAL; /* Set transmit mode   */
        Ultrasound_state = Ultrasound_SM_MSTR_WR_DATA;

        /* Wait until data byte has been transmitted */
        while(Ultrasound_WAIT_BYTE_COMPLETE(Ultrasound_CSR_REG))
        {
        }

    #if(Ultrasound_MODE_MULTI_MASTER_ENABLED)
        if(Ultrasound_CHECK_LOST_ARB(Ultrasound_CSR_REG))
        {
            Ultrasound_BUS_RELEASE_MANUAL;

            /* Master lost arbitrage: reset FSM to IDLE */
            Ultrasound_state = Ultrasound_SM_IDLE;
            errStatus = Ultrasound_MSTR_ERR_ARB_LOST;
        }
        /* Check LRB bit */
        else
    #endif /* (Ultrasound_MODE_MULTI_MASTER_ENABLED) */

        if(Ultrasound_CHECK_DATA_ACK(Ultrasound_CSR_REG))
        {
            Ultrasound_state = Ultrasound_SM_MSTR_HALT;
            errStatus = Ultrasound_MSTR_NO_ERROR;
        }
        else
        {
            Ultrasound_state = Ultrasound_SM_MSTR_HALT;
            errStatus = Ultrasound_MSTR_ERR_LB_NAK;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterReadByte
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
*  Ultrasound_state - The global variable used to store a current
*                           state of the software FSM.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_MasterReadByte(uint8 acknNak) 
{
    uint8 theByte;

    theByte = 0u;

    /* Check if START condition was generated */
    if(Ultrasound_CHECK_MASTER_MODE(Ultrasound_MCSR_REG))
    {
        /* When address phase needs to release bus and receive byte,
        * then decide ACK or NACK
        */
        if(Ultrasound_SM_MSTR_RD_ADDR == Ultrasound_state)
        {
            Ultrasound_READY_TO_READ_MANUAL;
            Ultrasound_state = Ultrasound_SM_MSTR_RD_DATA;
        }

        /* Wait until data byte has been received */
        while(Ultrasound_WAIT_BYTE_COMPLETE(Ultrasound_CSR_REG))
        {
        }

        theByte = Ultrasound_DATA_REG;

        /* Command ACK to receive next byte and continue transfer.
        *  Do nothing for NACK. The NACK will be generated by
        *  Stop or ReStart routine.
        */
        if(acknNak != 0u) /* Generate ACK */
        {
            Ultrasound_ACK_AND_RECEIVE_MANUAL;
        }
        else              /* Do nothing for the follwong NACK */
        {
            Ultrasound_state = Ultrasound_SM_MSTR_HALT;
        }
    }

    return(theByte);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterStatus
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
*  Ultrasound_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
*******************************************************************************/
uint8 Ultrasound_MasterStatus(void) 
{
    uint8 status;

    Ultrasound_DisableInt(); /* Lock from interrupt */

    /* Read master status */
    status = Ultrasound_mstrStatus;

    if (Ultrasound_CHECK_SM_MASTER)
    {
        /* Set transfer in progress flag in status */
        status |= Ultrasound_MSTAT_XFER_INP;
    }

    Ultrasound_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterClearStatus
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
*  Ultrasound_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 Ultrasound_MasterClearStatus(void) 
{
    uint8 status;

    Ultrasound_DisableInt(); /* Lock from interrupt */

    /* Read and clear master status */
    status = Ultrasound_mstrStatus;
    Ultrasound_mstrStatus = Ultrasound_MSTAT_CLEAR;

    Ultrasound_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterGetReadBufSize
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
*  Ultrasound_mstrRdBufIndex - The global variable stores current index
*                                    within the master read buffer.
*
*******************************************************************************/
uint8 Ultrasound_MasterGetReadBufSize(void) 
{
    return (Ultrasound_mstrRdBufIndex);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterGetWriteBufSize
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
*  Ultrasound_mstrWrBufIndex -  The global variable used to stores current
*                                     index within master write buffer.
*
*******************************************************************************/
uint8 Ultrasound_MasterGetWriteBufSize(void) 
{
    return (Ultrasound_mstrWrBufIndex);
}


/*******************************************************************************
* Function Name: Ultrasound_MasterClearReadBuf
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
*  Ultrasound_mstrRdBufIndex - The global variable used to stores current
*                                    index within master read buffer.
*  Ultrasound_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_MasterClearReadBuf(void) 
{
    Ultrasound_DisableInt(); /* Lock from interrupt */

    Ultrasound_mstrRdBufIndex = 0u;
    Ultrasound_mstrStatus    &= (uint8) ~Ultrasound_MSTAT_RD_CMPLT;

    Ultrasound_EnableInt(); /* Release lock */
}


/*******************************************************************************
* Function Name: Ultrasound_MasterClearWriteBuf
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
*  Ultrasound_mstrRdBufIndex - The global variable used to stote current
*                                    index within master read buffer.
*  Ultrasound_mstrStatus - The global variable used to store a current
*                                status of the I2C Master.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_MasterClearWriteBuf(void) 
{
    Ultrasound_DisableInt(); /* Lock from interrupt */

    Ultrasound_mstrWrBufIndex = 0u;
    Ultrasound_mstrStatus    &= (uint8) ~Ultrasound_MSTAT_WR_CMPLT;

    Ultrasound_EnableInt(); /* Release lock */
}

#endif /* (Ultrasound_MODE_MASTER_ENABLED) */


/* [] END OF FILE */

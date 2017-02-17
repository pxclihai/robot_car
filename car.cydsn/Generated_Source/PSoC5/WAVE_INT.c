/*******************************************************************************
* File Name: WAVE_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WAVE_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here.
********************************************************************************/
/* `#START WAVE_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: WAVE_ISR
********************************************************************************
*
* Summary:
*  The handler for the I2C interrupt. The slave and master operations are
*  handled here.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
CY_ISR(WAVE_ISR)
{
#if (WAVE_MODE_SLAVE_ENABLED)
   uint8  tmp8;
#endif  /* (WAVE_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;
    
#ifdef WAVE_ISR_ENTRY_CALLBACK
    WAVE_ISR_EntryCallback();
#endif /* WAVE_ISR_ENTRY_CALLBACK */
    

#if(WAVE_TIMEOUT_FF_ENABLED)
    if(0u != WAVE_TimeoutGetStatus())
    {
        WAVE_TimeoutReset();
        WAVE_state = WAVE_SM_EXIT_IDLE;
        /* WAVE_CSR_REG should be cleared after reset */
    }
#endif /* (WAVE_TIMEOUT_FF_ENABLED) */


    tmpCsr = WAVE_CSR_REG;      /* Make copy as interrupts clear */

#if(WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED)
    if(WAVE_CHECK_START_GEN(WAVE_MCSR_REG))
    {
        WAVE_CLEAR_START_GEN;

        /* Set transfer complete and error flags */
        WAVE_mstrStatus |= (WAVE_MSTAT_ERR_XFER |
                                        WAVE_GET_MSTAT_CMPLT);

        /* Slave was addressed */
        WAVE_state = WAVE_SM_SLAVE;
    }
#endif /* (WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED) */


#if(WAVE_MODE_MULTI_MASTER_ENABLED)
    if(WAVE_CHECK_LOST_ARB(tmpCsr))
    {
        /* Set errors */
        WAVE_mstrStatus |= (WAVE_MSTAT_ERR_XFER     |
                                        WAVE_MSTAT_ERR_ARB_LOST |
                                        WAVE_GET_MSTAT_CMPLT);

        WAVE_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

        #if(WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(WAVE_CHECK_ADDRESS_STS(tmpCsr))
            {
                /* Slave was addressed */
                WAVE_state = WAVE_SM_SLAVE;
            }
            else
            {
                WAVE_BUS_RELEASE;

                WAVE_state = WAVE_SM_EXIT_IDLE;
            }
        #else
            WAVE_BUS_RELEASE;

            WAVE_state = WAVE_SM_EXIT_IDLE;

        #endif /* (WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED) */
    }
#endif /* (WAVE_MODE_MULTI_MASTER_ENABLED) */

    /* Check for master operation mode */
    if(WAVE_CHECK_SM_MASTER)
    {
    #if(WAVE_MODE_MASTER_ENABLED)
        if(WAVE_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            switch (WAVE_state)
            {
            case WAVE_SM_MSTR_WR_ADDR:  /* After address is sent, write data */
            case WAVE_SM_MSTR_RD_ADDR:  /* After address is sent, read data */

                tmpCsr &= ((uint8) ~WAVE_CSR_STOP_STATUS); /* Clear Stop bit history on address phase */

                if(WAVE_CHECK_ADDR_ACK(tmpCsr))
                {
                    /* Setup for transmit or receive of data */
                    if(WAVE_state == WAVE_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                    {
                        /* Check if at least one byte to transfer */
                        if(WAVE_mstrWrBufSize > 0u)
                        {
                            /* Load the 1st data byte */
                            WAVE_DATA_REG = WAVE_mstrWrBufPtr[0u];
                            WAVE_TRANSMIT_DATA;
                            WAVE_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                            /* Set transmit state until done */
                            WAVE_state = WAVE_SM_MSTR_WR_DATA;
                        }
                        /* End of buffer: complete writing */
                        else if(WAVE_CHECK_NO_STOP(WAVE_mstrControl))
                        {
                            /* Set write complete and master halted */
                            WAVE_mstrStatus |= (WAVE_MSTAT_XFER_HALT |
                                                            WAVE_MSTAT_WR_CMPLT);

                            WAVE_state = WAVE_SM_MSTR_HALT; /* Expect ReStart */
                            WAVE_DisableInt();
                        }
                        else
                        {
                            WAVE_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                            WAVE_GENERATE_STOP;
                        }
                    }
                    else  /* Master receive data */
                    {
                        WAVE_READY_TO_READ; /* Release bus to read data */

                        WAVE_state  = WAVE_SM_MSTR_RD_DATA;
                    }
                }
                /* Address is NACKed */
                else if(WAVE_CHECK_ADDR_NAK(tmpCsr))
                {
                    /* Set Address NAK error */
                    WAVE_mstrStatus |= (WAVE_MSTAT_ERR_XFER |
                                                    WAVE_MSTAT_ERR_ADDR_NAK);

                    if(WAVE_CHECK_NO_STOP(WAVE_mstrControl))
                    {
                        WAVE_mstrStatus |= (WAVE_MSTAT_XFER_HALT |
                                                        WAVE_GET_MSTAT_CMPLT);

                        WAVE_state = WAVE_SM_MSTR_HALT; /* Expect RESTART */
                        WAVE_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        WAVE_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                        WAVE_GENERATE_STOP;
                    }
                }
                else
                {
                    /* Address phase is not set for some reason: error */
                    #if(WAVE_TIMEOUT_ENABLED)
                        /* Exit interrupt to take chance for timeout timer to handle this case */
                        WAVE_DisableInt();
                        WAVE_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (WAVE_TIMEOUT_ENABLED) */
                }
                break;

            case WAVE_SM_MSTR_WR_DATA:

                if(WAVE_CHECK_DATA_ACK(tmpCsr))
                {
                    /* Check if end of buffer */
                    if(WAVE_mstrWrBufIndex  < WAVE_mstrWrBufSize)
                    {
                        WAVE_DATA_REG =
                                                 WAVE_mstrWrBufPtr[WAVE_mstrWrBufIndex];
                        WAVE_TRANSMIT_DATA;
                        WAVE_mstrWrBufIndex++;
                    }
                    /* End of buffer: complete writing */
                    else if(WAVE_CHECK_NO_STOP(WAVE_mstrControl))
                    {
                        /* Set write complete and master halted */
                        WAVE_mstrStatus |= (WAVE_MSTAT_XFER_HALT |
                                                        WAVE_MSTAT_WR_CMPLT);

                        WAVE_state = WAVE_SM_MSTR_HALT;    /* Expect restart */
                        WAVE_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        WAVE_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                        WAVE_GENERATE_STOP;
                    }
                }
                /* Last byte NAKed: end writing */
                else if(WAVE_CHECK_NO_STOP(WAVE_mstrControl))
                {
                    /* Set write complete, short transfer and master halted */
                    WAVE_mstrStatus |= (WAVE_MSTAT_ERR_XFER       |
                                                    WAVE_MSTAT_ERR_SHORT_XFER |
                                                    WAVE_MSTAT_XFER_HALT      |
                                                    WAVE_MSTAT_WR_CMPLT);

                    WAVE_state = WAVE_SM_MSTR_HALT;    /* Expect ReStart */
                    WAVE_DisableInt();
                }
                else  /* Do normal Stop */
                {
                    WAVE_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                    WAVE_GENERATE_STOP;

                    /* Set short transfer and error flag */
                    WAVE_mstrStatus |= (WAVE_MSTAT_ERR_SHORT_XFER |
                                                    WAVE_MSTAT_ERR_XFER);
                }

                break;

            case WAVE_SM_MSTR_RD_DATA:

                WAVE_mstrRdBufPtr[WAVE_mstrRdBufIndex] = WAVE_DATA_REG;
                WAVE_mstrRdBufIndex++;

                /* Check if end of buffer */
                if(WAVE_mstrRdBufIndex < WAVE_mstrRdBufSize)
                {
                    WAVE_ACK_AND_RECEIVE;       /* ACK and receive byte */
                }
                /* End of buffer: complete reading */
                else if(WAVE_CHECK_NO_STOP(WAVE_mstrControl))
                {
                    /* Set read complete and master halted */
                    WAVE_mstrStatus |= (WAVE_MSTAT_XFER_HALT |
                                                    WAVE_MSTAT_RD_CMPLT);

                    WAVE_state = WAVE_SM_MSTR_HALT;    /* Expect ReStart */
                    WAVE_DisableInt();
                }
                else
                {
                    WAVE_ENABLE_INT_ON_STOP;
                    WAVE_NAK_AND_RECEIVE;       /* NACK and TRY to generate Stop */
                }
                break;

            default: /* This is an invalid state and should not occur */

            #if(WAVE_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handles this case */
                WAVE_DisableInt();
                WAVE_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (WAVE_TIMEOUT_ENABLED) */

                break;
            }
        }

        /* Catches Stop: end of transaction */
        if(WAVE_CHECK_STOP_STS(tmpCsr))
        {
            WAVE_mstrStatus |= WAVE_GET_MSTAT_CMPLT;

            WAVE_DISABLE_INT_ON_STOP;
            WAVE_state = WAVE_SM_IDLE;
        }
    #endif /* (WAVE_MODE_MASTER_ENABLED) */
    }
    else if(WAVE_CHECK_SM_SLAVE)
    {
    #if(WAVE_MODE_SLAVE_ENABLED)

        if((WAVE_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
           (WAVE_CHECK_BYTE_COMPLETE(tmpCsr) && WAVE_CHECK_ADDRESS_STS(tmpCsr)))
        {
            /* Catch end of master write transaction: use interrupt on Stop */
            /* The Stop bit history on address phase does not have correct state */
            if(WAVE_SM_SL_WR_DATA == WAVE_state)
            {
                WAVE_DISABLE_INT_ON_STOP;

                WAVE_slStatus &= ((uint8) ~WAVE_SSTAT_WR_BUSY);
                WAVE_slStatus |= ((uint8)  WAVE_SSTAT_WR_CMPLT);

                WAVE_state = WAVE_SM_IDLE;
            }
        }

        if(WAVE_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            /* The address only issued after Start or ReStart: so check the address
               to catch these events:
                FF : sets an address phase with a byte_complete interrupt trigger.
                UDB: sets an address phase immediately after Start or ReStart. */
            if(WAVE_CHECK_ADDRESS_STS(tmpCsr))
            {
            /* Check for software address detection */
            #if(WAVE_SW_ADRR_DECODE)
                tmp8 = WAVE_GET_SLAVE_ADDR(WAVE_DATA_REG);

                if(tmp8 == WAVE_slAddress)   /* Check for address match */
                {
                    if(0u != (WAVE_DATA_REG & WAVE_READ_FLAG))
                    {
                        /* Place code to prepare read buffer here                  */
                        /* `#START WAVE_SW_PREPARE_READ_BUF_interrupt` */

                        /* `#END` */

                    #ifdef WAVE_SW_PREPARE_READ_BUF_CALLBACK
                        WAVE_SwPrepareReadBuf_Callback();
                    #endif /* WAVE_SW_PREPARE_READ_BUF_CALLBACK */
                        
                        /* Prepare next operation to read, get data and place in data register */
                        if(WAVE_slRdBufIndex < WAVE_slRdBufSize)
                        {
                            /* Load first data byte from array */
                            WAVE_DATA_REG = WAVE_slRdBufPtr[WAVE_slRdBufIndex];
                            WAVE_ACK_AND_TRANSMIT;
                            WAVE_slRdBufIndex++;

                            WAVE_slStatus |= WAVE_SSTAT_RD_BUSY;
                        }
                        else    /* Overflow: provide 0xFF on bus */
                        {
                            WAVE_DATA_REG = WAVE_OVERFLOW_RETURN;
                            WAVE_ACK_AND_TRANSMIT;

                            WAVE_slStatus  |= (WAVE_SSTAT_RD_BUSY |
                                                           WAVE_SSTAT_RD_ERR_OVFL);
                        }

                        WAVE_state = WAVE_SM_SL_RD_DATA;
                    }
                    else  /* Write transaction: receive 1st byte */
                    {
                        WAVE_ACK_AND_RECEIVE;
                        WAVE_state = WAVE_SM_SL_WR_DATA;

                        WAVE_slStatus |= WAVE_SSTAT_WR_BUSY;
                        WAVE_ENABLE_INT_ON_STOP;
                    }
                }
                else
                {
                    /*     Place code to compare for additional address here    */
                    /* `#START WAVE_SW_ADDR_COMPARE_interruptStart` */

                    /* `#END` */

                #ifdef WAVE_SW_ADDR_COMPARE_ENTRY_CALLBACK
                    WAVE_SwAddrCompare_EntryCallback();
                #endif /* WAVE_SW_ADDR_COMPARE_ENTRY_CALLBACK */
                    
                    WAVE_NAK_AND_RECEIVE;   /* NACK address */

                    /* Place code to end of condition for NACK generation here */
                    /* `#START WAVE_SW_ADDR_COMPARE_interruptEnd`  */

                    /* `#END` */

                #ifdef WAVE_SW_ADDR_COMPARE_EXIT_CALLBACK
                    WAVE_SwAddrCompare_ExitCallback();
                #endif /* WAVE_SW_ADDR_COMPARE_EXIT_CALLBACK */
                }

            #else /* (WAVE_HW_ADRR_DECODE) */

                if(0u != (WAVE_DATA_REG & WAVE_READ_FLAG))
                {
                    /* Place code to prepare read buffer here                  */
                    /* `#START WAVE_HW_PREPARE_READ_BUF_interrupt` */

                    /* `#END` */
                    
                #ifdef WAVE_HW_PREPARE_READ_BUF_CALLBACK
                    WAVE_HwPrepareReadBuf_Callback();
                #endif /* WAVE_HW_PREPARE_READ_BUF_CALLBACK */

                    /* Prepare next operation to read, get data and place in data register */
                    if(WAVE_slRdBufIndex < WAVE_slRdBufSize)
                    {
                        /* Load first data byte from array */
                        WAVE_DATA_REG = WAVE_slRdBufPtr[WAVE_slRdBufIndex];
                        WAVE_ACK_AND_TRANSMIT;
                        WAVE_slRdBufIndex++;

                        WAVE_slStatus |= WAVE_SSTAT_RD_BUSY;
                    }
                    else    /* Overflow: provide 0xFF on bus */
                    {
                        WAVE_DATA_REG = WAVE_OVERFLOW_RETURN;
                        WAVE_ACK_AND_TRANSMIT;

                        WAVE_slStatus  |= (WAVE_SSTAT_RD_BUSY |
                                                       WAVE_SSTAT_RD_ERR_OVFL);
                    }

                    WAVE_state = WAVE_SM_SL_RD_DATA;
                }
                else  /* Write transaction: receive 1st byte */
                {
                    WAVE_ACK_AND_RECEIVE;
                    WAVE_state = WAVE_SM_SL_WR_DATA;

                    WAVE_slStatus |= WAVE_SSTAT_WR_BUSY;
                    WAVE_ENABLE_INT_ON_STOP;
                }

            #endif /* (WAVE_SW_ADRR_DECODE) */
            }
            /* Data states */
            /* Data master writes into slave */
            else if(WAVE_state == WAVE_SM_SL_WR_DATA)
            {
                if(WAVE_slWrBufIndex < WAVE_slWrBufSize)
                {
                    tmp8 = WAVE_DATA_REG;
                    WAVE_ACK_AND_RECEIVE;
                    WAVE_slWrBufPtr[WAVE_slWrBufIndex] = tmp8;
                    WAVE_slWrBufIndex++;
                }
                else  /* of array: complete write, send NACK */
                {
                    WAVE_NAK_AND_RECEIVE;

                    WAVE_slStatus |= WAVE_SSTAT_WR_ERR_OVFL;
                }
            }
            /* Data master reads from slave */
            else if(WAVE_state == WAVE_SM_SL_RD_DATA)
            {
                if(WAVE_CHECK_DATA_ACK(tmpCsr))
                {
                    if(WAVE_slRdBufIndex < WAVE_slRdBufSize)
                    {
                         /* Get data from array */
                        WAVE_DATA_REG = WAVE_slRdBufPtr[WAVE_slRdBufIndex];
                        WAVE_TRANSMIT_DATA;
                        WAVE_slRdBufIndex++;
                    }
                    else   /* Overflow: provide 0xFF on bus */
                    {
                        WAVE_DATA_REG = WAVE_OVERFLOW_RETURN;
                        WAVE_TRANSMIT_DATA;

                        WAVE_slStatus |= WAVE_SSTAT_RD_ERR_OVFL;
                    }
                }
                else  /* Last byte was NACKed: read complete */
                {
                    /* Only NACK appears on bus */
                    WAVE_DATA_REG = WAVE_OVERFLOW_RETURN;
                    WAVE_NAK_AND_TRANSMIT;

                    WAVE_slStatus &= ((uint8) ~WAVE_SSTAT_RD_BUSY);
                    WAVE_slStatus |= ((uint8)  WAVE_SSTAT_RD_CMPLT);

                    WAVE_state = WAVE_SM_IDLE;
                }
            }
            else
            {
            #if(WAVE_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handle this case */
                WAVE_DisableInt();
                WAVE_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (WAVE_TIMEOUT_ENABLED) */
            }
        }
    #endif /* (WAVE_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        WAVE_state = WAVE_SM_IDLE;
    }

#ifdef WAVE_ISR_EXIT_CALLBACK
    WAVE_ISR_ExitCallback();
#endif /* WAVE_ISR_EXIT_CALLBACK */    
}


#if ((WAVE_FF_IMPLEMENTED) && (WAVE_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: WAVE_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  The interrupt handler to trigger after a wakeup.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(WAVE_WAKEUP_ISR)
    {
    #ifdef WAVE_WAKEUP_ISR_ENTRY_CALLBACK
        WAVE_WAKEUP_ISR_EntryCallback();
    #endif /* WAVE_WAKEUP_ISR_ENTRY_CALLBACK */
         
        /* Set flag to notify that matched address is received */
        WAVE_wakeupSource = 1u;

        /* SCL is stretched until the I2C_Wake() is called */

    #ifdef WAVE_WAKEUP_ISR_EXIT_CALLBACK
        WAVE_WAKEUP_ISR_ExitCallback();
    #endif /* WAVE_WAKEUP_ISR_EXIT_CALLBACK */
    }
#endif /* ((WAVE_FF_IMPLEMENTED) && (WAVE_WAKEUP_ENABLED)) */


/* [] END OF FILE */

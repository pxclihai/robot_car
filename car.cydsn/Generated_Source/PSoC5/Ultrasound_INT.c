/*******************************************************************************
* File Name: Ultrasound_INT.c
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

#include "Ultrasound_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here.
********************************************************************************/
/* `#START Ultrasound_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: Ultrasound_ISR
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
CY_ISR(Ultrasound_ISR)
{
#if (Ultrasound_MODE_SLAVE_ENABLED)
   uint8  tmp8;
#endif  /* (Ultrasound_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;
    
#ifdef Ultrasound_ISR_ENTRY_CALLBACK
    Ultrasound_ISR_EntryCallback();
#endif /* Ultrasound_ISR_ENTRY_CALLBACK */
    

#if(Ultrasound_TIMEOUT_FF_ENABLED)
    if(0u != Ultrasound_TimeoutGetStatus())
    {
        Ultrasound_TimeoutReset();
        Ultrasound_state = Ultrasound_SM_EXIT_IDLE;
        /* Ultrasound_CSR_REG should be cleared after reset */
    }
#endif /* (Ultrasound_TIMEOUT_FF_ENABLED) */


    tmpCsr = Ultrasound_CSR_REG;      /* Make copy as interrupts clear */

#if(Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED)
    if(Ultrasound_CHECK_START_GEN(Ultrasound_MCSR_REG))
    {
        Ultrasound_CLEAR_START_GEN;

        /* Set transfer complete and error flags */
        Ultrasound_mstrStatus |= (Ultrasound_MSTAT_ERR_XFER |
                                        Ultrasound_GET_MSTAT_CMPLT);

        /* Slave was addressed */
        Ultrasound_state = Ultrasound_SM_SLAVE;
    }
#endif /* (Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED) */


#if(Ultrasound_MODE_MULTI_MASTER_ENABLED)
    if(Ultrasound_CHECK_LOST_ARB(tmpCsr))
    {
        /* Set errors */
        Ultrasound_mstrStatus |= (Ultrasound_MSTAT_ERR_XFER     |
                                        Ultrasound_MSTAT_ERR_ARB_LOST |
                                        Ultrasound_GET_MSTAT_CMPLT);

        Ultrasound_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

        #if(Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(Ultrasound_CHECK_ADDRESS_STS(tmpCsr))
            {
                /* Slave was addressed */
                Ultrasound_state = Ultrasound_SM_SLAVE;
            }
            else
            {
                Ultrasound_BUS_RELEASE;

                Ultrasound_state = Ultrasound_SM_EXIT_IDLE;
            }
        #else
            Ultrasound_BUS_RELEASE;

            Ultrasound_state = Ultrasound_SM_EXIT_IDLE;

        #endif /* (Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED) */
    }
#endif /* (Ultrasound_MODE_MULTI_MASTER_ENABLED) */

    /* Check for master operation mode */
    if(Ultrasound_CHECK_SM_MASTER)
    {
    #if(Ultrasound_MODE_MASTER_ENABLED)
        if(Ultrasound_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            switch (Ultrasound_state)
            {
            case Ultrasound_SM_MSTR_WR_ADDR:  /* After address is sent, write data */
            case Ultrasound_SM_MSTR_RD_ADDR:  /* After address is sent, read data */

                tmpCsr &= ((uint8) ~Ultrasound_CSR_STOP_STATUS); /* Clear Stop bit history on address phase */

                if(Ultrasound_CHECK_ADDR_ACK(tmpCsr))
                {
                    /* Setup for transmit or receive of data */
                    if(Ultrasound_state == Ultrasound_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                    {
                        /* Check if at least one byte to transfer */
                        if(Ultrasound_mstrWrBufSize > 0u)
                        {
                            /* Load the 1st data byte */
                            Ultrasound_DATA_REG = Ultrasound_mstrWrBufPtr[0u];
                            Ultrasound_TRANSMIT_DATA;
                            Ultrasound_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                            /* Set transmit state until done */
                            Ultrasound_state = Ultrasound_SM_MSTR_WR_DATA;
                        }
                        /* End of buffer: complete writing */
                        else if(Ultrasound_CHECK_NO_STOP(Ultrasound_mstrControl))
                        {
                            /* Set write complete and master halted */
                            Ultrasound_mstrStatus |= (Ultrasound_MSTAT_XFER_HALT |
                                                            Ultrasound_MSTAT_WR_CMPLT);

                            Ultrasound_state = Ultrasound_SM_MSTR_HALT; /* Expect ReStart */
                            Ultrasound_DisableInt();
                        }
                        else
                        {
                            Ultrasound_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                            Ultrasound_GENERATE_STOP;
                        }
                    }
                    else  /* Master receive data */
                    {
                        Ultrasound_READY_TO_READ; /* Release bus to read data */

                        Ultrasound_state  = Ultrasound_SM_MSTR_RD_DATA;
                    }
                }
                /* Address is NACKed */
                else if(Ultrasound_CHECK_ADDR_NAK(tmpCsr))
                {
                    /* Set Address NAK error */
                    Ultrasound_mstrStatus |= (Ultrasound_MSTAT_ERR_XFER |
                                                    Ultrasound_MSTAT_ERR_ADDR_NAK);

                    if(Ultrasound_CHECK_NO_STOP(Ultrasound_mstrControl))
                    {
                        Ultrasound_mstrStatus |= (Ultrasound_MSTAT_XFER_HALT |
                                                        Ultrasound_GET_MSTAT_CMPLT);

                        Ultrasound_state = Ultrasound_SM_MSTR_HALT; /* Expect RESTART */
                        Ultrasound_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        Ultrasound_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                        Ultrasound_GENERATE_STOP;
                    }
                }
                else
                {
                    /* Address phase is not set for some reason: error */
                    #if(Ultrasound_TIMEOUT_ENABLED)
                        /* Exit interrupt to take chance for timeout timer to handle this case */
                        Ultrasound_DisableInt();
                        Ultrasound_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (Ultrasound_TIMEOUT_ENABLED) */
                }
                break;

            case Ultrasound_SM_MSTR_WR_DATA:

                if(Ultrasound_CHECK_DATA_ACK(tmpCsr))
                {
                    /* Check if end of buffer */
                    if(Ultrasound_mstrWrBufIndex  < Ultrasound_mstrWrBufSize)
                    {
                        Ultrasound_DATA_REG =
                                                 Ultrasound_mstrWrBufPtr[Ultrasound_mstrWrBufIndex];
                        Ultrasound_TRANSMIT_DATA;
                        Ultrasound_mstrWrBufIndex++;
                    }
                    /* End of buffer: complete writing */
                    else if(Ultrasound_CHECK_NO_STOP(Ultrasound_mstrControl))
                    {
                        /* Set write complete and master halted */
                        Ultrasound_mstrStatus |= (Ultrasound_MSTAT_XFER_HALT |
                                                        Ultrasound_MSTAT_WR_CMPLT);

                        Ultrasound_state = Ultrasound_SM_MSTR_HALT;    /* Expect restart */
                        Ultrasound_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        Ultrasound_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                        Ultrasound_GENERATE_STOP;
                    }
                }
                /* Last byte NAKed: end writing */
                else if(Ultrasound_CHECK_NO_STOP(Ultrasound_mstrControl))
                {
                    /* Set write complete, short transfer and master halted */
                    Ultrasound_mstrStatus |= (Ultrasound_MSTAT_ERR_XFER       |
                                                    Ultrasound_MSTAT_ERR_SHORT_XFER |
                                                    Ultrasound_MSTAT_XFER_HALT      |
                                                    Ultrasound_MSTAT_WR_CMPLT);

                    Ultrasound_state = Ultrasound_SM_MSTR_HALT;    /* Expect ReStart */
                    Ultrasound_DisableInt();
                }
                else  /* Do normal Stop */
                {
                    Ultrasound_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                    Ultrasound_GENERATE_STOP;

                    /* Set short transfer and error flag */
                    Ultrasound_mstrStatus |= (Ultrasound_MSTAT_ERR_SHORT_XFER |
                                                    Ultrasound_MSTAT_ERR_XFER);
                }

                break;

            case Ultrasound_SM_MSTR_RD_DATA:

                Ultrasound_mstrRdBufPtr[Ultrasound_mstrRdBufIndex] = Ultrasound_DATA_REG;
                Ultrasound_mstrRdBufIndex++;

                /* Check if end of buffer */
                if(Ultrasound_mstrRdBufIndex < Ultrasound_mstrRdBufSize)
                {
                    Ultrasound_ACK_AND_RECEIVE;       /* ACK and receive byte */
                }
                /* End of buffer: complete reading */
                else if(Ultrasound_CHECK_NO_STOP(Ultrasound_mstrControl))
                {
                    /* Set read complete and master halted */
                    Ultrasound_mstrStatus |= (Ultrasound_MSTAT_XFER_HALT |
                                                    Ultrasound_MSTAT_RD_CMPLT);

                    Ultrasound_state = Ultrasound_SM_MSTR_HALT;    /* Expect ReStart */
                    Ultrasound_DisableInt();
                }
                else
                {
                    Ultrasound_ENABLE_INT_ON_STOP;
                    Ultrasound_NAK_AND_RECEIVE;       /* NACK and TRY to generate Stop */
                }
                break;

            default: /* This is an invalid state and should not occur */

            #if(Ultrasound_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handles this case */
                Ultrasound_DisableInt();
                Ultrasound_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (Ultrasound_TIMEOUT_ENABLED) */

                break;
            }
        }

        /* Catches Stop: end of transaction */
        if(Ultrasound_CHECK_STOP_STS(tmpCsr))
        {
            Ultrasound_mstrStatus |= Ultrasound_GET_MSTAT_CMPLT;

            Ultrasound_DISABLE_INT_ON_STOP;
            Ultrasound_state = Ultrasound_SM_IDLE;
        }
    #endif /* (Ultrasound_MODE_MASTER_ENABLED) */
    }
    else if(Ultrasound_CHECK_SM_SLAVE)
    {
    #if(Ultrasound_MODE_SLAVE_ENABLED)

        if((Ultrasound_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
           (Ultrasound_CHECK_BYTE_COMPLETE(tmpCsr) && Ultrasound_CHECK_ADDRESS_STS(tmpCsr)))
        {
            /* Catch end of master write transaction: use interrupt on Stop */
            /* The Stop bit history on address phase does not have correct state */
            if(Ultrasound_SM_SL_WR_DATA == Ultrasound_state)
            {
                Ultrasound_DISABLE_INT_ON_STOP;

                Ultrasound_slStatus &= ((uint8) ~Ultrasound_SSTAT_WR_BUSY);
                Ultrasound_slStatus |= ((uint8)  Ultrasound_SSTAT_WR_CMPLT);

                Ultrasound_state = Ultrasound_SM_IDLE;
            }
        }

        if(Ultrasound_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            /* The address only issued after Start or ReStart: so check the address
               to catch these events:
                FF : sets an address phase with a byte_complete interrupt trigger.
                UDB: sets an address phase immediately after Start or ReStart. */
            if(Ultrasound_CHECK_ADDRESS_STS(tmpCsr))
            {
            /* Check for software address detection */
            #if(Ultrasound_SW_ADRR_DECODE)
                tmp8 = Ultrasound_GET_SLAVE_ADDR(Ultrasound_DATA_REG);

                if(tmp8 == Ultrasound_slAddress)   /* Check for address match */
                {
                    if(0u != (Ultrasound_DATA_REG & Ultrasound_READ_FLAG))
                    {
                        /* Place code to prepare read buffer here                  */
                        /* `#START Ultrasound_SW_PREPARE_READ_BUF_interrupt` */

                        /* `#END` */

                    #ifdef Ultrasound_SW_PREPARE_READ_BUF_CALLBACK
                        Ultrasound_SwPrepareReadBuf_Callback();
                    #endif /* Ultrasound_SW_PREPARE_READ_BUF_CALLBACK */
                        
                        /* Prepare next operation to read, get data and place in data register */
                        if(Ultrasound_slRdBufIndex < Ultrasound_slRdBufSize)
                        {
                            /* Load first data byte from array */
                            Ultrasound_DATA_REG = Ultrasound_slRdBufPtr[Ultrasound_slRdBufIndex];
                            Ultrasound_ACK_AND_TRANSMIT;
                            Ultrasound_slRdBufIndex++;

                            Ultrasound_slStatus |= Ultrasound_SSTAT_RD_BUSY;
                        }
                        else    /* Overflow: provide 0xFF on bus */
                        {
                            Ultrasound_DATA_REG = Ultrasound_OVERFLOW_RETURN;
                            Ultrasound_ACK_AND_TRANSMIT;

                            Ultrasound_slStatus  |= (Ultrasound_SSTAT_RD_BUSY |
                                                           Ultrasound_SSTAT_RD_ERR_OVFL);
                        }

                        Ultrasound_state = Ultrasound_SM_SL_RD_DATA;
                    }
                    else  /* Write transaction: receive 1st byte */
                    {
                        Ultrasound_ACK_AND_RECEIVE;
                        Ultrasound_state = Ultrasound_SM_SL_WR_DATA;

                        Ultrasound_slStatus |= Ultrasound_SSTAT_WR_BUSY;
                        Ultrasound_ENABLE_INT_ON_STOP;
                    }
                }
                else
                {
                    /*     Place code to compare for additional address here    */
                    /* `#START Ultrasound_SW_ADDR_COMPARE_interruptStart` */

                    /* `#END` */

                #ifdef Ultrasound_SW_ADDR_COMPARE_ENTRY_CALLBACK
                    Ultrasound_SwAddrCompare_EntryCallback();
                #endif /* Ultrasound_SW_ADDR_COMPARE_ENTRY_CALLBACK */
                    
                    Ultrasound_NAK_AND_RECEIVE;   /* NACK address */

                    /* Place code to end of condition for NACK generation here */
                    /* `#START Ultrasound_SW_ADDR_COMPARE_interruptEnd`  */

                    /* `#END` */

                #ifdef Ultrasound_SW_ADDR_COMPARE_EXIT_CALLBACK
                    Ultrasound_SwAddrCompare_ExitCallback();
                #endif /* Ultrasound_SW_ADDR_COMPARE_EXIT_CALLBACK */
                }

            #else /* (Ultrasound_HW_ADRR_DECODE) */

                if(0u != (Ultrasound_DATA_REG & Ultrasound_READ_FLAG))
                {
                    /* Place code to prepare read buffer here                  */
                    /* `#START Ultrasound_HW_PREPARE_READ_BUF_interrupt` */

                    /* `#END` */
                    
                #ifdef Ultrasound_HW_PREPARE_READ_BUF_CALLBACK
                    Ultrasound_HwPrepareReadBuf_Callback();
                #endif /* Ultrasound_HW_PREPARE_READ_BUF_CALLBACK */

                    /* Prepare next operation to read, get data and place in data register */
                    if(Ultrasound_slRdBufIndex < Ultrasound_slRdBufSize)
                    {
                        /* Load first data byte from array */
                        Ultrasound_DATA_REG = Ultrasound_slRdBufPtr[Ultrasound_slRdBufIndex];
                        Ultrasound_ACK_AND_TRANSMIT;
                        Ultrasound_slRdBufIndex++;

                        Ultrasound_slStatus |= Ultrasound_SSTAT_RD_BUSY;
                    }
                    else    /* Overflow: provide 0xFF on bus */
                    {
                        Ultrasound_DATA_REG = Ultrasound_OVERFLOW_RETURN;
                        Ultrasound_ACK_AND_TRANSMIT;

                        Ultrasound_slStatus  |= (Ultrasound_SSTAT_RD_BUSY |
                                                       Ultrasound_SSTAT_RD_ERR_OVFL);
                    }

                    Ultrasound_state = Ultrasound_SM_SL_RD_DATA;
                }
                else  /* Write transaction: receive 1st byte */
                {
                    Ultrasound_ACK_AND_RECEIVE;
                    Ultrasound_state = Ultrasound_SM_SL_WR_DATA;

                    Ultrasound_slStatus |= Ultrasound_SSTAT_WR_BUSY;
                    Ultrasound_ENABLE_INT_ON_STOP;
                }

            #endif /* (Ultrasound_SW_ADRR_DECODE) */
            }
            /* Data states */
            /* Data master writes into slave */
            else if(Ultrasound_state == Ultrasound_SM_SL_WR_DATA)
            {
                if(Ultrasound_slWrBufIndex < Ultrasound_slWrBufSize)
                {
                    tmp8 = Ultrasound_DATA_REG;
                    Ultrasound_ACK_AND_RECEIVE;
                    Ultrasound_slWrBufPtr[Ultrasound_slWrBufIndex] = tmp8;
                    Ultrasound_slWrBufIndex++;
                }
                else  /* of array: complete write, send NACK */
                {
                    Ultrasound_NAK_AND_RECEIVE;

                    Ultrasound_slStatus |= Ultrasound_SSTAT_WR_ERR_OVFL;
                }
            }
            /* Data master reads from slave */
            else if(Ultrasound_state == Ultrasound_SM_SL_RD_DATA)
            {
                if(Ultrasound_CHECK_DATA_ACK(tmpCsr))
                {
                    if(Ultrasound_slRdBufIndex < Ultrasound_slRdBufSize)
                    {
                         /* Get data from array */
                        Ultrasound_DATA_REG = Ultrasound_slRdBufPtr[Ultrasound_slRdBufIndex];
                        Ultrasound_TRANSMIT_DATA;
                        Ultrasound_slRdBufIndex++;
                    }
                    else   /* Overflow: provide 0xFF on bus */
                    {
                        Ultrasound_DATA_REG = Ultrasound_OVERFLOW_RETURN;
                        Ultrasound_TRANSMIT_DATA;

                        Ultrasound_slStatus |= Ultrasound_SSTAT_RD_ERR_OVFL;
                    }
                }
                else  /* Last byte was NACKed: read complete */
                {
                    /* Only NACK appears on bus */
                    Ultrasound_DATA_REG = Ultrasound_OVERFLOW_RETURN;
                    Ultrasound_NAK_AND_TRANSMIT;

                    Ultrasound_slStatus &= ((uint8) ~Ultrasound_SSTAT_RD_BUSY);
                    Ultrasound_slStatus |= ((uint8)  Ultrasound_SSTAT_RD_CMPLT);

                    Ultrasound_state = Ultrasound_SM_IDLE;
                }
            }
            else
            {
            #if(Ultrasound_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handle this case */
                Ultrasound_DisableInt();
                Ultrasound_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (Ultrasound_TIMEOUT_ENABLED) */
            }
        }
    #endif /* (Ultrasound_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        Ultrasound_state = Ultrasound_SM_IDLE;
    }

#ifdef Ultrasound_ISR_EXIT_CALLBACK
    Ultrasound_ISR_ExitCallback();
#endif /* Ultrasound_ISR_EXIT_CALLBACK */    
}


#if ((Ultrasound_FF_IMPLEMENTED) && (Ultrasound_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: Ultrasound_WAKEUP_ISR
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
    CY_ISR(Ultrasound_WAKEUP_ISR)
    {
    #ifdef Ultrasound_WAKEUP_ISR_ENTRY_CALLBACK
        Ultrasound_WAKEUP_ISR_EntryCallback();
    #endif /* Ultrasound_WAKEUP_ISR_ENTRY_CALLBACK */
         
        /* Set flag to notify that matched address is received */
        Ultrasound_wakeupSource = 1u;

        /* SCL is stretched until the I2C_Wake() is called */

    #ifdef Ultrasound_WAKEUP_ISR_EXIT_CALLBACK
        Ultrasound_WAKEUP_ISR_ExitCallback();
    #endif /* Ultrasound_WAKEUP_ISR_EXIT_CALLBACK */
    }
#endif /* ((Ultrasound_FF_IMPLEMENTED) && (Ultrasound_WAKEUP_ENABLED)) */


/* [] END OF FILE */

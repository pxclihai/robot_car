/*******************************************************************************
* File Name: ECDR_INT.c
* Version 3.0
*
* Description:
*  This file contains the Interrupt Service Routine (ISR) for the Quadrature
*  Decoder component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ECDR.h"
#include "ECDR_PVT.h"
#include "cyapicallbacks.h"

volatile int32 ECDR_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void ECDR_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for Quadrature Decoder Component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  ECDR_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  ECDR_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( ECDR_ISR )
{
   uint8 ECDR_swStatus;

   ECDR_swStatus = ECDR_STATUS_REG;

    #ifdef ECDR_ISR_ENTRY_CALLBACK
        ECDR_ISR_EntryCallback();
    #endif /* ECDR_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START ECDR_ISR_START` */

    /* `#END` */

    if (0u != (ECDR_swStatus & ECDR_COUNTER_OVERFLOW))
    {
        ECDR_count32SoftPart += (int32) ECDR_COUNTER_MAX_VALUE;
    }
    else if (0u != (ECDR_swStatus & ECDR_COUNTER_UNDERFLOW))
    {
        ECDR_count32SoftPart -= (int32) ECDR_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (ECDR_swStatus & ECDR_COUNTER_RESET))
    {
        ECDR_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START ECDR_ISR_END` */

    /* `#END` */

    #ifdef ECDR_ISR_EXIT_CALLBACK
        ECDR_ISR_ExitCallback();
    #endif /* ECDR_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */

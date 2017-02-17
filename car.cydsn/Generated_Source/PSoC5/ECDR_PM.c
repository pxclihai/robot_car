/*******************************************************************************
* File Name: ECDR_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
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

static ECDR_BACKUP_STRUCT ECDR_backup = {0u};


/*******************************************************************************
* Function Name: ECDR_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ECDR_SaveConfig(void) 
{
    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
        ECDR_Cnt8_SaveConfig();
    #else 
        /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) || 
         * (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
         */
        ECDR_Cnt16_SaveConfig();
    #endif /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: ECDR_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ECDR_RestoreConfig(void) 
{
    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
        ECDR_Cnt8_RestoreConfig();
    #else 
        /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) || 
         * (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT) 
         */
        ECDR_Cnt16_RestoreConfig();
    #endif /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: ECDR_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ECDR_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ECDR_Sleep(void) 
{
    if (0u != (ECDR_SR_AUX_CONTROL & ECDR_INTERRUPTS_ENABLE))
    {
        ECDR_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        ECDR_backup.enableState = 0u;
    }

    ECDR_Stop();
    ECDR_SaveConfig();
}


/*******************************************************************************
* Function Name: ECDR_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ECDR_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void ECDR_Wakeup(void) 
{
    ECDR_RestoreConfig();

    if (ECDR_backup.enableState != 0u)
    {
        #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
            ECDR_Cnt8_Enable();
        #else 
            /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) || 
            *  (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT) 
            */
            ECDR_Cnt16_Enable();
        #endif /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        ECDR_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


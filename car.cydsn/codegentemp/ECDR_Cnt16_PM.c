/*******************************************************************************
* File Name: ECDR_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "ECDR_Cnt16.h"

static ECDR_Cnt16_backupStruct ECDR_Cnt16_backup;


/*******************************************************************************
* Function Name: ECDR_Cnt16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  ECDR_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void ECDR_Cnt16_SaveConfig(void) 
{
    #if (!ECDR_Cnt16_UsingFixedFunction)

        ECDR_Cnt16_backup.CounterUdb = ECDR_Cnt16_ReadCounter();

        #if(!ECDR_Cnt16_ControlRegRemoved)
            ECDR_Cnt16_backup.CounterControlRegister = ECDR_Cnt16_ReadControlRegister();
        #endif /* (!ECDR_Cnt16_ControlRegRemoved) */

    #endif /* (!ECDR_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: ECDR_Cnt16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  ECDR_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void ECDR_Cnt16_RestoreConfig(void) 
{      
    #if (!ECDR_Cnt16_UsingFixedFunction)

       ECDR_Cnt16_WriteCounter(ECDR_Cnt16_backup.CounterUdb);

        #if(!ECDR_Cnt16_ControlRegRemoved)
            ECDR_Cnt16_WriteControlRegister(ECDR_Cnt16_backup.CounterControlRegister);
        #endif /* (!ECDR_Cnt16_ControlRegRemoved) */

    #endif /* (!ECDR_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: ECDR_Cnt16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  ECDR_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void ECDR_Cnt16_Sleep(void) 
{
    #if(!ECDR_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(ECDR_Cnt16_CTRL_ENABLE == (ECDR_Cnt16_CONTROL & ECDR_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            ECDR_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            ECDR_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        ECDR_Cnt16_backup.CounterEnableState = 1u;
        if(ECDR_Cnt16_backup.CounterEnableState != 0u)
        {
            ECDR_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!ECDR_Cnt16_ControlRegRemoved) */
    
    ECDR_Cnt16_Stop();
    ECDR_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: ECDR_Cnt16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  ECDR_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ECDR_Cnt16_Wakeup(void) 
{
    ECDR_Cnt16_RestoreConfig();
    #if(!ECDR_Cnt16_ControlRegRemoved)
        if(ECDR_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            ECDR_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!ECDR_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */

/*******************************************************************************
* File Name: SRF_05_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "SRF_05_Timer.h"

static SRF_05_Timer_backupStruct SRF_05_Timer_backup;


/*******************************************************************************
* Function Name: SRF_05_Timer_SaveConfig
********************************************************************************
*
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
*  SRF_05_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void SRF_05_Timer_SaveConfig(void) 
{
    #if (!SRF_05_Timer_UsingFixedFunction)
        SRF_05_Timer_backup.TimerUdb = SRF_05_Timer_ReadCounter();
        SRF_05_Timer_backup.InterruptMaskValue = SRF_05_Timer_STATUS_MASK;
        #if (SRF_05_Timer_UsingHWCaptureCounter)
            SRF_05_Timer_backup.TimerCaptureCounter = SRF_05_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!SRF_05_Timer_UDB_CONTROL_REG_REMOVED)
            SRF_05_Timer_backup.TimerControlRegister = SRF_05_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: SRF_05_Timer_RestoreConfig
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
*  SRF_05_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SRF_05_Timer_RestoreConfig(void) 
{   
    #if (!SRF_05_Timer_UsingFixedFunction)

        SRF_05_Timer_WriteCounter(SRF_05_Timer_backup.TimerUdb);
        SRF_05_Timer_STATUS_MASK =SRF_05_Timer_backup.InterruptMaskValue;
        #if (SRF_05_Timer_UsingHWCaptureCounter)
            SRF_05_Timer_SetCaptureCount(SRF_05_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!SRF_05_Timer_UDB_CONTROL_REG_REMOVED)
            SRF_05_Timer_WriteControlRegister(SRF_05_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: SRF_05_Timer_Sleep
********************************************************************************
*
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
*  SRF_05_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void SRF_05_Timer_Sleep(void) 
{
    #if(!SRF_05_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(SRF_05_Timer_CTRL_ENABLE == (SRF_05_Timer_CONTROL & SRF_05_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            SRF_05_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            SRF_05_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    SRF_05_Timer_Stop();
    SRF_05_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: SRF_05_Timer_Wakeup
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
*  SRF_05_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SRF_05_Timer_Wakeup(void) 
{
    SRF_05_Timer_RestoreConfig();
    #if(!SRF_05_Timer_UDB_CONTROL_REG_REMOVED)
        if(SRF_05_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                SRF_05_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

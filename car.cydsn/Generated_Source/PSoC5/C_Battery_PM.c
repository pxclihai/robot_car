/*******************************************************************************
* File Name: C_Battery_PM.c
* Version 2.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "C_Battery.h"


/***************************************
* Local data allocation
***************************************/

static C_Battery_BACKUP_STRUCT  C_Battery_backup =
{
    C_Battery_DISABLED
};


/*******************************************************************************
* Function Name: C_Battery_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void C_Battery_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: C_Battery_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void C_Battery_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: C_Battery_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The C_Battery_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  C_Battery_backup - modified.
*
*******************************************************************************/
void C_Battery_Sleep(void)
{
    if((C_Battery_PWRMGR_SAR_REG  & C_Battery_ACT_PWR_SAR_EN) != 0u)
    {
        if((C_Battery_SAR_CSR0_REG & C_Battery_SAR_SOF_START_CONV) != 0u)
        {
            C_Battery_backup.enableState = C_Battery_ENABLED | C_Battery_STARTED;
        }
        else
        {
            C_Battery_backup.enableState = C_Battery_ENABLED;
        }
        C_Battery_Stop();
    }
    else
    {
        C_Battery_backup.enableState = C_Battery_DISABLED;
    }
}


/*******************************************************************************
* Function Name: C_Battery_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  C_Battery_Sleep() was called. If the component was enabled before the
*  C_Battery_Sleep() function was called, the
*  C_Battery_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  C_Battery_backup - used to check enabable state.
*
*******************************************************************************/
void C_Battery_Wakeup(void)
{
    if(C_Battery_backup.enableState != C_Battery_DISABLED)
    {
        C_Battery_Enable();
        #if(C_Battery_DEFAULT_CONV_MODE != C_Battery__HARDWARE_TRIGGER)
            if((C_Battery_backup.enableState & C_Battery_STARTED) != 0u)
            {
                C_Battery_StartConvert();
            }
        #endif /* End C_Battery_DEFAULT_CONV_MODE != C_Battery__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */

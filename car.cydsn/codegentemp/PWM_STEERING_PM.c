/*******************************************************************************
* File Name: PWM_STEERING_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_STEERING.h"

static PWM_STEERING_backupStruct PWM_STEERING_backup;


/*******************************************************************************
* Function Name: PWM_STEERING_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_STEERING_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_STEERING_SaveConfig(void) 
{

    #if(!PWM_STEERING_UsingFixedFunction)
        #if(!PWM_STEERING_PWMModeIsCenterAligned)
            PWM_STEERING_backup.PWMPeriod = PWM_STEERING_ReadPeriod();
        #endif /* (!PWM_STEERING_PWMModeIsCenterAligned) */
        PWM_STEERING_backup.PWMUdb = PWM_STEERING_ReadCounter();
        #if (PWM_STEERING_UseStatus)
            PWM_STEERING_backup.InterruptMaskValue = PWM_STEERING_STATUS_MASK;
        #endif /* (PWM_STEERING_UseStatus) */

        #if(PWM_STEERING_DeadBandMode == PWM_STEERING__B_PWM__DBM_256_CLOCKS || \
            PWM_STEERING_DeadBandMode == PWM_STEERING__B_PWM__DBM_2_4_CLOCKS)
            PWM_STEERING_backup.PWMdeadBandValue = PWM_STEERING_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_STEERING_KillModeMinTime)
             PWM_STEERING_backup.PWMKillCounterPeriod = PWM_STEERING_ReadKillTime();
        #endif /* (PWM_STEERING_KillModeMinTime) */

        #if(PWM_STEERING_UseControl)
            PWM_STEERING_backup.PWMControlRegister = PWM_STEERING_ReadControlRegister();
        #endif /* (PWM_STEERING_UseControl) */
    #endif  /* (!PWM_STEERING_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_STEERING_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_STEERING_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_STEERING_RestoreConfig(void) 
{
        #if(!PWM_STEERING_UsingFixedFunction)
            #if(!PWM_STEERING_PWMModeIsCenterAligned)
                PWM_STEERING_WritePeriod(PWM_STEERING_backup.PWMPeriod);
            #endif /* (!PWM_STEERING_PWMModeIsCenterAligned) */

            PWM_STEERING_WriteCounter(PWM_STEERING_backup.PWMUdb);

            #if (PWM_STEERING_UseStatus)
                PWM_STEERING_STATUS_MASK = PWM_STEERING_backup.InterruptMaskValue;
            #endif /* (PWM_STEERING_UseStatus) */

            #if(PWM_STEERING_DeadBandMode == PWM_STEERING__B_PWM__DBM_256_CLOCKS || \
                PWM_STEERING_DeadBandMode == PWM_STEERING__B_PWM__DBM_2_4_CLOCKS)
                PWM_STEERING_WriteDeadTime(PWM_STEERING_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_STEERING_KillModeMinTime)
                PWM_STEERING_WriteKillTime(PWM_STEERING_backup.PWMKillCounterPeriod);
            #endif /* (PWM_STEERING_KillModeMinTime) */

            #if(PWM_STEERING_UseControl)
                PWM_STEERING_WriteControlRegister(PWM_STEERING_backup.PWMControlRegister);
            #endif /* (PWM_STEERING_UseControl) */
        #endif  /* (!PWM_STEERING_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_STEERING_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_STEERING_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_STEERING_Sleep(void) 
{
    #if(PWM_STEERING_UseControl)
        if(PWM_STEERING_CTRL_ENABLE == (PWM_STEERING_CONTROL & PWM_STEERING_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_STEERING_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_STEERING_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_STEERING_UseControl) */

    /* Stop component */
    PWM_STEERING_Stop();

    /* Save registers configuration */
    PWM_STEERING_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_STEERING_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_STEERING_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_STEERING_Wakeup(void) 
{
     /* Restore registers values */
    PWM_STEERING_RestoreConfig();

    if(PWM_STEERING_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_STEERING_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

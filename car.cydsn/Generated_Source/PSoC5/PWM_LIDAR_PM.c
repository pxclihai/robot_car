/*******************************************************************************
* File Name: PWM_LIDAR_PM.c
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

#include "PWM_LIDAR.h"

static PWM_LIDAR_backupStruct PWM_LIDAR_backup;


/*******************************************************************************
* Function Name: PWM_LIDAR_SaveConfig
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
*  PWM_LIDAR_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_LIDAR_SaveConfig(void) 
{

    #if(!PWM_LIDAR_UsingFixedFunction)
        #if(!PWM_LIDAR_PWMModeIsCenterAligned)
            PWM_LIDAR_backup.PWMPeriod = PWM_LIDAR_ReadPeriod();
        #endif /* (!PWM_LIDAR_PWMModeIsCenterAligned) */
        PWM_LIDAR_backup.PWMUdb = PWM_LIDAR_ReadCounter();
        #if (PWM_LIDAR_UseStatus)
            PWM_LIDAR_backup.InterruptMaskValue = PWM_LIDAR_STATUS_MASK;
        #endif /* (PWM_LIDAR_UseStatus) */

        #if(PWM_LIDAR_DeadBandMode == PWM_LIDAR__B_PWM__DBM_256_CLOCKS || \
            PWM_LIDAR_DeadBandMode == PWM_LIDAR__B_PWM__DBM_2_4_CLOCKS)
            PWM_LIDAR_backup.PWMdeadBandValue = PWM_LIDAR_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_LIDAR_KillModeMinTime)
             PWM_LIDAR_backup.PWMKillCounterPeriod = PWM_LIDAR_ReadKillTime();
        #endif /* (PWM_LIDAR_KillModeMinTime) */

        #if(PWM_LIDAR_UseControl)
            PWM_LIDAR_backup.PWMControlRegister = PWM_LIDAR_ReadControlRegister();
        #endif /* (PWM_LIDAR_UseControl) */
    #endif  /* (!PWM_LIDAR_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_LIDAR_RestoreConfig
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
*  PWM_LIDAR_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_LIDAR_RestoreConfig(void) 
{
        #if(!PWM_LIDAR_UsingFixedFunction)
            #if(!PWM_LIDAR_PWMModeIsCenterAligned)
                PWM_LIDAR_WritePeriod(PWM_LIDAR_backup.PWMPeriod);
            #endif /* (!PWM_LIDAR_PWMModeIsCenterAligned) */

            PWM_LIDAR_WriteCounter(PWM_LIDAR_backup.PWMUdb);

            #if (PWM_LIDAR_UseStatus)
                PWM_LIDAR_STATUS_MASK = PWM_LIDAR_backup.InterruptMaskValue;
            #endif /* (PWM_LIDAR_UseStatus) */

            #if(PWM_LIDAR_DeadBandMode == PWM_LIDAR__B_PWM__DBM_256_CLOCKS || \
                PWM_LIDAR_DeadBandMode == PWM_LIDAR__B_PWM__DBM_2_4_CLOCKS)
                PWM_LIDAR_WriteDeadTime(PWM_LIDAR_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_LIDAR_KillModeMinTime)
                PWM_LIDAR_WriteKillTime(PWM_LIDAR_backup.PWMKillCounterPeriod);
            #endif /* (PWM_LIDAR_KillModeMinTime) */

            #if(PWM_LIDAR_UseControl)
                PWM_LIDAR_WriteControlRegister(PWM_LIDAR_backup.PWMControlRegister);
            #endif /* (PWM_LIDAR_UseControl) */
        #endif  /* (!PWM_LIDAR_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_LIDAR_Sleep
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
*  PWM_LIDAR_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_LIDAR_Sleep(void) 
{
    #if(PWM_LIDAR_UseControl)
        if(PWM_LIDAR_CTRL_ENABLE == (PWM_LIDAR_CONTROL & PWM_LIDAR_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_LIDAR_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_LIDAR_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_LIDAR_UseControl) */

    /* Stop component */
    PWM_LIDAR_Stop();

    /* Save registers configuration */
    PWM_LIDAR_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_LIDAR_Wakeup
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
*  PWM_LIDAR_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_LIDAR_Wakeup(void) 
{
     /* Restore registers values */
    PWM_LIDAR_RestoreConfig();

    if(PWM_LIDAR_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_LIDAR_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

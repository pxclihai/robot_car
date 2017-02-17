/*******************************************************************************
* File Name: WAVE_PM.c
* Version 3.50
*
* Description:
*  This file provides low power mode APIs for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WAVE_PVT.h"

WAVE_BACKUP_STRUCT WAVE_backup =
{
    WAVE_DISABLE,

#if (WAVE_FF_IMPLEMENTED)
    WAVE_DEFAULT_XCFG,
    WAVE_DEFAULT_CFG,
    WAVE_DEFAULT_ADDR,
    LO8(WAVE_DEFAULT_DIVIDE_FACTOR),
    HI8(WAVE_DEFAULT_DIVIDE_FACTOR),
#else  /* (WAVE_UDB_IMPLEMENTED) */
    WAVE_DEFAULT_CFG,
#endif /* (WAVE_FF_IMPLEMENTED) */

#if (WAVE_TIMEOUT_ENABLED)
    WAVE_DEFAULT_TMOUT_PERIOD,
    WAVE_DEFAULT_TMOUT_INTR_MASK,
#endif /* (WAVE_TIMEOUT_ENABLED) */
};

#if ((WAVE_FF_IMPLEMENTED) && (WAVE_WAKEUP_ENABLED))
    volatile uint8 WAVE_wakeupSource;
#endif /* ((WAVE_FF_IMPLEMENTED) && (WAVE_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: WAVE_SaveConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Stores the component non-retention configuration registers.
*   Checked:   Disables the master, if it was enabled before, and enables
*              backup regulator of the I2C hardware. If a transaction intended
*              for component executes during this function call, it waits until
*              the current transaction is completed and I2C hardware is ready
*              to enter sleep mode. All subsequent I2C traffic is NAKed until
*              the device is put into sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  WAVE_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            entering the sleep mode.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_SaveConfig(void) 
{
#if (WAVE_FF_IMPLEMENTED)
    #if (WAVE_WAKEUP_ENABLED)
        uint8 intState;
    #endif /* (WAVE_WAKEUP_ENABLED) */

    /* Store registers before enter low power mode */
    WAVE_backup.cfg     = WAVE_CFG_REG;
    WAVE_backup.xcfg    = WAVE_XCFG_REG;
    WAVE_backup.addr    = WAVE_ADDR_REG;
    WAVE_backup.clkDiv1 = WAVE_CLKDIV1_REG;
    WAVE_backup.clkDiv2 = WAVE_CLKDIV2_REG;

#if (WAVE_WAKEUP_ENABLED)
    /* Disable master */
    WAVE_CFG_REG &= (uint8) ~WAVE_ENABLE_MASTER;

    /* Enable backup regulator to keep block powered in low power mode */
    intState = CyEnterCriticalSection();
    WAVE_PWRSYS_CR1_REG |= WAVE_PWRSYS_CR1_I2C_REG_BACKUP;
    CyExitCriticalSection(intState);

    /* 1) Set force NACK to ignore I2C transactions;
    *  2) Wait unti I2C is ready go to Sleep; !!
    *  3) These bits are cleared on wake up.
    */
    /* Wait when block is ready for sleep */
    WAVE_XCFG_REG |= WAVE_XCFG_FORCE_NACK;
    while (0u == (WAVE_XCFG_REG & WAVE_XCFG_RDY_TO_SLEEP))
    {
    }

    /* Setup wakeup interrupt */
    WAVE_DisableInt();
    (void) CyIntSetVector(WAVE_ISR_NUMBER, &WAVE_WAKEUP_ISR);
    WAVE_wakeupSource = 0u;
    WAVE_EnableInt();
#endif /* (WAVE_WAKEUP_ENABLED) */

#else
    /* Store only address match bit */
    WAVE_backup.control = (WAVE_CFG_REG & WAVE_CTRL_ANY_ADDRESS_MASK);
#endif /* (WAVE_FF_IMPLEMENTED) */

#if (WAVE_TIMEOUT_ENABLED)
    WAVE_TimeoutSaveConfig();
#endif /* (WAVE_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: WAVE_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component before device enters
*  sleep mode. The Enable wakeup from Sleep Mode selection influences this
*  function implementation:
*   Unchecked: Checks current I2C component state, saves it, and disables the
*              component by calling I2C_Stop() if it is currently enabled.
*              I2C_SaveConfig() is then called to save the component
*              non-retention configuration registers.
*   Checked:   If a transaction intended for component executes during this
*              function call, it waits until the current transaction is
*              completed. All subsequent I2C traffic intended for component
*              is NAKed until the device is put to sleep mode. The address
*              match event wakes up the device.
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
void WAVE_Sleep(void) 
{
#if (WAVE_WAKEUP_ENABLED)
    /* Do not enable block after exit low power mode if it is wakeup source */
    WAVE_backup.enableState = WAVE_DISABLE;

    #if (WAVE_TIMEOUT_ENABLED)
        WAVE_TimeoutStop();
    #endif /* (WAVE_TIMEOUT_ENABLED) */

#else
    /* Store enable state */
    WAVE_backup.enableState = (uint8) WAVE_IS_ENABLED;

    if (0u != WAVE_backup.enableState)
    {
        WAVE_Stop();
    }
#endif /* (WAVE_WAKEUP_ENABLED) */

    WAVE_SaveConfig();
}


/*******************************************************************************
* Function Name: WAVE_RestoreConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              to the state they were in before I2C_Sleep() or I2C_SaveConfig()
*              was called.
*   Checked:   Disables the backup regulator of the I2C hardware. Sets up the
*              regular component interrupt handler and generates the component
*              interrupt if it was wake up source to release the bus and
*              continue in-coming I2C transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  WAVE_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            exiting the sleep mode.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before WAVE_SaveConfig() or
*  WAVE_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void WAVE_RestoreConfig(void) 
{
#if (WAVE_FF_IMPLEMENTED)
    uint8 intState;

    if (WAVE_CHECK_PWRSYS_I2C_BACKUP)
    /* Low power mode was Sleep - backup regulator is enabled */
    {
        /* Enable backup regulator in active mode */
        intState = CyEnterCriticalSection();
        WAVE_PWRSYS_CR1_REG &= (uint8) ~WAVE_PWRSYS_CR1_I2C_REG_BACKUP;
        CyExitCriticalSection(intState);

        /* Restore master */
        WAVE_CFG_REG = WAVE_backup.cfg;
    }
    else
    /* Low power mode was Hibernate - backup regulator is disabled. All registers are cleared */
    {
    #if (WAVE_WAKEUP_ENABLED)
        /* Disable power to block before register restore */
        intState = CyEnterCriticalSection();
        WAVE_ACT_PWRMGR_REG  &= (uint8) ~WAVE_ACT_PWR_EN;
        WAVE_STBY_PWRMGR_REG &= (uint8) ~WAVE_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable component in I2C_Wakeup() after register restore */
        WAVE_backup.enableState = WAVE_ENABLE;
    #endif /* (WAVE_WAKEUP_ENABLED) */

        /* Restore component registers after Hibernate */
        WAVE_XCFG_REG    = WAVE_backup.xcfg;
        WAVE_CFG_REG     = WAVE_backup.cfg;
        WAVE_ADDR_REG    = WAVE_backup.addr;
        WAVE_CLKDIV1_REG = WAVE_backup.clkDiv1;
        WAVE_CLKDIV2_REG = WAVE_backup.clkDiv2;
    }

#if (WAVE_WAKEUP_ENABLED)
    WAVE_DisableInt();
    (void) CyIntSetVector(WAVE_ISR_NUMBER, &WAVE_ISR);
    if (0u != WAVE_wakeupSource)
    {
        /* Generate interrupt to process incoming transaction */
        WAVE_SetPendingInt();
    }
    WAVE_EnableInt();
#endif /* (WAVE_WAKEUP_ENABLED) */

#else
    WAVE_CFG_REG = WAVE_backup.control;
#endif /* (WAVE_FF_IMPLEMENTED) */

#if (WAVE_TIMEOUT_ENABLED)
    WAVE_TimeoutRestoreConfig();
#endif /* (WAVE_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: WAVE_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component for active mode
*  operation (when device exits sleep mode). The Enable wakeup from Sleep Mode
*  selection influences this function implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              by calling I2C_RestoreConfig(). If the component was enabled
*              before the I2C_Sleep() function was called, I2C_Wakeup()
*              re-enables it.
*   Checked:   Enables  master functionality if it was enabled before sleep,
*              and disables the backup regulator of the I2C hardware.
*              The incoming transaction continues as soon as the regular
*              I2C interrupt handler is set up (global interrupts has to be
*              enabled to service I2C component interrupt).
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
* Side Effects:
*  Calling this function before WAVE_SaveConfig() or
*  WAVE_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void WAVE_Wakeup(void) 
{
    WAVE_RestoreConfig();

    /* Restore component enable state */
    if (0u != WAVE_backup.enableState)
    {
        WAVE_Enable();
        WAVE_EnableInt();
    }
    else
    {
    #if (WAVE_TIMEOUT_ENABLED)
        WAVE_TimeoutEnable();
    #endif /* (WAVE_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */

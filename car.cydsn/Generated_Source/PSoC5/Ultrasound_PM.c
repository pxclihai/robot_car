/*******************************************************************************
* File Name: Ultrasound_PM.c
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

#include "Ultrasound_PVT.h"

Ultrasound_BACKUP_STRUCT Ultrasound_backup =
{
    Ultrasound_DISABLE,

#if (Ultrasound_FF_IMPLEMENTED)
    Ultrasound_DEFAULT_XCFG,
    Ultrasound_DEFAULT_CFG,
    Ultrasound_DEFAULT_ADDR,
    LO8(Ultrasound_DEFAULT_DIVIDE_FACTOR),
    HI8(Ultrasound_DEFAULT_DIVIDE_FACTOR),
#else  /* (Ultrasound_UDB_IMPLEMENTED) */
    Ultrasound_DEFAULT_CFG,
#endif /* (Ultrasound_FF_IMPLEMENTED) */

#if (Ultrasound_TIMEOUT_ENABLED)
    Ultrasound_DEFAULT_TMOUT_PERIOD,
    Ultrasound_DEFAULT_TMOUT_INTR_MASK,
#endif /* (Ultrasound_TIMEOUT_ENABLED) */
};

#if ((Ultrasound_FF_IMPLEMENTED) && (Ultrasound_WAKEUP_ENABLED))
    volatile uint8 Ultrasound_wakeupSource;
#endif /* ((Ultrasound_FF_IMPLEMENTED) && (Ultrasound_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: Ultrasound_SaveConfig
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
*  Ultrasound_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            entering the sleep mode.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_SaveConfig(void) 
{
#if (Ultrasound_FF_IMPLEMENTED)
    #if (Ultrasound_WAKEUP_ENABLED)
        uint8 intState;
    #endif /* (Ultrasound_WAKEUP_ENABLED) */

    /* Store registers before enter low power mode */
    Ultrasound_backup.cfg     = Ultrasound_CFG_REG;
    Ultrasound_backup.xcfg    = Ultrasound_XCFG_REG;
    Ultrasound_backup.addr    = Ultrasound_ADDR_REG;
    Ultrasound_backup.clkDiv1 = Ultrasound_CLKDIV1_REG;
    Ultrasound_backup.clkDiv2 = Ultrasound_CLKDIV2_REG;

#if (Ultrasound_WAKEUP_ENABLED)
    /* Disable master */
    Ultrasound_CFG_REG &= (uint8) ~Ultrasound_ENABLE_MASTER;

    /* Enable backup regulator to keep block powered in low power mode */
    intState = CyEnterCriticalSection();
    Ultrasound_PWRSYS_CR1_REG |= Ultrasound_PWRSYS_CR1_I2C_REG_BACKUP;
    CyExitCriticalSection(intState);

    /* 1) Set force NACK to ignore I2C transactions;
    *  2) Wait unti I2C is ready go to Sleep; !!
    *  3) These bits are cleared on wake up.
    */
    /* Wait when block is ready for sleep */
    Ultrasound_XCFG_REG |= Ultrasound_XCFG_FORCE_NACK;
    while (0u == (Ultrasound_XCFG_REG & Ultrasound_XCFG_RDY_TO_SLEEP))
    {
    }

    /* Setup wakeup interrupt */
    Ultrasound_DisableInt();
    (void) CyIntSetVector(Ultrasound_ISR_NUMBER, &Ultrasound_WAKEUP_ISR);
    Ultrasound_wakeupSource = 0u;
    Ultrasound_EnableInt();
#endif /* (Ultrasound_WAKEUP_ENABLED) */

#else
    /* Store only address match bit */
    Ultrasound_backup.control = (Ultrasound_CFG_REG & Ultrasound_CTRL_ANY_ADDRESS_MASK);
#endif /* (Ultrasound_FF_IMPLEMENTED) */

#if (Ultrasound_TIMEOUT_ENABLED)
    Ultrasound_TimeoutSaveConfig();
#endif /* (Ultrasound_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: Ultrasound_Sleep
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
void Ultrasound_Sleep(void) 
{
#if (Ultrasound_WAKEUP_ENABLED)
    /* Do not enable block after exit low power mode if it is wakeup source */
    Ultrasound_backup.enableState = Ultrasound_DISABLE;

    #if (Ultrasound_TIMEOUT_ENABLED)
        Ultrasound_TimeoutStop();
    #endif /* (Ultrasound_TIMEOUT_ENABLED) */

#else
    /* Store enable state */
    Ultrasound_backup.enableState = (uint8) Ultrasound_IS_ENABLED;

    if (0u != Ultrasound_backup.enableState)
    {
        Ultrasound_Stop();
    }
#endif /* (Ultrasound_WAKEUP_ENABLED) */

    Ultrasound_SaveConfig();
}


/*******************************************************************************
* Function Name: Ultrasound_RestoreConfig
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
*  Ultrasound_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            exiting the sleep mode.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before Ultrasound_SaveConfig() or
*  Ultrasound_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void Ultrasound_RestoreConfig(void) 
{
#if (Ultrasound_FF_IMPLEMENTED)
    uint8 intState;

    if (Ultrasound_CHECK_PWRSYS_I2C_BACKUP)
    /* Low power mode was Sleep - backup regulator is enabled */
    {
        /* Enable backup regulator in active mode */
        intState = CyEnterCriticalSection();
        Ultrasound_PWRSYS_CR1_REG &= (uint8) ~Ultrasound_PWRSYS_CR1_I2C_REG_BACKUP;
        CyExitCriticalSection(intState);

        /* Restore master */
        Ultrasound_CFG_REG = Ultrasound_backup.cfg;
    }
    else
    /* Low power mode was Hibernate - backup regulator is disabled. All registers are cleared */
    {
    #if (Ultrasound_WAKEUP_ENABLED)
        /* Disable power to block before register restore */
        intState = CyEnterCriticalSection();
        Ultrasound_ACT_PWRMGR_REG  &= (uint8) ~Ultrasound_ACT_PWR_EN;
        Ultrasound_STBY_PWRMGR_REG &= (uint8) ~Ultrasound_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable component in I2C_Wakeup() after register restore */
        Ultrasound_backup.enableState = Ultrasound_ENABLE;
    #endif /* (Ultrasound_WAKEUP_ENABLED) */

        /* Restore component registers after Hibernate */
        Ultrasound_XCFG_REG    = Ultrasound_backup.xcfg;
        Ultrasound_CFG_REG     = Ultrasound_backup.cfg;
        Ultrasound_ADDR_REG    = Ultrasound_backup.addr;
        Ultrasound_CLKDIV1_REG = Ultrasound_backup.clkDiv1;
        Ultrasound_CLKDIV2_REG = Ultrasound_backup.clkDiv2;
    }

#if (Ultrasound_WAKEUP_ENABLED)
    Ultrasound_DisableInt();
    (void) CyIntSetVector(Ultrasound_ISR_NUMBER, &Ultrasound_ISR);
    if (0u != Ultrasound_wakeupSource)
    {
        /* Generate interrupt to process incoming transaction */
        Ultrasound_SetPendingInt();
    }
    Ultrasound_EnableInt();
#endif /* (Ultrasound_WAKEUP_ENABLED) */

#else
    Ultrasound_CFG_REG = Ultrasound_backup.control;
#endif /* (Ultrasound_FF_IMPLEMENTED) */

#if (Ultrasound_TIMEOUT_ENABLED)
    Ultrasound_TimeoutRestoreConfig();
#endif /* (Ultrasound_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: Ultrasound_Wakeup
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
*  Calling this function before Ultrasound_SaveConfig() or
*  Ultrasound_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void Ultrasound_Wakeup(void) 
{
    Ultrasound_RestoreConfig();

    /* Restore component enable state */
    if (0u != Ultrasound_backup.enableState)
    {
        Ultrasound_Enable();
        Ultrasound_EnableInt();
    }
    else
    {
    #if (Ultrasound_TIMEOUT_ENABLED)
        Ultrasound_TimeoutEnable();
    #endif /* (Ultrasound_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */

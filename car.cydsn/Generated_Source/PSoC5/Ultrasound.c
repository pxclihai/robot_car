/*******************************************************************************
* File Name: Ultrasound.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  The actual protocol and operation code resides in the interrupt service
*  routine file.
*
*******************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Ultrasound_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 Ultrasound_initVar = 0u; /* Defines if component was initialized */

volatile uint8 Ultrasound_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: Ultrasound_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_Init(void) 
{
#if (Ultrasound_FF_IMPLEMENTED)
    /* Configure fixed function block */
    Ultrasound_CFG_REG  = Ultrasound_DEFAULT_CFG;
    Ultrasound_XCFG_REG = Ultrasound_DEFAULT_XCFG;
    Ultrasound_ADDR_REG = Ultrasound_DEFAULT_ADDR;
    Ultrasound_CLKDIV1_REG = LO8(Ultrasound_DEFAULT_DIVIDE_FACTOR);
    Ultrasound_CLKDIV2_REG = HI8(Ultrasound_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    Ultrasound_CFG_REG      = Ultrasound_DEFAULT_CFG;
    Ultrasound_INT_MASK_REG = Ultrasound_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    Ultrasound_INT_ENABLE_REG |= Ultrasound_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (Ultrasound_MODE_SLAVE_ENABLED)
        Ultrasound_PERIOD_REG = Ultrasound_DEFAULT_PERIOD;
    #endif  /* (Ultrasound_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (Ultrasound_MODE_MASTER_ENABLED)
        Ultrasound_MCLK_PRD_REG = Ultrasound_DEFAULT_MCLK_PRD;
        Ultrasound_MCLK_CMP_REG = Ultrasound_DEFAULT_MCLK_CMP;
    #endif /* (Ultrasound_MODE_MASTER_ENABLED) */
#endif /* (Ultrasound_FF_IMPLEMENTED) */

#if (Ultrasound_TIMEOUT_ENABLED)
    Ultrasound_TimeoutInit();
#endif /* (Ultrasound_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (Ultrasound_ISR_NUMBER);
    CyIntSetPriority(Ultrasound_ISR_NUMBER, Ultrasound_ISR_PRIORITY);
    #if (Ultrasound_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(Ultrasound_ISR_NUMBER, &Ultrasound_ISR);
    #endif /* (Ultrasound_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    Ultrasound_state = Ultrasound_SM_IDLE;

#if (Ultrasound_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    Ultrasound_slStatus = 0u;
    Ultrasound_slRdBufIndex = 0u;
    Ultrasound_slWrBufIndex = 0u;

    /* Configure matched address */
    Ultrasound_SlaveSetAddress(Ultrasound_DEFAULT_ADDR);
#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */

#if (Ultrasound_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    Ultrasound_mstrStatus = 0u;
    Ultrasound_mstrRdBufIndex = 0u;
    Ultrasound_mstrWrBufIndex = 0u;
#endif /* (Ultrasound_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: Ultrasound_Enable
********************************************************************************
*
* Summary:
*  Enables I2C operations.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
*******************************************************************************/
void Ultrasound_Enable(void) 
{
#if (Ultrasound_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    Ultrasound_ACT_PWRMGR_REG  |= Ultrasound_ACT_PWR_EN;
    Ultrasound_STBY_PWRMGR_REG |= Ultrasound_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (Ultrasound_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        Ultrasound_COUNTER_AUX_CTL_REG |= Ultrasound_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (Ultrasound_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    Ultrasound_CFG_REG |= Ultrasound_ENABLE_MS;
#endif /* (Ultrasound_FF_IMPLEMENTED) */

#if (Ultrasound_TIMEOUT_ENABLED)
    Ultrasound_TimeoutEnable();
#endif /* (Ultrasound_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: Ultrasound_Start
********************************************************************************
*
* Summary:
*  Starts the I2C hardware. Enables Active mode power template bits or clock
*  gating as appropriate. It is required to be executed before I2C bus
*  operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  This component automatically enables its interrupt.  If I2C is enabled !
*  without the interrupt enabled, it can lock up the I2C bus.
*
* Global variables:
*  Ultrasound_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Ultrasound_Start(void) 
{
    if (0u == Ultrasound_initVar)
    {
        Ultrasound_Init();
        Ultrasound_initVar = 1u; /* Component initialized */
    }

    Ultrasound_Enable();
    Ultrasound_EnableInt();
}


/*******************************************************************************
* Function Name: Ultrasound_Stop
********************************************************************************
*
* Summary:
*  Disables I2C hardware and disables I2C interrupt. Disables Active mode power
*  template bits or clock gating as appropriate.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Ultrasound_Stop(void) 
{
    Ultrasound_DisableInt();

#if (Ultrasound_TIMEOUT_ENABLED)
    Ultrasound_TimeoutStop();
#endif  /* End (Ultrasound_TIMEOUT_ENABLED) */

#if (Ultrasound_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        Ultrasound_backup.addr    = Ultrasound_ADDR_REG;
        Ultrasound_backup.clkDiv1 = Ultrasound_CLKDIV1_REG;
        Ultrasound_backup.clkDiv2 = Ultrasound_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) Ultrasound_CLKDIV2_REG << 8u) | Ultrasound_CLKDIV1_REG) + 1u;

        /* Disable block */
        Ultrasound_CFG_REG &= (uint8) ~Ultrasound_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        Ultrasound_ACT_PWRMGR_REG  &= (uint8) ~Ultrasound_ACT_PWR_EN;
        Ultrasound_STBY_PWRMGR_REG &= (uint8) ~Ultrasound_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        Ultrasound_CFG_REG |= (uint8) Ultrasound_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        Ultrasound_ADDR_REG    = Ultrasound_backup.addr;
        Ultrasound_ADDR_REG    = Ultrasound_backup.addr;
        Ultrasound_CLKDIV1_REG = Ultrasound_backup.clkDiv1;
        Ultrasound_CLKDIV2_REG = Ultrasound_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    Ultrasound_CFG_REG &= (uint8) ~Ultrasound_ENABLE_MS;

#if (Ultrasound_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        Ultrasound_COUNTER_AUX_CTL_REG &= (uint8) ~Ultrasound_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) Ultrasound_CSR_REG;
#endif /* (Ultrasound_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    Ultrasound_DISABLE_INT_ON_STOP;
    Ultrasound_ClearPendingInt();

    /* Reset FSM to default state */
    Ultrasound_state = Ultrasound_SM_IDLE;

    /* Clear busy statuses */
#if (Ultrasound_MODE_SLAVE_ENABLED)
    Ultrasound_slStatus &= (uint8) ~(Ultrasound_SSTAT_RD_BUSY | Ultrasound_SSTAT_WR_BUSY);
#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */

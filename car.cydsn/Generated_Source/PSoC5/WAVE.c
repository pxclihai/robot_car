/*******************************************************************************
* File Name: WAVE.c
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

#include "WAVE_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 WAVE_initVar = 0u; /* Defines if component was initialized */

volatile uint8 WAVE_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: WAVE_Init
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
void WAVE_Init(void) 
{
#if (WAVE_FF_IMPLEMENTED)
    /* Configure fixed function block */
    WAVE_CFG_REG  = WAVE_DEFAULT_CFG;
    WAVE_XCFG_REG = WAVE_DEFAULT_XCFG;
    WAVE_ADDR_REG = WAVE_DEFAULT_ADDR;
    WAVE_CLKDIV1_REG = LO8(WAVE_DEFAULT_DIVIDE_FACTOR);
    WAVE_CLKDIV2_REG = HI8(WAVE_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    WAVE_CFG_REG      = WAVE_DEFAULT_CFG;
    WAVE_INT_MASK_REG = WAVE_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    WAVE_INT_ENABLE_REG |= WAVE_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (WAVE_MODE_SLAVE_ENABLED)
        WAVE_PERIOD_REG = WAVE_DEFAULT_PERIOD;
    #endif  /* (WAVE_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (WAVE_MODE_MASTER_ENABLED)
        WAVE_MCLK_PRD_REG = WAVE_DEFAULT_MCLK_PRD;
        WAVE_MCLK_CMP_REG = WAVE_DEFAULT_MCLK_CMP;
    #endif /* (WAVE_MODE_MASTER_ENABLED) */
#endif /* (WAVE_FF_IMPLEMENTED) */

#if (WAVE_TIMEOUT_ENABLED)
    WAVE_TimeoutInit();
#endif /* (WAVE_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (WAVE_ISR_NUMBER);
    CyIntSetPriority(WAVE_ISR_NUMBER, WAVE_ISR_PRIORITY);
    #if (WAVE_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(WAVE_ISR_NUMBER, &WAVE_ISR);
    #endif /* (WAVE_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    WAVE_state = WAVE_SM_IDLE;

#if (WAVE_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    WAVE_slStatus = 0u;
    WAVE_slRdBufIndex = 0u;
    WAVE_slWrBufIndex = 0u;

    /* Configure matched address */
    WAVE_SlaveSetAddress(WAVE_DEFAULT_ADDR);
#endif /* (WAVE_MODE_SLAVE_ENABLED) */

#if (WAVE_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    WAVE_mstrStatus = 0u;
    WAVE_mstrRdBufIndex = 0u;
    WAVE_mstrWrBufIndex = 0u;
#endif /* (WAVE_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: WAVE_Enable
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
void WAVE_Enable(void) 
{
#if (WAVE_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    WAVE_ACT_PWRMGR_REG  |= WAVE_ACT_PWR_EN;
    WAVE_STBY_PWRMGR_REG |= WAVE_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (WAVE_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        WAVE_COUNTER_AUX_CTL_REG |= WAVE_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (WAVE_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    WAVE_CFG_REG |= WAVE_ENABLE_MS;
#endif /* (WAVE_FF_IMPLEMENTED) */

#if (WAVE_TIMEOUT_ENABLED)
    WAVE_TimeoutEnable();
#endif /* (WAVE_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: WAVE_Start
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
*  WAVE_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void WAVE_Start(void) 
{
    if (0u == WAVE_initVar)
    {
        WAVE_Init();
        WAVE_initVar = 1u; /* Component initialized */
    }

    WAVE_Enable();
    WAVE_EnableInt();
}


/*******************************************************************************
* Function Name: WAVE_Stop
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
void WAVE_Stop(void) 
{
    WAVE_DisableInt();

#if (WAVE_TIMEOUT_ENABLED)
    WAVE_TimeoutStop();
#endif  /* End (WAVE_TIMEOUT_ENABLED) */

#if (WAVE_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        WAVE_backup.addr    = WAVE_ADDR_REG;
        WAVE_backup.clkDiv1 = WAVE_CLKDIV1_REG;
        WAVE_backup.clkDiv2 = WAVE_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) WAVE_CLKDIV2_REG << 8u) | WAVE_CLKDIV1_REG) + 1u;

        /* Disable block */
        WAVE_CFG_REG &= (uint8) ~WAVE_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        WAVE_ACT_PWRMGR_REG  &= (uint8) ~WAVE_ACT_PWR_EN;
        WAVE_STBY_PWRMGR_REG &= (uint8) ~WAVE_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        WAVE_CFG_REG |= (uint8) WAVE_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        WAVE_ADDR_REG    = WAVE_backup.addr;
        WAVE_ADDR_REG    = WAVE_backup.addr;
        WAVE_CLKDIV1_REG = WAVE_backup.clkDiv1;
        WAVE_CLKDIV2_REG = WAVE_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    WAVE_CFG_REG &= (uint8) ~WAVE_ENABLE_MS;

#if (WAVE_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        WAVE_COUNTER_AUX_CTL_REG &= (uint8) ~WAVE_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (WAVE_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) WAVE_CSR_REG;
#endif /* (WAVE_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    WAVE_DISABLE_INT_ON_STOP;
    WAVE_ClearPendingInt();

    /* Reset FSM to default state */
    WAVE_state = WAVE_SM_IDLE;

    /* Clear busy statuses */
#if (WAVE_MODE_SLAVE_ENABLED)
    WAVE_slStatus &= (uint8) ~(WAVE_SSTAT_RD_BUSY | WAVE_SSTAT_WR_BUSY);
#endif /* (WAVE_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */

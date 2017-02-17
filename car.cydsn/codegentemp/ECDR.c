/*******************************************************************************
* File Name: ECDR.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
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

#if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
    #include "ECDR_PVT.h"
#endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */

uint8 ECDR_initVar = 0u;


/*******************************************************************************
* Function Name: ECDR_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ECDR_Init(void) 
{
    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(ECDR_ISR_NUMBER);
        /* Set the ISR to point to the ECDR_isr Interrupt. */
        (void) CyIntSetVector(ECDR_ISR_NUMBER, & ECDR_ISR);
        /* Set the priority. */
        CyIntSetPriority(ECDR_ISR_NUMBER, ECDR_ISR_PRIORITY);
    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: ECDR_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ECDR_Enable(void) 
{
    uint8 enableInterrupts;

    ECDR_SetInterruptMask(ECDR_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) ECDR_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    ECDR_SR_AUX_CONTROL |= ECDR_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(ECDR_ISR_NUMBER);
    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: ECDR_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  ECDR_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ECDR_Start(void) 
{
    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
        ECDR_Cnt8_Start();
        ECDR_Cnt8_WriteCounter(ECDR_COUNTER_INIT_VALUE);
    #else
        /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) || 
        *  (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT) 
        */
        ECDR_Cnt16_Start();
        ECDR_Cnt16_WriteCounter(ECDR_COUNTER_INIT_VALUE);
    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT */
    
    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)        
       ECDR_count32SoftPart = 0;
    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */

    if (ECDR_initVar == 0u)
    {
        ECDR_Init();
        ECDR_initVar = 1u;
    }

    ECDR_Enable();
}


/*******************************************************************************
* Function Name: ECDR_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ECDR_Stop(void) 
{
    uint8 enableInterrupts;

    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
        ECDR_Cnt8_Stop();
    #else 
        /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) ||
        *  (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
        */
        ECDR_Cnt16_Stop();    /* counter disable */
    #endif /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    ECDR_SR_AUX_CONTROL &= (uint8) (~ECDR_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
        CyIntDisable(ECDR_ISR_NUMBER);    /* interrupt disable */
    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: ECDR_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  ECDR_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int16 ECDR_GetCounter(void) 
{
    int16 count;
    uint16 tmpCnt;

    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(ECDR_ISR_NUMBER);

        tmpCnt = ECDR_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) ECDR_COUNTER_INIT_VALUE);
        count = ECDR_count32SoftPart + hwCount;

        CyIntEnable(ECDR_ISR_NUMBER);
    #else 
        /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) || 
        *  (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT)
        */
        #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
            tmpCnt = ECDR_Cnt8_ReadCounter();
        #else /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) */
            tmpCnt = ECDR_Cnt16_ReadCounter();
        #endif  /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT */

        count = (int16) ((int32) tmpCnt -
                (int32) ECDR_COUNTER_INIT_VALUE);

    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: ECDR_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  ECDR_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ECDR_SetCounter(int16 value) 
{
    #if ((ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) || \
         (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + ECDR_COUNTER_INIT_VALUE;
        }
        else
        {
            count = ECDR_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
            ECDR_Cnt8_WriteCounter(count);
        #else /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) */
            ECDR_Cnt16_WriteCounter(count);
        #endif  /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT */
    #else /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT) */
        CyIntDisable(ECDR_ISR_NUMBER);

        ECDR_Cnt16_WriteCounter(ECDR_COUNTER_INIT_VALUE);
        ECDR_count32SoftPart = value;

        CyIntEnable(ECDR_ISR_NUMBER);
    #endif  /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT) ||
             * (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: ECDR_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 ECDR_GetEvents(void) 
{
    return (ECDR_STATUS_REG & ECDR_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: ECDR_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void ECDR_SetInterruptMask(uint8 mask) 
{
    #if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (ECDR_COUNTER_OVERFLOW | ECDR_COUNTER_UNDERFLOW |
                 ECDR_COUNTER_RESET);
    #endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */

    ECDR_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: ECDR_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 ECDR_GetInterruptMask(void) 
{
    return (ECDR_STATUS_MASK & ECDR_INIT_INT_MASK);
}


/* [] END OF FILE */

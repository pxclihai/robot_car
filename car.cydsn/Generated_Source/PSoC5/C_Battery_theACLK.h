/*******************************************************************************
* File Name: C_Battery_theACLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_C_Battery_theACLK_H)
#define CY_CLOCK_C_Battery_theACLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void C_Battery_theACLK_Start(void) ;
void C_Battery_theACLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void C_Battery_theACLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void C_Battery_theACLK_StandbyPower(uint8 state) ;
void C_Battery_theACLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 C_Battery_theACLK_GetDividerRegister(void) ;
void C_Battery_theACLK_SetModeRegister(uint8 modeBitMask) ;
void C_Battery_theACLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 C_Battery_theACLK_GetModeRegister(void) ;
void C_Battery_theACLK_SetSourceRegister(uint8 clkSource) ;
uint8 C_Battery_theACLK_GetSourceRegister(void) ;
#if defined(C_Battery_theACLK__CFG3)
void C_Battery_theACLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 C_Battery_theACLK_GetPhaseRegister(void) ;
#endif /* defined(C_Battery_theACLK__CFG3) */

#define C_Battery_theACLK_Enable()                       C_Battery_theACLK_Start()
#define C_Battery_theACLK_Disable()                      C_Battery_theACLK_Stop()
#define C_Battery_theACLK_SetDivider(clkDivider)         C_Battery_theACLK_SetDividerRegister(clkDivider, 1u)
#define C_Battery_theACLK_SetDividerValue(clkDivider)    C_Battery_theACLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define C_Battery_theACLK_SetMode(clkMode)               C_Battery_theACLK_SetModeRegister(clkMode)
#define C_Battery_theACLK_SetSource(clkSource)           C_Battery_theACLK_SetSourceRegister(clkSource)
#if defined(C_Battery_theACLK__CFG3)
#define C_Battery_theACLK_SetPhase(clkPhase)             C_Battery_theACLK_SetPhaseRegister(clkPhase)
#define C_Battery_theACLK_SetPhaseValue(clkPhase)        C_Battery_theACLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(C_Battery_theACLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define C_Battery_theACLK_CLKEN              (* (reg8 *) C_Battery_theACLK__PM_ACT_CFG)
#define C_Battery_theACLK_CLKEN_PTR          ((reg8 *) C_Battery_theACLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define C_Battery_theACLK_CLKSTBY            (* (reg8 *) C_Battery_theACLK__PM_STBY_CFG)
#define C_Battery_theACLK_CLKSTBY_PTR        ((reg8 *) C_Battery_theACLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define C_Battery_theACLK_DIV_LSB            (* (reg8 *) C_Battery_theACLK__CFG0)
#define C_Battery_theACLK_DIV_LSB_PTR        ((reg8 *) C_Battery_theACLK__CFG0)
#define C_Battery_theACLK_DIV_PTR            ((reg16 *) C_Battery_theACLK__CFG0)

/* Clock MSB divider configuration register. */
#define C_Battery_theACLK_DIV_MSB            (* (reg8 *) C_Battery_theACLK__CFG1)
#define C_Battery_theACLK_DIV_MSB_PTR        ((reg8 *) C_Battery_theACLK__CFG1)

/* Mode and source configuration register */
#define C_Battery_theACLK_MOD_SRC            (* (reg8 *) C_Battery_theACLK__CFG2)
#define C_Battery_theACLK_MOD_SRC_PTR        ((reg8 *) C_Battery_theACLK__CFG2)

#if defined(C_Battery_theACLK__CFG3)
/* Analog clock phase configuration register */
#define C_Battery_theACLK_PHASE              (* (reg8 *) C_Battery_theACLK__CFG3)
#define C_Battery_theACLK_PHASE_PTR          ((reg8 *) C_Battery_theACLK__CFG3)
#endif /* defined(C_Battery_theACLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define C_Battery_theACLK_CLKEN_MASK         C_Battery_theACLK__PM_ACT_MSK
#define C_Battery_theACLK_CLKSTBY_MASK       C_Battery_theACLK__PM_STBY_MSK

/* CFG2 field masks */
#define C_Battery_theACLK_SRC_SEL_MSK        C_Battery_theACLK__CFG2_SRC_SEL_MASK
#define C_Battery_theACLK_MODE_MASK          (~(C_Battery_theACLK_SRC_SEL_MSK))

#if defined(C_Battery_theACLK__CFG3)
/* CFG3 phase mask */
#define C_Battery_theACLK_PHASE_MASK         C_Battery_theACLK__CFG3_PHASE_DLY_MASK
#endif /* defined(C_Battery_theACLK__CFG3) */

#endif /* CY_CLOCK_C_Battery_theACLK_H */


/* [] END OF FILE */

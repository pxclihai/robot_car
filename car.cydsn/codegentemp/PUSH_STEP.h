/*******************************************************************************
* File Name: PUSH_STEP.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PUSH_STEP_H) /* Pins PUSH_STEP_H */
#define CY_PINS_PUSH_STEP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PUSH_STEP_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PUSH_STEP__PORT == 15 && ((PUSH_STEP__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    PUSH_STEP_Write(uint8 value) ;
void    PUSH_STEP_SetDriveMode(uint8 mode) ;
uint8   PUSH_STEP_ReadDataReg(void) ;
uint8   PUSH_STEP_Read(void) ;
uint8   PUSH_STEP_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PUSH_STEP_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define PUSH_STEP_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define PUSH_STEP_DM_RES_UP          PIN_DM_RES_UP
#define PUSH_STEP_DM_RES_DWN         PIN_DM_RES_DWN
#define PUSH_STEP_DM_OD_LO           PIN_DM_OD_LO
#define PUSH_STEP_DM_OD_HI           PIN_DM_OD_HI
#define PUSH_STEP_DM_STRONG          PIN_DM_STRONG
#define PUSH_STEP_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define PUSH_STEP_MASK               PUSH_STEP__MASK
#define PUSH_STEP_SHIFT              PUSH_STEP__SHIFT
#define PUSH_STEP_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PUSH_STEP_PS                     (* (reg8 *) PUSH_STEP__PS)
/* Data Register */
#define PUSH_STEP_DR                     (* (reg8 *) PUSH_STEP__DR)
/* Port Number */
#define PUSH_STEP_PRT_NUM                (* (reg8 *) PUSH_STEP__PRT) 
/* Connect to Analog Globals */                                                  
#define PUSH_STEP_AG                     (* (reg8 *) PUSH_STEP__AG)                       
/* Analog MUX bux enable */
#define PUSH_STEP_AMUX                   (* (reg8 *) PUSH_STEP__AMUX) 
/* Bidirectional Enable */                                                        
#define PUSH_STEP_BIE                    (* (reg8 *) PUSH_STEP__BIE)
/* Bit-mask for Aliased Register Access */
#define PUSH_STEP_BIT_MASK               (* (reg8 *) PUSH_STEP__BIT_MASK)
/* Bypass Enable */
#define PUSH_STEP_BYP                    (* (reg8 *) PUSH_STEP__BYP)
/* Port wide control signals */                                                   
#define PUSH_STEP_CTL                    (* (reg8 *) PUSH_STEP__CTL)
/* Drive Modes */
#define PUSH_STEP_DM0                    (* (reg8 *) PUSH_STEP__DM0) 
#define PUSH_STEP_DM1                    (* (reg8 *) PUSH_STEP__DM1)
#define PUSH_STEP_DM2                    (* (reg8 *) PUSH_STEP__DM2) 
/* Input Buffer Disable Override */
#define PUSH_STEP_INP_DIS                (* (reg8 *) PUSH_STEP__INP_DIS)
/* LCD Common or Segment Drive */
#define PUSH_STEP_LCD_COM_SEG            (* (reg8 *) PUSH_STEP__LCD_COM_SEG)
/* Enable Segment LCD */
#define PUSH_STEP_LCD_EN                 (* (reg8 *) PUSH_STEP__LCD_EN)
/* Slew Rate Control */
#define PUSH_STEP_SLW                    (* (reg8 *) PUSH_STEP__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PUSH_STEP_PRTDSI__CAPS_SEL       (* (reg8 *) PUSH_STEP__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PUSH_STEP_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PUSH_STEP__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PUSH_STEP_PRTDSI__OE_SEL0        (* (reg8 *) PUSH_STEP__PRTDSI__OE_SEL0) 
#define PUSH_STEP_PRTDSI__OE_SEL1        (* (reg8 *) PUSH_STEP__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PUSH_STEP_PRTDSI__OUT_SEL0       (* (reg8 *) PUSH_STEP__PRTDSI__OUT_SEL0) 
#define PUSH_STEP_PRTDSI__OUT_SEL1       (* (reg8 *) PUSH_STEP__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PUSH_STEP_PRTDSI__SYNC_OUT       (* (reg8 *) PUSH_STEP__PRTDSI__SYNC_OUT) 


#if defined(PUSH_STEP__INTSTAT)  /* Interrupt Registers */

    #define PUSH_STEP_INTSTAT                (* (reg8 *) PUSH_STEP__INTSTAT)
    #define PUSH_STEP_SNAP                   (* (reg8 *) PUSH_STEP__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PUSH_STEP_H */


/* [] END OF FILE */

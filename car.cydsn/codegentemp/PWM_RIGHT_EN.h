/*******************************************************************************
* File Name: PWM_RIGHT_EN.h  
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

#if !defined(CY_PINS_PWM_RIGHT_EN_H) /* Pins PWM_RIGHT_EN_H */
#define CY_PINS_PWM_RIGHT_EN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PWM_RIGHT_EN_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PWM_RIGHT_EN__PORT == 15 && ((PWM_RIGHT_EN__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    PWM_RIGHT_EN_Write(uint8 value) ;
void    PWM_RIGHT_EN_SetDriveMode(uint8 mode) ;
uint8   PWM_RIGHT_EN_ReadDataReg(void) ;
uint8   PWM_RIGHT_EN_Read(void) ;
uint8   PWM_RIGHT_EN_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PWM_RIGHT_EN_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define PWM_RIGHT_EN_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define PWM_RIGHT_EN_DM_RES_UP          PIN_DM_RES_UP
#define PWM_RIGHT_EN_DM_RES_DWN         PIN_DM_RES_DWN
#define PWM_RIGHT_EN_DM_OD_LO           PIN_DM_OD_LO
#define PWM_RIGHT_EN_DM_OD_HI           PIN_DM_OD_HI
#define PWM_RIGHT_EN_DM_STRONG          PIN_DM_STRONG
#define PWM_RIGHT_EN_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define PWM_RIGHT_EN_MASK               PWM_RIGHT_EN__MASK
#define PWM_RIGHT_EN_SHIFT              PWM_RIGHT_EN__SHIFT
#define PWM_RIGHT_EN_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PWM_RIGHT_EN_PS                     (* (reg8 *) PWM_RIGHT_EN__PS)
/* Data Register */
#define PWM_RIGHT_EN_DR                     (* (reg8 *) PWM_RIGHT_EN__DR)
/* Port Number */
#define PWM_RIGHT_EN_PRT_NUM                (* (reg8 *) PWM_RIGHT_EN__PRT) 
/* Connect to Analog Globals */                                                  
#define PWM_RIGHT_EN_AG                     (* (reg8 *) PWM_RIGHT_EN__AG)                       
/* Analog MUX bux enable */
#define PWM_RIGHT_EN_AMUX                   (* (reg8 *) PWM_RIGHT_EN__AMUX) 
/* Bidirectional Enable */                                                        
#define PWM_RIGHT_EN_BIE                    (* (reg8 *) PWM_RIGHT_EN__BIE)
/* Bit-mask for Aliased Register Access */
#define PWM_RIGHT_EN_BIT_MASK               (* (reg8 *) PWM_RIGHT_EN__BIT_MASK)
/* Bypass Enable */
#define PWM_RIGHT_EN_BYP                    (* (reg8 *) PWM_RIGHT_EN__BYP)
/* Port wide control signals */                                                   
#define PWM_RIGHT_EN_CTL                    (* (reg8 *) PWM_RIGHT_EN__CTL)
/* Drive Modes */
#define PWM_RIGHT_EN_DM0                    (* (reg8 *) PWM_RIGHT_EN__DM0) 
#define PWM_RIGHT_EN_DM1                    (* (reg8 *) PWM_RIGHT_EN__DM1)
#define PWM_RIGHT_EN_DM2                    (* (reg8 *) PWM_RIGHT_EN__DM2) 
/* Input Buffer Disable Override */
#define PWM_RIGHT_EN_INP_DIS                (* (reg8 *) PWM_RIGHT_EN__INP_DIS)
/* LCD Common or Segment Drive */
#define PWM_RIGHT_EN_LCD_COM_SEG            (* (reg8 *) PWM_RIGHT_EN__LCD_COM_SEG)
/* Enable Segment LCD */
#define PWM_RIGHT_EN_LCD_EN                 (* (reg8 *) PWM_RIGHT_EN__LCD_EN)
/* Slew Rate Control */
#define PWM_RIGHT_EN_SLW                    (* (reg8 *) PWM_RIGHT_EN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PWM_RIGHT_EN_PRTDSI__CAPS_SEL       (* (reg8 *) PWM_RIGHT_EN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PWM_RIGHT_EN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PWM_RIGHT_EN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PWM_RIGHT_EN_PRTDSI__OE_SEL0        (* (reg8 *) PWM_RIGHT_EN__PRTDSI__OE_SEL0) 
#define PWM_RIGHT_EN_PRTDSI__OE_SEL1        (* (reg8 *) PWM_RIGHT_EN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PWM_RIGHT_EN_PRTDSI__OUT_SEL0       (* (reg8 *) PWM_RIGHT_EN__PRTDSI__OUT_SEL0) 
#define PWM_RIGHT_EN_PRTDSI__OUT_SEL1       (* (reg8 *) PWM_RIGHT_EN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PWM_RIGHT_EN_PRTDSI__SYNC_OUT       (* (reg8 *) PWM_RIGHT_EN__PRTDSI__SYNC_OUT) 


#if defined(PWM_RIGHT_EN__INTSTAT)  /* Interrupt Registers */

    #define PWM_RIGHT_EN_INTSTAT                (* (reg8 *) PWM_RIGHT_EN__INTSTAT)
    #define PWM_RIGHT_EN_SNAP                   (* (reg8 *) PWM_RIGHT_EN__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PWM_RIGHT_EN_H */


/* [] END OF FILE */

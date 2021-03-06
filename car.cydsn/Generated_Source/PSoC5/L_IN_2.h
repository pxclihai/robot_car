/*******************************************************************************
* File Name: L_IN_2.h  
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

#if !defined(CY_PINS_L_IN_2_H) /* Pins L_IN_2_H */
#define CY_PINS_L_IN_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "L_IN_2_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 L_IN_2__PORT == 15 && ((L_IN_2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    L_IN_2_Write(uint8 value) ;
void    L_IN_2_SetDriveMode(uint8 mode) ;
uint8   L_IN_2_ReadDataReg(void) ;
uint8   L_IN_2_Read(void) ;
uint8   L_IN_2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define L_IN_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define L_IN_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define L_IN_2_DM_RES_UP          PIN_DM_RES_UP
#define L_IN_2_DM_RES_DWN         PIN_DM_RES_DWN
#define L_IN_2_DM_OD_LO           PIN_DM_OD_LO
#define L_IN_2_DM_OD_HI           PIN_DM_OD_HI
#define L_IN_2_DM_STRONG          PIN_DM_STRONG
#define L_IN_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define L_IN_2_MASK               L_IN_2__MASK
#define L_IN_2_SHIFT              L_IN_2__SHIFT
#define L_IN_2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define L_IN_2_PS                     (* (reg8 *) L_IN_2__PS)
/* Data Register */
#define L_IN_2_DR                     (* (reg8 *) L_IN_2__DR)
/* Port Number */
#define L_IN_2_PRT_NUM                (* (reg8 *) L_IN_2__PRT) 
/* Connect to Analog Globals */                                                  
#define L_IN_2_AG                     (* (reg8 *) L_IN_2__AG)                       
/* Analog MUX bux enable */
#define L_IN_2_AMUX                   (* (reg8 *) L_IN_2__AMUX) 
/* Bidirectional Enable */                                                        
#define L_IN_2_BIE                    (* (reg8 *) L_IN_2__BIE)
/* Bit-mask for Aliased Register Access */
#define L_IN_2_BIT_MASK               (* (reg8 *) L_IN_2__BIT_MASK)
/* Bypass Enable */
#define L_IN_2_BYP                    (* (reg8 *) L_IN_2__BYP)
/* Port wide control signals */                                                   
#define L_IN_2_CTL                    (* (reg8 *) L_IN_2__CTL)
/* Drive Modes */
#define L_IN_2_DM0                    (* (reg8 *) L_IN_2__DM0) 
#define L_IN_2_DM1                    (* (reg8 *) L_IN_2__DM1)
#define L_IN_2_DM2                    (* (reg8 *) L_IN_2__DM2) 
/* Input Buffer Disable Override */
#define L_IN_2_INP_DIS                (* (reg8 *) L_IN_2__INP_DIS)
/* LCD Common or Segment Drive */
#define L_IN_2_LCD_COM_SEG            (* (reg8 *) L_IN_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define L_IN_2_LCD_EN                 (* (reg8 *) L_IN_2__LCD_EN)
/* Slew Rate Control */
#define L_IN_2_SLW                    (* (reg8 *) L_IN_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define L_IN_2_PRTDSI__CAPS_SEL       (* (reg8 *) L_IN_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define L_IN_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) L_IN_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define L_IN_2_PRTDSI__OE_SEL0        (* (reg8 *) L_IN_2__PRTDSI__OE_SEL0) 
#define L_IN_2_PRTDSI__OE_SEL1        (* (reg8 *) L_IN_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define L_IN_2_PRTDSI__OUT_SEL0       (* (reg8 *) L_IN_2__PRTDSI__OUT_SEL0) 
#define L_IN_2_PRTDSI__OUT_SEL1       (* (reg8 *) L_IN_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define L_IN_2_PRTDSI__SYNC_OUT       (* (reg8 *) L_IN_2__PRTDSI__SYNC_OUT) 


#if defined(L_IN_2__INTSTAT)  /* Interrupt Registers */

    #define L_IN_2_INTSTAT                (* (reg8 *) L_IN_2__INTSTAT)
    #define L_IN_2_SNAP                   (* (reg8 *) L_IN_2__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_L_IN_2_H */


/* [] END OF FILE */

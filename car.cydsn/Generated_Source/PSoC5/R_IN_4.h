/*******************************************************************************
* File Name: R_IN_4.h  
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

#if !defined(CY_PINS_R_IN_4_H) /* Pins R_IN_4_H */
#define CY_PINS_R_IN_4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "R_IN_4_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 R_IN_4__PORT == 15 && ((R_IN_4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    R_IN_4_Write(uint8 value) ;
void    R_IN_4_SetDriveMode(uint8 mode) ;
uint8   R_IN_4_ReadDataReg(void) ;
uint8   R_IN_4_Read(void) ;
uint8   R_IN_4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define R_IN_4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define R_IN_4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define R_IN_4_DM_RES_UP          PIN_DM_RES_UP
#define R_IN_4_DM_RES_DWN         PIN_DM_RES_DWN
#define R_IN_4_DM_OD_LO           PIN_DM_OD_LO
#define R_IN_4_DM_OD_HI           PIN_DM_OD_HI
#define R_IN_4_DM_STRONG          PIN_DM_STRONG
#define R_IN_4_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define R_IN_4_MASK               R_IN_4__MASK
#define R_IN_4_SHIFT              R_IN_4__SHIFT
#define R_IN_4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define R_IN_4_PS                     (* (reg8 *) R_IN_4__PS)
/* Data Register */
#define R_IN_4_DR                     (* (reg8 *) R_IN_4__DR)
/* Port Number */
#define R_IN_4_PRT_NUM                (* (reg8 *) R_IN_4__PRT) 
/* Connect to Analog Globals */                                                  
#define R_IN_4_AG                     (* (reg8 *) R_IN_4__AG)                       
/* Analog MUX bux enable */
#define R_IN_4_AMUX                   (* (reg8 *) R_IN_4__AMUX) 
/* Bidirectional Enable */                                                        
#define R_IN_4_BIE                    (* (reg8 *) R_IN_4__BIE)
/* Bit-mask for Aliased Register Access */
#define R_IN_4_BIT_MASK               (* (reg8 *) R_IN_4__BIT_MASK)
/* Bypass Enable */
#define R_IN_4_BYP                    (* (reg8 *) R_IN_4__BYP)
/* Port wide control signals */                                                   
#define R_IN_4_CTL                    (* (reg8 *) R_IN_4__CTL)
/* Drive Modes */
#define R_IN_4_DM0                    (* (reg8 *) R_IN_4__DM0) 
#define R_IN_4_DM1                    (* (reg8 *) R_IN_4__DM1)
#define R_IN_4_DM2                    (* (reg8 *) R_IN_4__DM2) 
/* Input Buffer Disable Override */
#define R_IN_4_INP_DIS                (* (reg8 *) R_IN_4__INP_DIS)
/* LCD Common or Segment Drive */
#define R_IN_4_LCD_COM_SEG            (* (reg8 *) R_IN_4__LCD_COM_SEG)
/* Enable Segment LCD */
#define R_IN_4_LCD_EN                 (* (reg8 *) R_IN_4__LCD_EN)
/* Slew Rate Control */
#define R_IN_4_SLW                    (* (reg8 *) R_IN_4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define R_IN_4_PRTDSI__CAPS_SEL       (* (reg8 *) R_IN_4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define R_IN_4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) R_IN_4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define R_IN_4_PRTDSI__OE_SEL0        (* (reg8 *) R_IN_4__PRTDSI__OE_SEL0) 
#define R_IN_4_PRTDSI__OE_SEL1        (* (reg8 *) R_IN_4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define R_IN_4_PRTDSI__OUT_SEL0       (* (reg8 *) R_IN_4__PRTDSI__OUT_SEL0) 
#define R_IN_4_PRTDSI__OUT_SEL1       (* (reg8 *) R_IN_4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define R_IN_4_PRTDSI__SYNC_OUT       (* (reg8 *) R_IN_4__PRTDSI__SYNC_OUT) 


#if defined(R_IN_4__INTSTAT)  /* Interrupt Registers */

    #define R_IN_4_INTSTAT                (* (reg8 *) R_IN_4__INTSTAT)
    #define R_IN_4_SNAP                   (* (reg8 *) R_IN_4__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_R_IN_4_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: EM1.h  
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

#if !defined(CY_PINS_EM1_H) /* Pins EM1_H */
#define CY_PINS_EM1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "EM1_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 EM1__PORT == 15 && ((EM1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    EM1_Write(uint8 value) ;
void    EM1_SetDriveMode(uint8 mode) ;
uint8   EM1_ReadDataReg(void) ;
uint8   EM1_Read(void) ;
uint8   EM1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define EM1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define EM1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define EM1_DM_RES_UP          PIN_DM_RES_UP
#define EM1_DM_RES_DWN         PIN_DM_RES_DWN
#define EM1_DM_OD_LO           PIN_DM_OD_LO
#define EM1_DM_OD_HI           PIN_DM_OD_HI
#define EM1_DM_STRONG          PIN_DM_STRONG
#define EM1_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define EM1_MASK               EM1__MASK
#define EM1_SHIFT              EM1__SHIFT
#define EM1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define EM1_PS                     (* (reg8 *) EM1__PS)
/* Data Register */
#define EM1_DR                     (* (reg8 *) EM1__DR)
/* Port Number */
#define EM1_PRT_NUM                (* (reg8 *) EM1__PRT) 
/* Connect to Analog Globals */                                                  
#define EM1_AG                     (* (reg8 *) EM1__AG)                       
/* Analog MUX bux enable */
#define EM1_AMUX                   (* (reg8 *) EM1__AMUX) 
/* Bidirectional Enable */                                                        
#define EM1_BIE                    (* (reg8 *) EM1__BIE)
/* Bit-mask for Aliased Register Access */
#define EM1_BIT_MASK               (* (reg8 *) EM1__BIT_MASK)
/* Bypass Enable */
#define EM1_BYP                    (* (reg8 *) EM1__BYP)
/* Port wide control signals */                                                   
#define EM1_CTL                    (* (reg8 *) EM1__CTL)
/* Drive Modes */
#define EM1_DM0                    (* (reg8 *) EM1__DM0) 
#define EM1_DM1                    (* (reg8 *) EM1__DM1)
#define EM1_DM2                    (* (reg8 *) EM1__DM2) 
/* Input Buffer Disable Override */
#define EM1_INP_DIS                (* (reg8 *) EM1__INP_DIS)
/* LCD Common or Segment Drive */
#define EM1_LCD_COM_SEG            (* (reg8 *) EM1__LCD_COM_SEG)
/* Enable Segment LCD */
#define EM1_LCD_EN                 (* (reg8 *) EM1__LCD_EN)
/* Slew Rate Control */
#define EM1_SLW                    (* (reg8 *) EM1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define EM1_PRTDSI__CAPS_SEL       (* (reg8 *) EM1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define EM1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) EM1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define EM1_PRTDSI__OE_SEL0        (* (reg8 *) EM1__PRTDSI__OE_SEL0) 
#define EM1_PRTDSI__OE_SEL1        (* (reg8 *) EM1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define EM1_PRTDSI__OUT_SEL0       (* (reg8 *) EM1__PRTDSI__OUT_SEL0) 
#define EM1_PRTDSI__OUT_SEL1       (* (reg8 *) EM1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define EM1_PRTDSI__SYNC_OUT       (* (reg8 *) EM1__PRTDSI__SYNC_OUT) 


#if defined(EM1__INTSTAT)  /* Interrupt Registers */

    #define EM1_INTSTAT                (* (reg8 *) EM1__INTSTAT)
    #define EM1_SNAP                   (* (reg8 *) EM1__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_EM1_H */


/* [] END OF FILE */

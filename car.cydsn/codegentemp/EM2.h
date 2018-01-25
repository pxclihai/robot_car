/*******************************************************************************
* File Name: EM2.h  
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

#if !defined(CY_PINS_EM2_H) /* Pins EM2_H */
#define CY_PINS_EM2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "EM2_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 EM2__PORT == 15 && ((EM2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    EM2_Write(uint8 value) ;
void    EM2_SetDriveMode(uint8 mode) ;
uint8   EM2_ReadDataReg(void) ;
uint8   EM2_Read(void) ;
uint8   EM2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define EM2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define EM2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define EM2_DM_RES_UP          PIN_DM_RES_UP
#define EM2_DM_RES_DWN         PIN_DM_RES_DWN
#define EM2_DM_OD_LO           PIN_DM_OD_LO
#define EM2_DM_OD_HI           PIN_DM_OD_HI
#define EM2_DM_STRONG          PIN_DM_STRONG
#define EM2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define EM2_MASK               EM2__MASK
#define EM2_SHIFT              EM2__SHIFT
#define EM2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define EM2_PS                     (* (reg8 *) EM2__PS)
/* Data Register */
#define EM2_DR                     (* (reg8 *) EM2__DR)
/* Port Number */
#define EM2_PRT_NUM                (* (reg8 *) EM2__PRT) 
/* Connect to Analog Globals */                                                  
#define EM2_AG                     (* (reg8 *) EM2__AG)                       
/* Analog MUX bux enable */
#define EM2_AMUX                   (* (reg8 *) EM2__AMUX) 
/* Bidirectional Enable */                                                        
#define EM2_BIE                    (* (reg8 *) EM2__BIE)
/* Bit-mask for Aliased Register Access */
#define EM2_BIT_MASK               (* (reg8 *) EM2__BIT_MASK)
/* Bypass Enable */
#define EM2_BYP                    (* (reg8 *) EM2__BYP)
/* Port wide control signals */                                                   
#define EM2_CTL                    (* (reg8 *) EM2__CTL)
/* Drive Modes */
#define EM2_DM0                    (* (reg8 *) EM2__DM0) 
#define EM2_DM1                    (* (reg8 *) EM2__DM1)
#define EM2_DM2                    (* (reg8 *) EM2__DM2) 
/* Input Buffer Disable Override */
#define EM2_INP_DIS                (* (reg8 *) EM2__INP_DIS)
/* LCD Common or Segment Drive */
#define EM2_LCD_COM_SEG            (* (reg8 *) EM2__LCD_COM_SEG)
/* Enable Segment LCD */
#define EM2_LCD_EN                 (* (reg8 *) EM2__LCD_EN)
/* Slew Rate Control */
#define EM2_SLW                    (* (reg8 *) EM2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define EM2_PRTDSI__CAPS_SEL       (* (reg8 *) EM2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define EM2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) EM2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define EM2_PRTDSI__OE_SEL0        (* (reg8 *) EM2__PRTDSI__OE_SEL0) 
#define EM2_PRTDSI__OE_SEL1        (* (reg8 *) EM2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define EM2_PRTDSI__OUT_SEL0       (* (reg8 *) EM2__PRTDSI__OUT_SEL0) 
#define EM2_PRTDSI__OUT_SEL1       (* (reg8 *) EM2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define EM2_PRTDSI__SYNC_OUT       (* (reg8 *) EM2__PRTDSI__SYNC_OUT) 


#if defined(EM2__INTSTAT)  /* Interrupt Registers */

    #define EM2_INTSTAT                (* (reg8 *) EM2__INTSTAT)
    #define EM2_SNAP                   (* (reg8 *) EM2__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_EM2_H */


/* [] END OF FILE */

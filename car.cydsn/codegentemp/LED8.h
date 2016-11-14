/*******************************************************************************
* File Name: LED8.h  
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

#if !defined(CY_PINS_LED8_H) /* Pins LED8_H */
#define CY_PINS_LED8_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LED8_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 LED8__PORT == 15 && ((LED8__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    LED8_Write(uint8 value) ;
void    LED8_SetDriveMode(uint8 mode) ;
uint8   LED8_ReadDataReg(void) ;
uint8   LED8_Read(void) ;
uint8   LED8_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LED8_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define LED8_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define LED8_DM_RES_UP          PIN_DM_RES_UP
#define LED8_DM_RES_DWN         PIN_DM_RES_DWN
#define LED8_DM_OD_LO           PIN_DM_OD_LO
#define LED8_DM_OD_HI           PIN_DM_OD_HI
#define LED8_DM_STRONG          PIN_DM_STRONG
#define LED8_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define LED8_MASK               LED8__MASK
#define LED8_SHIFT              LED8__SHIFT
#define LED8_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LED8_PS                     (* (reg8 *) LED8__PS)
/* Data Register */
#define LED8_DR                     (* (reg8 *) LED8__DR)
/* Port Number */
#define LED8_PRT_NUM                (* (reg8 *) LED8__PRT) 
/* Connect to Analog Globals */                                                  
#define LED8_AG                     (* (reg8 *) LED8__AG)                       
/* Analog MUX bux enable */
#define LED8_AMUX                   (* (reg8 *) LED8__AMUX) 
/* Bidirectional Enable */                                                        
#define LED8_BIE                    (* (reg8 *) LED8__BIE)
/* Bit-mask for Aliased Register Access */
#define LED8_BIT_MASK               (* (reg8 *) LED8__BIT_MASK)
/* Bypass Enable */
#define LED8_BYP                    (* (reg8 *) LED8__BYP)
/* Port wide control signals */                                                   
#define LED8_CTL                    (* (reg8 *) LED8__CTL)
/* Drive Modes */
#define LED8_DM0                    (* (reg8 *) LED8__DM0) 
#define LED8_DM1                    (* (reg8 *) LED8__DM1)
#define LED8_DM2                    (* (reg8 *) LED8__DM2) 
/* Input Buffer Disable Override */
#define LED8_INP_DIS                (* (reg8 *) LED8__INP_DIS)
/* LCD Common or Segment Drive */
#define LED8_LCD_COM_SEG            (* (reg8 *) LED8__LCD_COM_SEG)
/* Enable Segment LCD */
#define LED8_LCD_EN                 (* (reg8 *) LED8__LCD_EN)
/* Slew Rate Control */
#define LED8_SLW                    (* (reg8 *) LED8__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LED8_PRTDSI__CAPS_SEL       (* (reg8 *) LED8__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LED8_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LED8__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LED8_PRTDSI__OE_SEL0        (* (reg8 *) LED8__PRTDSI__OE_SEL0) 
#define LED8_PRTDSI__OE_SEL1        (* (reg8 *) LED8__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LED8_PRTDSI__OUT_SEL0       (* (reg8 *) LED8__PRTDSI__OUT_SEL0) 
#define LED8_PRTDSI__OUT_SEL1       (* (reg8 *) LED8__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LED8_PRTDSI__SYNC_OUT       (* (reg8 *) LED8__PRTDSI__SYNC_OUT) 


#if defined(LED8__INTSTAT)  /* Interrupt Registers */

    #define LED8_INTSTAT                (* (reg8 *) LED8__INTSTAT)
    #define LED8_SNAP                   (* (reg8 *) LED8__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_LED8_H */


/* [] END OF FILE */

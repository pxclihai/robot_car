/*******************************************************************************
* File Name: CHANGE_PWR.h  
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

#if !defined(CY_PINS_CHANGE_PWR_H) /* Pins CHANGE_PWR_H */
#define CY_PINS_CHANGE_PWR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CHANGE_PWR_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CHANGE_PWR__PORT == 15 && ((CHANGE_PWR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    CHANGE_PWR_Write(uint8 value) ;
void    CHANGE_PWR_SetDriveMode(uint8 mode) ;
uint8   CHANGE_PWR_ReadDataReg(void) ;
uint8   CHANGE_PWR_Read(void) ;
uint8   CHANGE_PWR_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define CHANGE_PWR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define CHANGE_PWR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define CHANGE_PWR_DM_RES_UP          PIN_DM_RES_UP
#define CHANGE_PWR_DM_RES_DWN         PIN_DM_RES_DWN
#define CHANGE_PWR_DM_OD_LO           PIN_DM_OD_LO
#define CHANGE_PWR_DM_OD_HI           PIN_DM_OD_HI
#define CHANGE_PWR_DM_STRONG          PIN_DM_STRONG
#define CHANGE_PWR_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define CHANGE_PWR_MASK               CHANGE_PWR__MASK
#define CHANGE_PWR_SHIFT              CHANGE_PWR__SHIFT
#define CHANGE_PWR_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CHANGE_PWR_PS                     (* (reg8 *) CHANGE_PWR__PS)
/* Data Register */
#define CHANGE_PWR_DR                     (* (reg8 *) CHANGE_PWR__DR)
/* Port Number */
#define CHANGE_PWR_PRT_NUM                (* (reg8 *) CHANGE_PWR__PRT) 
/* Connect to Analog Globals */                                                  
#define CHANGE_PWR_AG                     (* (reg8 *) CHANGE_PWR__AG)                       
/* Analog MUX bux enable */
#define CHANGE_PWR_AMUX                   (* (reg8 *) CHANGE_PWR__AMUX) 
/* Bidirectional Enable */                                                        
#define CHANGE_PWR_BIE                    (* (reg8 *) CHANGE_PWR__BIE)
/* Bit-mask for Aliased Register Access */
#define CHANGE_PWR_BIT_MASK               (* (reg8 *) CHANGE_PWR__BIT_MASK)
/* Bypass Enable */
#define CHANGE_PWR_BYP                    (* (reg8 *) CHANGE_PWR__BYP)
/* Port wide control signals */                                                   
#define CHANGE_PWR_CTL                    (* (reg8 *) CHANGE_PWR__CTL)
/* Drive Modes */
#define CHANGE_PWR_DM0                    (* (reg8 *) CHANGE_PWR__DM0) 
#define CHANGE_PWR_DM1                    (* (reg8 *) CHANGE_PWR__DM1)
#define CHANGE_PWR_DM2                    (* (reg8 *) CHANGE_PWR__DM2) 
/* Input Buffer Disable Override */
#define CHANGE_PWR_INP_DIS                (* (reg8 *) CHANGE_PWR__INP_DIS)
/* LCD Common or Segment Drive */
#define CHANGE_PWR_LCD_COM_SEG            (* (reg8 *) CHANGE_PWR__LCD_COM_SEG)
/* Enable Segment LCD */
#define CHANGE_PWR_LCD_EN                 (* (reg8 *) CHANGE_PWR__LCD_EN)
/* Slew Rate Control */
#define CHANGE_PWR_SLW                    (* (reg8 *) CHANGE_PWR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CHANGE_PWR_PRTDSI__CAPS_SEL       (* (reg8 *) CHANGE_PWR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CHANGE_PWR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CHANGE_PWR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CHANGE_PWR_PRTDSI__OE_SEL0        (* (reg8 *) CHANGE_PWR__PRTDSI__OE_SEL0) 
#define CHANGE_PWR_PRTDSI__OE_SEL1        (* (reg8 *) CHANGE_PWR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CHANGE_PWR_PRTDSI__OUT_SEL0       (* (reg8 *) CHANGE_PWR__PRTDSI__OUT_SEL0) 
#define CHANGE_PWR_PRTDSI__OUT_SEL1       (* (reg8 *) CHANGE_PWR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CHANGE_PWR_PRTDSI__SYNC_OUT       (* (reg8 *) CHANGE_PWR__PRTDSI__SYNC_OUT) 


#if defined(CHANGE_PWR__INTSTAT)  /* Interrupt Registers */

    #define CHANGE_PWR_INTSTAT                (* (reg8 *) CHANGE_PWR__INTSTAT)
    #define CHANGE_PWR_SNAP                   (* (reg8 *) CHANGE_PWR__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CHANGE_PWR_H */


/* [] END OF FILE */

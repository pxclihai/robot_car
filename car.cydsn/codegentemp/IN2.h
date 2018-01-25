/*******************************************************************************
* File Name: IN2.h  
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

#if !defined(CY_PINS_IN2_H) /* Pins IN2_H */
#define CY_PINS_IN2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "IN2_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 IN2__PORT == 15 && ((IN2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    IN2_Write(uint8 value) ;
void    IN2_SetDriveMode(uint8 mode) ;
uint8   IN2_ReadDataReg(void) ;
uint8   IN2_Read(void) ;
uint8   IN2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define IN2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define IN2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define IN2_DM_RES_UP          PIN_DM_RES_UP
#define IN2_DM_RES_DWN         PIN_DM_RES_DWN
#define IN2_DM_OD_LO           PIN_DM_OD_LO
#define IN2_DM_OD_HI           PIN_DM_OD_HI
#define IN2_DM_STRONG          PIN_DM_STRONG
#define IN2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define IN2_MASK               IN2__MASK
#define IN2_SHIFT              IN2__SHIFT
#define IN2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define IN2_PS                     (* (reg8 *) IN2__PS)
/* Data Register */
#define IN2_DR                     (* (reg8 *) IN2__DR)
/* Port Number */
#define IN2_PRT_NUM                (* (reg8 *) IN2__PRT) 
/* Connect to Analog Globals */                                                  
#define IN2_AG                     (* (reg8 *) IN2__AG)                       
/* Analog MUX bux enable */
#define IN2_AMUX                   (* (reg8 *) IN2__AMUX) 
/* Bidirectional Enable */                                                        
#define IN2_BIE                    (* (reg8 *) IN2__BIE)
/* Bit-mask for Aliased Register Access */
#define IN2_BIT_MASK               (* (reg8 *) IN2__BIT_MASK)
/* Bypass Enable */
#define IN2_BYP                    (* (reg8 *) IN2__BYP)
/* Port wide control signals */                                                   
#define IN2_CTL                    (* (reg8 *) IN2__CTL)
/* Drive Modes */
#define IN2_DM0                    (* (reg8 *) IN2__DM0) 
#define IN2_DM1                    (* (reg8 *) IN2__DM1)
#define IN2_DM2                    (* (reg8 *) IN2__DM2) 
/* Input Buffer Disable Override */
#define IN2_INP_DIS                (* (reg8 *) IN2__INP_DIS)
/* LCD Common or Segment Drive */
#define IN2_LCD_COM_SEG            (* (reg8 *) IN2__LCD_COM_SEG)
/* Enable Segment LCD */
#define IN2_LCD_EN                 (* (reg8 *) IN2__LCD_EN)
/* Slew Rate Control */
#define IN2_SLW                    (* (reg8 *) IN2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define IN2_PRTDSI__CAPS_SEL       (* (reg8 *) IN2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define IN2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) IN2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define IN2_PRTDSI__OE_SEL0        (* (reg8 *) IN2__PRTDSI__OE_SEL0) 
#define IN2_PRTDSI__OE_SEL1        (* (reg8 *) IN2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define IN2_PRTDSI__OUT_SEL0       (* (reg8 *) IN2__PRTDSI__OUT_SEL0) 
#define IN2_PRTDSI__OUT_SEL1       (* (reg8 *) IN2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define IN2_PRTDSI__SYNC_OUT       (* (reg8 *) IN2__PRTDSI__SYNC_OUT) 


#if defined(IN2__INTSTAT)  /* Interrupt Registers */

    #define IN2_INTSTAT                (* (reg8 *) IN2__INTSTAT)
    #define IN2_SNAP                   (* (reg8 *) IN2__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_IN2_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: IN4.h  
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

#if !defined(CY_PINS_IN4_H) /* Pins IN4_H */
#define CY_PINS_IN4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "IN4_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 IN4__PORT == 15 && ((IN4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    IN4_Write(uint8 value) ;
void    IN4_SetDriveMode(uint8 mode) ;
uint8   IN4_ReadDataReg(void) ;
uint8   IN4_Read(void) ;
uint8   IN4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define IN4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define IN4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define IN4_DM_RES_UP          PIN_DM_RES_UP
#define IN4_DM_RES_DWN         PIN_DM_RES_DWN
#define IN4_DM_OD_LO           PIN_DM_OD_LO
#define IN4_DM_OD_HI           PIN_DM_OD_HI
#define IN4_DM_STRONG          PIN_DM_STRONG
#define IN4_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define IN4_MASK               IN4__MASK
#define IN4_SHIFT              IN4__SHIFT
#define IN4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define IN4_PS                     (* (reg8 *) IN4__PS)
/* Data Register */
#define IN4_DR                     (* (reg8 *) IN4__DR)
/* Port Number */
#define IN4_PRT_NUM                (* (reg8 *) IN4__PRT) 
/* Connect to Analog Globals */                                                  
#define IN4_AG                     (* (reg8 *) IN4__AG)                       
/* Analog MUX bux enable */
#define IN4_AMUX                   (* (reg8 *) IN4__AMUX) 
/* Bidirectional Enable */                                                        
#define IN4_BIE                    (* (reg8 *) IN4__BIE)
/* Bit-mask for Aliased Register Access */
#define IN4_BIT_MASK               (* (reg8 *) IN4__BIT_MASK)
/* Bypass Enable */
#define IN4_BYP                    (* (reg8 *) IN4__BYP)
/* Port wide control signals */                                                   
#define IN4_CTL                    (* (reg8 *) IN4__CTL)
/* Drive Modes */
#define IN4_DM0                    (* (reg8 *) IN4__DM0) 
#define IN4_DM1                    (* (reg8 *) IN4__DM1)
#define IN4_DM2                    (* (reg8 *) IN4__DM2) 
/* Input Buffer Disable Override */
#define IN4_INP_DIS                (* (reg8 *) IN4__INP_DIS)
/* LCD Common or Segment Drive */
#define IN4_LCD_COM_SEG            (* (reg8 *) IN4__LCD_COM_SEG)
/* Enable Segment LCD */
#define IN4_LCD_EN                 (* (reg8 *) IN4__LCD_EN)
/* Slew Rate Control */
#define IN4_SLW                    (* (reg8 *) IN4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define IN4_PRTDSI__CAPS_SEL       (* (reg8 *) IN4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define IN4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) IN4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define IN4_PRTDSI__OE_SEL0        (* (reg8 *) IN4__PRTDSI__OE_SEL0) 
#define IN4_PRTDSI__OE_SEL1        (* (reg8 *) IN4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define IN4_PRTDSI__OUT_SEL0       (* (reg8 *) IN4__PRTDSI__OUT_SEL0) 
#define IN4_PRTDSI__OUT_SEL1       (* (reg8 *) IN4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define IN4_PRTDSI__SYNC_OUT       (* (reg8 *) IN4__PRTDSI__SYNC_OUT) 


#if defined(IN4__INTSTAT)  /* Interrupt Registers */

    #define IN4_INTSTAT                (* (reg8 *) IN4__INTSTAT)
    #define IN4_SNAP                   (* (reg8 *) IN4__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_IN4_H */


/* [] END OF FILE */

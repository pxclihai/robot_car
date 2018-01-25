/*******************************************************************************
* File Name: IN3.h  
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

#if !defined(CY_PINS_IN3_H) /* Pins IN3_H */
#define CY_PINS_IN3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "IN3_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 IN3__PORT == 15 && ((IN3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    IN3_Write(uint8 value) ;
void    IN3_SetDriveMode(uint8 mode) ;
uint8   IN3_ReadDataReg(void) ;
uint8   IN3_Read(void) ;
uint8   IN3_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define IN3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define IN3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define IN3_DM_RES_UP          PIN_DM_RES_UP
#define IN3_DM_RES_DWN         PIN_DM_RES_DWN
#define IN3_DM_OD_LO           PIN_DM_OD_LO
#define IN3_DM_OD_HI           PIN_DM_OD_HI
#define IN3_DM_STRONG          PIN_DM_STRONG
#define IN3_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define IN3_MASK               IN3__MASK
#define IN3_SHIFT              IN3__SHIFT
#define IN3_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define IN3_PS                     (* (reg8 *) IN3__PS)
/* Data Register */
#define IN3_DR                     (* (reg8 *) IN3__DR)
/* Port Number */
#define IN3_PRT_NUM                (* (reg8 *) IN3__PRT) 
/* Connect to Analog Globals */                                                  
#define IN3_AG                     (* (reg8 *) IN3__AG)                       
/* Analog MUX bux enable */
#define IN3_AMUX                   (* (reg8 *) IN3__AMUX) 
/* Bidirectional Enable */                                                        
#define IN3_BIE                    (* (reg8 *) IN3__BIE)
/* Bit-mask for Aliased Register Access */
#define IN3_BIT_MASK               (* (reg8 *) IN3__BIT_MASK)
/* Bypass Enable */
#define IN3_BYP                    (* (reg8 *) IN3__BYP)
/* Port wide control signals */                                                   
#define IN3_CTL                    (* (reg8 *) IN3__CTL)
/* Drive Modes */
#define IN3_DM0                    (* (reg8 *) IN3__DM0) 
#define IN3_DM1                    (* (reg8 *) IN3__DM1)
#define IN3_DM2                    (* (reg8 *) IN3__DM2) 
/* Input Buffer Disable Override */
#define IN3_INP_DIS                (* (reg8 *) IN3__INP_DIS)
/* LCD Common or Segment Drive */
#define IN3_LCD_COM_SEG            (* (reg8 *) IN3__LCD_COM_SEG)
/* Enable Segment LCD */
#define IN3_LCD_EN                 (* (reg8 *) IN3__LCD_EN)
/* Slew Rate Control */
#define IN3_SLW                    (* (reg8 *) IN3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define IN3_PRTDSI__CAPS_SEL       (* (reg8 *) IN3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define IN3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) IN3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define IN3_PRTDSI__OE_SEL0        (* (reg8 *) IN3__PRTDSI__OE_SEL0) 
#define IN3_PRTDSI__OE_SEL1        (* (reg8 *) IN3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define IN3_PRTDSI__OUT_SEL0       (* (reg8 *) IN3__PRTDSI__OUT_SEL0) 
#define IN3_PRTDSI__OUT_SEL1       (* (reg8 *) IN3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define IN3_PRTDSI__SYNC_OUT       (* (reg8 *) IN3__PRTDSI__SYNC_OUT) 


#if defined(IN3__INTSTAT)  /* Interrupt Registers */

    #define IN3_INTSTAT                (* (reg8 *) IN3__INTSTAT)
    #define IN3_SNAP                   (* (reg8 *) IN3__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_IN3_H */


/* [] END OF FILE */

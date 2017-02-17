/*******************************************************************************
* File Name: M_DOWN.h  
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

#if !defined(CY_PINS_M_DOWN_H) /* Pins M_DOWN_H */
#define CY_PINS_M_DOWN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M_DOWN_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M_DOWN__PORT == 15 && ((M_DOWN__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    M_DOWN_Write(uint8 value) ;
void    M_DOWN_SetDriveMode(uint8 mode) ;
uint8   M_DOWN_ReadDataReg(void) ;
uint8   M_DOWN_Read(void) ;
uint8   M_DOWN_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define M_DOWN_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define M_DOWN_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define M_DOWN_DM_RES_UP          PIN_DM_RES_UP
#define M_DOWN_DM_RES_DWN         PIN_DM_RES_DWN
#define M_DOWN_DM_OD_LO           PIN_DM_OD_LO
#define M_DOWN_DM_OD_HI           PIN_DM_OD_HI
#define M_DOWN_DM_STRONG          PIN_DM_STRONG
#define M_DOWN_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define M_DOWN_MASK               M_DOWN__MASK
#define M_DOWN_SHIFT              M_DOWN__SHIFT
#define M_DOWN_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M_DOWN_PS                     (* (reg8 *) M_DOWN__PS)
/* Data Register */
#define M_DOWN_DR                     (* (reg8 *) M_DOWN__DR)
/* Port Number */
#define M_DOWN_PRT_NUM                (* (reg8 *) M_DOWN__PRT) 
/* Connect to Analog Globals */                                                  
#define M_DOWN_AG                     (* (reg8 *) M_DOWN__AG)                       
/* Analog MUX bux enable */
#define M_DOWN_AMUX                   (* (reg8 *) M_DOWN__AMUX) 
/* Bidirectional Enable */                                                        
#define M_DOWN_BIE                    (* (reg8 *) M_DOWN__BIE)
/* Bit-mask for Aliased Register Access */
#define M_DOWN_BIT_MASK               (* (reg8 *) M_DOWN__BIT_MASK)
/* Bypass Enable */
#define M_DOWN_BYP                    (* (reg8 *) M_DOWN__BYP)
/* Port wide control signals */                                                   
#define M_DOWN_CTL                    (* (reg8 *) M_DOWN__CTL)
/* Drive Modes */
#define M_DOWN_DM0                    (* (reg8 *) M_DOWN__DM0) 
#define M_DOWN_DM1                    (* (reg8 *) M_DOWN__DM1)
#define M_DOWN_DM2                    (* (reg8 *) M_DOWN__DM2) 
/* Input Buffer Disable Override */
#define M_DOWN_INP_DIS                (* (reg8 *) M_DOWN__INP_DIS)
/* LCD Common or Segment Drive */
#define M_DOWN_LCD_COM_SEG            (* (reg8 *) M_DOWN__LCD_COM_SEG)
/* Enable Segment LCD */
#define M_DOWN_LCD_EN                 (* (reg8 *) M_DOWN__LCD_EN)
/* Slew Rate Control */
#define M_DOWN_SLW                    (* (reg8 *) M_DOWN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M_DOWN_PRTDSI__CAPS_SEL       (* (reg8 *) M_DOWN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M_DOWN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M_DOWN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M_DOWN_PRTDSI__OE_SEL0        (* (reg8 *) M_DOWN__PRTDSI__OE_SEL0) 
#define M_DOWN_PRTDSI__OE_SEL1        (* (reg8 *) M_DOWN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M_DOWN_PRTDSI__OUT_SEL0       (* (reg8 *) M_DOWN__PRTDSI__OUT_SEL0) 
#define M_DOWN_PRTDSI__OUT_SEL1       (* (reg8 *) M_DOWN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M_DOWN_PRTDSI__SYNC_OUT       (* (reg8 *) M_DOWN__PRTDSI__SYNC_OUT) 


#if defined(M_DOWN__INTSTAT)  /* Interrupt Registers */

    #define M_DOWN_INTSTAT                (* (reg8 *) M_DOWN__INTSTAT)
    #define M_DOWN_SNAP                   (* (reg8 *) M_DOWN__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M_DOWN_H */


/* [] END OF FILE */

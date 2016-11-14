/*******************************************************************************
* File Name: debug_time.h  
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

#if !defined(CY_PINS_debug_time_H) /* Pins debug_time_H */
#define CY_PINS_debug_time_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "debug_time_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 debug_time__PORT == 15 && ((debug_time__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    debug_time_Write(uint8 value) ;
void    debug_time_SetDriveMode(uint8 mode) ;
uint8   debug_time_ReadDataReg(void) ;
uint8   debug_time_Read(void) ;
uint8   debug_time_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define debug_time_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define debug_time_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define debug_time_DM_RES_UP          PIN_DM_RES_UP
#define debug_time_DM_RES_DWN         PIN_DM_RES_DWN
#define debug_time_DM_OD_LO           PIN_DM_OD_LO
#define debug_time_DM_OD_HI           PIN_DM_OD_HI
#define debug_time_DM_STRONG          PIN_DM_STRONG
#define debug_time_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define debug_time_MASK               debug_time__MASK
#define debug_time_SHIFT              debug_time__SHIFT
#define debug_time_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define debug_time_PS                     (* (reg8 *) debug_time__PS)
/* Data Register */
#define debug_time_DR                     (* (reg8 *) debug_time__DR)
/* Port Number */
#define debug_time_PRT_NUM                (* (reg8 *) debug_time__PRT) 
/* Connect to Analog Globals */                                                  
#define debug_time_AG                     (* (reg8 *) debug_time__AG)                       
/* Analog MUX bux enable */
#define debug_time_AMUX                   (* (reg8 *) debug_time__AMUX) 
/* Bidirectional Enable */                                                        
#define debug_time_BIE                    (* (reg8 *) debug_time__BIE)
/* Bit-mask for Aliased Register Access */
#define debug_time_BIT_MASK               (* (reg8 *) debug_time__BIT_MASK)
/* Bypass Enable */
#define debug_time_BYP                    (* (reg8 *) debug_time__BYP)
/* Port wide control signals */                                                   
#define debug_time_CTL                    (* (reg8 *) debug_time__CTL)
/* Drive Modes */
#define debug_time_DM0                    (* (reg8 *) debug_time__DM0) 
#define debug_time_DM1                    (* (reg8 *) debug_time__DM1)
#define debug_time_DM2                    (* (reg8 *) debug_time__DM2) 
/* Input Buffer Disable Override */
#define debug_time_INP_DIS                (* (reg8 *) debug_time__INP_DIS)
/* LCD Common or Segment Drive */
#define debug_time_LCD_COM_SEG            (* (reg8 *) debug_time__LCD_COM_SEG)
/* Enable Segment LCD */
#define debug_time_LCD_EN                 (* (reg8 *) debug_time__LCD_EN)
/* Slew Rate Control */
#define debug_time_SLW                    (* (reg8 *) debug_time__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define debug_time_PRTDSI__CAPS_SEL       (* (reg8 *) debug_time__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define debug_time_PRTDSI__DBL_SYNC_IN    (* (reg8 *) debug_time__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define debug_time_PRTDSI__OE_SEL0        (* (reg8 *) debug_time__PRTDSI__OE_SEL0) 
#define debug_time_PRTDSI__OE_SEL1        (* (reg8 *) debug_time__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define debug_time_PRTDSI__OUT_SEL0       (* (reg8 *) debug_time__PRTDSI__OUT_SEL0) 
#define debug_time_PRTDSI__OUT_SEL1       (* (reg8 *) debug_time__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define debug_time_PRTDSI__SYNC_OUT       (* (reg8 *) debug_time__PRTDSI__SYNC_OUT) 


#if defined(debug_time__INTSTAT)  /* Interrupt Registers */

    #define debug_time_INTSTAT                (* (reg8 *) debug_time__INTSTAT)
    #define debug_time_SNAP                   (* (reg8 *) debug_time__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_debug_time_H */


/* [] END OF FILE */

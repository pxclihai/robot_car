/*******************************************************************************
* File Name: CONTRAL_LIDAR.h  
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

#if !defined(CY_PINS_CONTRAL_LIDAR_H) /* Pins CONTRAL_LIDAR_H */
#define CY_PINS_CONTRAL_LIDAR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CONTRAL_LIDAR_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CONTRAL_LIDAR__PORT == 15 && ((CONTRAL_LIDAR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    CONTRAL_LIDAR_Write(uint8 value) ;
void    CONTRAL_LIDAR_SetDriveMode(uint8 mode) ;
uint8   CONTRAL_LIDAR_ReadDataReg(void) ;
uint8   CONTRAL_LIDAR_Read(void) ;
uint8   CONTRAL_LIDAR_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define CONTRAL_LIDAR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define CONTRAL_LIDAR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define CONTRAL_LIDAR_DM_RES_UP          PIN_DM_RES_UP
#define CONTRAL_LIDAR_DM_RES_DWN         PIN_DM_RES_DWN
#define CONTRAL_LIDAR_DM_OD_LO           PIN_DM_OD_LO
#define CONTRAL_LIDAR_DM_OD_HI           PIN_DM_OD_HI
#define CONTRAL_LIDAR_DM_STRONG          PIN_DM_STRONG
#define CONTRAL_LIDAR_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define CONTRAL_LIDAR_MASK               CONTRAL_LIDAR__MASK
#define CONTRAL_LIDAR_SHIFT              CONTRAL_LIDAR__SHIFT
#define CONTRAL_LIDAR_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CONTRAL_LIDAR_PS                     (* (reg8 *) CONTRAL_LIDAR__PS)
/* Data Register */
#define CONTRAL_LIDAR_DR                     (* (reg8 *) CONTRAL_LIDAR__DR)
/* Port Number */
#define CONTRAL_LIDAR_PRT_NUM                (* (reg8 *) CONTRAL_LIDAR__PRT) 
/* Connect to Analog Globals */                                                  
#define CONTRAL_LIDAR_AG                     (* (reg8 *) CONTRAL_LIDAR__AG)                       
/* Analog MUX bux enable */
#define CONTRAL_LIDAR_AMUX                   (* (reg8 *) CONTRAL_LIDAR__AMUX) 
/* Bidirectional Enable */                                                        
#define CONTRAL_LIDAR_BIE                    (* (reg8 *) CONTRAL_LIDAR__BIE)
/* Bit-mask for Aliased Register Access */
#define CONTRAL_LIDAR_BIT_MASK               (* (reg8 *) CONTRAL_LIDAR__BIT_MASK)
/* Bypass Enable */
#define CONTRAL_LIDAR_BYP                    (* (reg8 *) CONTRAL_LIDAR__BYP)
/* Port wide control signals */                                                   
#define CONTRAL_LIDAR_CTL                    (* (reg8 *) CONTRAL_LIDAR__CTL)
/* Drive Modes */
#define CONTRAL_LIDAR_DM0                    (* (reg8 *) CONTRAL_LIDAR__DM0) 
#define CONTRAL_LIDAR_DM1                    (* (reg8 *) CONTRAL_LIDAR__DM1)
#define CONTRAL_LIDAR_DM2                    (* (reg8 *) CONTRAL_LIDAR__DM2) 
/* Input Buffer Disable Override */
#define CONTRAL_LIDAR_INP_DIS                (* (reg8 *) CONTRAL_LIDAR__INP_DIS)
/* LCD Common or Segment Drive */
#define CONTRAL_LIDAR_LCD_COM_SEG            (* (reg8 *) CONTRAL_LIDAR__LCD_COM_SEG)
/* Enable Segment LCD */
#define CONTRAL_LIDAR_LCD_EN                 (* (reg8 *) CONTRAL_LIDAR__LCD_EN)
/* Slew Rate Control */
#define CONTRAL_LIDAR_SLW                    (* (reg8 *) CONTRAL_LIDAR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CONTRAL_LIDAR_PRTDSI__CAPS_SEL       (* (reg8 *) CONTRAL_LIDAR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CONTRAL_LIDAR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CONTRAL_LIDAR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CONTRAL_LIDAR_PRTDSI__OE_SEL0        (* (reg8 *) CONTRAL_LIDAR__PRTDSI__OE_SEL0) 
#define CONTRAL_LIDAR_PRTDSI__OE_SEL1        (* (reg8 *) CONTRAL_LIDAR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CONTRAL_LIDAR_PRTDSI__OUT_SEL0       (* (reg8 *) CONTRAL_LIDAR__PRTDSI__OUT_SEL0) 
#define CONTRAL_LIDAR_PRTDSI__OUT_SEL1       (* (reg8 *) CONTRAL_LIDAR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CONTRAL_LIDAR_PRTDSI__SYNC_OUT       (* (reg8 *) CONTRAL_LIDAR__PRTDSI__SYNC_OUT) 


#if defined(CONTRAL_LIDAR__INTSTAT)  /* Interrupt Registers */

    #define CONTRAL_LIDAR_INTSTAT                (* (reg8 *) CONTRAL_LIDAR__INTSTAT)
    #define CONTRAL_LIDAR_SNAP                   (* (reg8 *) CONTRAL_LIDAR__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CONTRAL_LIDAR_H */


/* [] END OF FILE */

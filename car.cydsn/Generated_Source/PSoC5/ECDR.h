/*******************************************************************************
* File Name: ECDR.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_QUADRATURE_DECODER_ECDR_H)
#define CY_QUADRATURE_DECODER_ECDR_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define ECDR_COUNTER_SIZE               (16u)
#define ECDR_COUNTER_SIZE_8_BIT         (8u)
#define ECDR_COUNTER_SIZE_16_BIT        (16u)
#define ECDR_COUNTER_SIZE_32_BIT        (32u)

#if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
    #include "ECDR_Cnt8.h"
#else 
    /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) || 
    *  (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT) 
    */
    #include "ECDR_Cnt16.h"
#endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT */

extern uint8 ECDR_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define ECDR_COUNTER_RESOLUTION         (1u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} ECDR_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  ECDR_Init(void) ;
void  ECDR_Start(void) ;
void  ECDR_Stop(void) ;
void  ECDR_Enable(void) ;
uint8 ECDR_GetEvents(void) ;
void  ECDR_SetInterruptMask(uint8 mask) ;
uint8 ECDR_GetInterruptMask(void) ;
int16 ECDR_GetCounter(void) ;
void  ECDR_SetCounter(int16 value)
;
void  ECDR_Sleep(void) ;
void  ECDR_Wakeup(void) ;
void  ECDR_SaveConfig(void) ;
void  ECDR_RestoreConfig(void) ;

#if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(ECDR_ISR);
#endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
    #define ECDR_ISR_NUMBER             ((uint8) ECDR_isr__INTC_NUMBER)
    #define ECDR_ISR_PRIORITY           ((uint8) ECDR_isr__INTC_PRIOR_NUM)
#endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define ECDR_GLITCH_FILTERING           (1u)
#define ECDR_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT)
    #define ECDR_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_16_BIT) ||
    *  (ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_32_BIT)
    */
    #define ECDR_COUNTER_INIT_VALUE    (0x8000u)
    #define ECDR_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* ECDR_COUNTER_SIZE == ECDR_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define ECDR_STATUS_REG                 (* (reg8 *) ECDR_bQuadDec_Stsreg__STATUS_REG)
#define ECDR_STATUS_PTR                 (  (reg8 *) ECDR_bQuadDec_Stsreg__STATUS_REG)
#define ECDR_STATUS_MASK                (* (reg8 *) ECDR_bQuadDec_Stsreg__MASK_REG)
#define ECDR_STATUS_MASK_PTR            (  (reg8 *) ECDR_bQuadDec_Stsreg__MASK_REG)
#define ECDR_SR_AUX_CONTROL             (* (reg8 *) ECDR_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define ECDR_SR_AUX_CONTROL_PTR         (  (reg8 *) ECDR_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define ECDR_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define ECDR_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define ECDR_COUNTER_RESET_SHIFT        (0x02u)
#define ECDR_INVALID_IN_SHIFT           (0x03u)
#define ECDR_COUNTER_OVERFLOW           ((uint8) (0x01u << ECDR_COUNTER_OVERFLOW_SHIFT))
#define ECDR_COUNTER_UNDERFLOW          ((uint8) (0x01u << ECDR_COUNTER_UNDERFLOW_SHIFT))
#define ECDR_COUNTER_RESET              ((uint8) (0x01u << ECDR_COUNTER_RESET_SHIFT))
#define ECDR_INVALID_IN                 ((uint8) (0x01u << ECDR_INVALID_IN_SHIFT))

#define ECDR_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define ECDR_INTERRUPTS_ENABLE          ((uint8)(0x01u << ECDR_INTERRUPTS_ENABLE_SHIFT))
#define ECDR_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define ECDR_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_ECDR_H */


/* [] END OF FILE */

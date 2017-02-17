/*******************************************************************************
* File Name: .h
* Version 3.50
*
* Description:
*  This file provides private constants and parameter values for the I2C
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_I2C_PVT_Ultrasound_H)
#define CY_I2C_PVT_Ultrasound_H

#include "Ultrasound.h"

#define Ultrasound_TIMEOUT_ENABLED_INC    (0u)
#if (0u != Ultrasound_TIMEOUT_ENABLED_INC)
    #include "Ultrasound_TMOUT.h"
#endif /* (0u != Ultrasound_TIMEOUT_ENABLED_INC) */


/**********************************
*   Variables with external linkage
**********************************/

extern Ultrasound_BACKUP_STRUCT Ultrasound_backup;

extern volatile uint8 Ultrasound_state;   /* Current state of I2C FSM */

/* Master variables */
#if (Ultrasound_MODE_MASTER_ENABLED)
    extern volatile uint8 Ultrasound_mstrStatus;   /* Master Status byte  */
    extern volatile uint8 Ultrasound_mstrControl;  /* Master Control byte */

    /* Transmit buffer variables */
    extern volatile uint8 * Ultrasound_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint8   Ultrasound_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint8   Ultrasound_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Receive buffer variables */
    extern volatile uint8 * Ultrasound_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint8   Ultrasound_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint8   Ultrasound_mstrWrBufIndex; /* Master Write buffer Index      */

#endif /* (Ultrasound_MODE_MASTER_ENABLED) */

/* Slave variables */
#if (Ultrasound_MODE_SLAVE_ENABLED)
    extern volatile uint8 Ultrasound_slStatus;         /* Slave Status  */

    /* Transmit buffer variables */
    extern volatile uint8 * Ultrasound_slRdBufPtr;     /* Pointer to Transmit buffer  */
    extern volatile uint8   Ultrasound_slRdBufSize;    /* Slave Transmit buffer size  */
    extern volatile uint8   Ultrasound_slRdBufIndex;   /* Slave Transmit buffer Index */

    /* Receive buffer variables */
    extern volatile uint8 * Ultrasound_slWrBufPtr;     /* Pointer to Receive buffer  */
    extern volatile uint8   Ultrasound_slWrBufSize;    /* Slave Receive buffer size  */
    extern volatile uint8   Ultrasound_slWrBufIndex;   /* Slave Receive buffer Index */

    #if (Ultrasound_SW_ADRR_DECODE)
        extern volatile uint8 Ultrasound_slAddress;     /* Software address variable */
    #endif   /* (Ultrasound_SW_ADRR_DECODE) */

#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */

#if ((Ultrasound_FF_IMPLEMENTED) && (Ultrasound_WAKEUP_ENABLED))
    extern volatile uint8 Ultrasound_wakeupSource;
#endif /* ((Ultrasound_FF_IMPLEMENTED) && (Ultrasound_WAKEUP_ENABLED)) */


#endif /* CY_I2C_PVT_Ultrasound_H */


/* [] END OF FILE */

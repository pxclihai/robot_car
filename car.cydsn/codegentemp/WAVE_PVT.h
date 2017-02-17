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

#if !defined(CY_I2C_PVT_WAVE_H)
#define CY_I2C_PVT_WAVE_H

#include "WAVE.h"

#define WAVE_TIMEOUT_ENABLED_INC    (0u)
#if (0u != WAVE_TIMEOUT_ENABLED_INC)
    #include "WAVE_TMOUT.h"
#endif /* (0u != WAVE_TIMEOUT_ENABLED_INC) */


/**********************************
*   Variables with external linkage
**********************************/

extern WAVE_BACKUP_STRUCT WAVE_backup;

extern volatile uint8 WAVE_state;   /* Current state of I2C FSM */

/* Master variables */
#if (WAVE_MODE_MASTER_ENABLED)
    extern volatile uint8 WAVE_mstrStatus;   /* Master Status byte  */
    extern volatile uint8 WAVE_mstrControl;  /* Master Control byte */

    /* Transmit buffer variables */
    extern volatile uint8 * WAVE_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint8   WAVE_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint8   WAVE_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Receive buffer variables */
    extern volatile uint8 * WAVE_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint8   WAVE_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint8   WAVE_mstrWrBufIndex; /* Master Write buffer Index      */

#endif /* (WAVE_MODE_MASTER_ENABLED) */

/* Slave variables */
#if (WAVE_MODE_SLAVE_ENABLED)
    extern volatile uint8 WAVE_slStatus;         /* Slave Status  */

    /* Transmit buffer variables */
    extern volatile uint8 * WAVE_slRdBufPtr;     /* Pointer to Transmit buffer  */
    extern volatile uint8   WAVE_slRdBufSize;    /* Slave Transmit buffer size  */
    extern volatile uint8   WAVE_slRdBufIndex;   /* Slave Transmit buffer Index */

    /* Receive buffer variables */
    extern volatile uint8 * WAVE_slWrBufPtr;     /* Pointer to Receive buffer  */
    extern volatile uint8   WAVE_slWrBufSize;    /* Slave Receive buffer size  */
    extern volatile uint8   WAVE_slWrBufIndex;   /* Slave Receive buffer Index */

    #if (WAVE_SW_ADRR_DECODE)
        extern volatile uint8 WAVE_slAddress;     /* Software address variable */
    #endif   /* (WAVE_SW_ADRR_DECODE) */

#endif /* (WAVE_MODE_SLAVE_ENABLED) */

#if ((WAVE_FF_IMPLEMENTED) && (WAVE_WAKEUP_ENABLED))
    extern volatile uint8 WAVE_wakeupSource;
#endif /* ((WAVE_FF_IMPLEMENTED) && (WAVE_WAKEUP_ENABLED)) */


#endif /* CY_I2C_PVT_WAVE_H */


/* [] END OF FILE */

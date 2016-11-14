/*******************************************************************************
* File Name: UART_LIDAR.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_UART_LIDAR_H)
#define CY_UART_UART_LIDAR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

#define UART_LIDAR_RX_ENABLED                     (1u)
#define UART_LIDAR_TX_ENABLED                     (1u)
#define UART_LIDAR_HD_ENABLED                     (0u)
#define UART_LIDAR_RX_INTERRUPT_ENABLED           (1u)
#define UART_LIDAR_TX_INTERRUPT_ENABLED           (1u)
#define UART_LIDAR_INTERNAL_CLOCK_USED            (1u)
#define UART_LIDAR_RXHW_ADDRESS_ENABLED           (0u)
#define UART_LIDAR_OVER_SAMPLE_COUNT              (8u)
#define UART_LIDAR_PARITY_TYPE                    (0u)
#define UART_LIDAR_PARITY_TYPE_SW                 (0u)
#define UART_LIDAR_BREAK_DETECT                   (0u)
#define UART_LIDAR_BREAK_BITS_TX                  (13u)
#define UART_LIDAR_BREAK_BITS_RX                  (13u)
#define UART_LIDAR_TXCLKGEN_DP                    (1u)
#define UART_LIDAR_USE23POLLING                   (1u)
#define UART_LIDAR_FLOW_CONTROL                   (0u)
#define UART_LIDAR_CLK_FREQ                       (0u)
#define UART_LIDAR_TX_BUFFER_SIZE                 (5u)
#define UART_LIDAR_RX_BUFFER_SIZE                 (5u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define UART_LIDAR_CONTROL_REG_REMOVED            (0u)
#else
    #define UART_LIDAR_CONTROL_REG_REMOVED            (1u)
#endif /* End UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct UART_LIDAR_backupStruct_
{
    uint8 enableState;

    #if(UART_LIDAR_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End UART_LIDAR_CONTROL_REG_REMOVED */

} UART_LIDAR_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void UART_LIDAR_Start(void) ;
void UART_LIDAR_Stop(void) ;
uint8 UART_LIDAR_ReadControlRegister(void) ;
void UART_LIDAR_WriteControlRegister(uint8 control) ;

void UART_LIDAR_Init(void) ;
void UART_LIDAR_Enable(void) ;
void UART_LIDAR_SaveConfig(void) ;
void UART_LIDAR_RestoreConfig(void) ;
void UART_LIDAR_Sleep(void) ;
void UART_LIDAR_Wakeup(void) ;

/* Only if RX is enabled */
#if( (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) )

    #if (UART_LIDAR_RX_INTERRUPT_ENABLED)
        #define UART_LIDAR_EnableRxInt()  CyIntEnable (UART_LIDAR_RX_VECT_NUM)
        #define UART_LIDAR_DisableRxInt() CyIntDisable(UART_LIDAR_RX_VECT_NUM)
        CY_ISR_PROTO(UART_LIDAR_RXISR);
    #endif /* UART_LIDAR_RX_INTERRUPT_ENABLED */

    void UART_LIDAR_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void UART_LIDAR_SetRxAddress1(uint8 address) ;
    void UART_LIDAR_SetRxAddress2(uint8 address) ;

    void  UART_LIDAR_SetRxInterruptMode(uint8 intSrc) ;
    uint8 UART_LIDAR_ReadRxData(void) ;
    uint8 UART_LIDAR_ReadRxStatus(void) ;
    uint8 UART_LIDAR_GetChar(void) ;
    uint16 UART_LIDAR_GetByte(void) ;
    uint8 UART_LIDAR_GetRxBufferSize(void)
                                                            ;
    void UART_LIDAR_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define UART_LIDAR_GetRxInterruptSource   UART_LIDAR_ReadRxStatus

#endif /* End (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) */

/* Only if TX is enabled */
#if(UART_LIDAR_TX_ENABLED || UART_LIDAR_HD_ENABLED)

    #if(UART_LIDAR_TX_INTERRUPT_ENABLED)
        #define UART_LIDAR_EnableTxInt()  CyIntEnable (UART_LIDAR_TX_VECT_NUM)
        #define UART_LIDAR_DisableTxInt() CyIntDisable(UART_LIDAR_TX_VECT_NUM)
        #define UART_LIDAR_SetPendingTxInt() CyIntSetPending(UART_LIDAR_TX_VECT_NUM)
        #define UART_LIDAR_ClearPendingTxInt() CyIntClearPending(UART_LIDAR_TX_VECT_NUM)
        CY_ISR_PROTO(UART_LIDAR_TXISR);
    #endif /* UART_LIDAR_TX_INTERRUPT_ENABLED */

    void UART_LIDAR_SetTxInterruptMode(uint8 intSrc) ;
    void UART_LIDAR_WriteTxData(uint8 txDataByte) ;
    uint8 UART_LIDAR_ReadTxStatus(void) ;
    void UART_LIDAR_PutChar(uint8 txDataByte) ;
    void UART_LIDAR_PutString(const char8 string[]) ;
    void UART_LIDAR_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void UART_LIDAR_PutCRLF(uint8 txDataByte) ;
    void UART_LIDAR_ClearTxBuffer(void) ;
    void UART_LIDAR_SetTxAddressMode(uint8 addressMode) ;
    void UART_LIDAR_SendBreak(uint8 retMode) ;
    uint8 UART_LIDAR_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define UART_LIDAR_PutStringConst         UART_LIDAR_PutString
    #define UART_LIDAR_PutArrayConst          UART_LIDAR_PutArray
    #define UART_LIDAR_GetTxInterruptSource   UART_LIDAR_ReadTxStatus

#endif /* End UART_LIDAR_TX_ENABLED || UART_LIDAR_HD_ENABLED */

#if(UART_LIDAR_HD_ENABLED)
    void UART_LIDAR_LoadRxConfig(void) ;
    void UART_LIDAR_LoadTxConfig(void) ;
#endif /* End UART_LIDAR_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART_LIDAR) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    UART_LIDAR_CyBtldrCommStart(void) CYSMALL ;
    void    UART_LIDAR_CyBtldrCommStop(void) CYSMALL ;
    void    UART_LIDAR_CyBtldrCommReset(void) CYSMALL ;
    cystatus UART_LIDAR_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus UART_LIDAR_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART_LIDAR)
        #define CyBtldrCommStart    UART_LIDAR_CyBtldrCommStart
        #define CyBtldrCommStop     UART_LIDAR_CyBtldrCommStop
        #define CyBtldrCommReset    UART_LIDAR_CyBtldrCommReset
        #define CyBtldrCommWrite    UART_LIDAR_CyBtldrCommWrite
        #define CyBtldrCommRead     UART_LIDAR_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART_LIDAR) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define UART_LIDAR_BYTE2BYTE_TIME_OUT (25u)
    #define UART_LIDAR_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define UART_LIDAR_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define UART_LIDAR_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define UART_LIDAR_SET_SPACE      (0x00u)
#define UART_LIDAR_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (UART_LIDAR_TX_ENABLED) || (UART_LIDAR_HD_ENABLED) )
    #if(UART_LIDAR_TX_INTERRUPT_ENABLED)
        #define UART_LIDAR_TX_VECT_NUM            (uint8)UART_LIDAR_TXInternalInterrupt__INTC_NUMBER
        #define UART_LIDAR_TX_PRIOR_NUM           (uint8)UART_LIDAR_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* UART_LIDAR_TX_INTERRUPT_ENABLED */

    #define UART_LIDAR_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define UART_LIDAR_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define UART_LIDAR_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(UART_LIDAR_TX_ENABLED)
        #define UART_LIDAR_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (UART_LIDAR_HD_ENABLED) */
        #define UART_LIDAR_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (UART_LIDAR_TX_ENABLED) */

    #define UART_LIDAR_TX_STS_COMPLETE            (uint8)(0x01u << UART_LIDAR_TX_STS_COMPLETE_SHIFT)
    #define UART_LIDAR_TX_STS_FIFO_EMPTY          (uint8)(0x01u << UART_LIDAR_TX_STS_FIFO_EMPTY_SHIFT)
    #define UART_LIDAR_TX_STS_FIFO_FULL           (uint8)(0x01u << UART_LIDAR_TX_STS_FIFO_FULL_SHIFT)
    #define UART_LIDAR_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << UART_LIDAR_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (UART_LIDAR_TX_ENABLED) || (UART_LIDAR_HD_ENABLED)*/

#if( (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) )
    #if(UART_LIDAR_RX_INTERRUPT_ENABLED)
        #define UART_LIDAR_RX_VECT_NUM            (uint8)UART_LIDAR_RXInternalInterrupt__INTC_NUMBER
        #define UART_LIDAR_RX_PRIOR_NUM           (uint8)UART_LIDAR_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* UART_LIDAR_RX_INTERRUPT_ENABLED */
    #define UART_LIDAR_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define UART_LIDAR_RX_STS_BREAK_SHIFT             (0x01u)
    #define UART_LIDAR_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define UART_LIDAR_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define UART_LIDAR_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define UART_LIDAR_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define UART_LIDAR_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define UART_LIDAR_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define UART_LIDAR_RX_STS_MRKSPC           (uint8)(0x01u << UART_LIDAR_RX_STS_MRKSPC_SHIFT)
    #define UART_LIDAR_RX_STS_BREAK            (uint8)(0x01u << UART_LIDAR_RX_STS_BREAK_SHIFT)
    #define UART_LIDAR_RX_STS_PAR_ERROR        (uint8)(0x01u << UART_LIDAR_RX_STS_PAR_ERROR_SHIFT)
    #define UART_LIDAR_RX_STS_STOP_ERROR       (uint8)(0x01u << UART_LIDAR_RX_STS_STOP_ERROR_SHIFT)
    #define UART_LIDAR_RX_STS_OVERRUN          (uint8)(0x01u << UART_LIDAR_RX_STS_OVERRUN_SHIFT)
    #define UART_LIDAR_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << UART_LIDAR_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define UART_LIDAR_RX_STS_ADDR_MATCH       (uint8)(0x01u << UART_LIDAR_RX_STS_ADDR_MATCH_SHIFT)
    #define UART_LIDAR_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << UART_LIDAR_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define UART_LIDAR_RX_HW_MASK                     (0x7Fu)
#endif /* End (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) */

/* Control Register definitions */
#define UART_LIDAR_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define UART_LIDAR_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define UART_LIDAR_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define UART_LIDAR_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define UART_LIDAR_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define UART_LIDAR_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define UART_LIDAR_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define UART_LIDAR_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define UART_LIDAR_CTRL_HD_SEND               (uint8)(0x01u << UART_LIDAR_CTRL_HD_SEND_SHIFT)
#define UART_LIDAR_CTRL_HD_SEND_BREAK         (uint8)(0x01u << UART_LIDAR_CTRL_HD_SEND_BREAK_SHIFT)
#define UART_LIDAR_CTRL_MARK                  (uint8)(0x01u << UART_LIDAR_CTRL_MARK_SHIFT)
#define UART_LIDAR_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << UART_LIDAR_CTRL_PARITY_TYPE0_SHIFT)
#define UART_LIDAR_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << UART_LIDAR_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define UART_LIDAR_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define UART_LIDAR_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define UART_LIDAR_SEND_BREAK                         (0x00u)
#define UART_LIDAR_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define UART_LIDAR_REINIT                             (0x02u)
#define UART_LIDAR_SEND_WAIT_REINIT                   (0x03u)

#define UART_LIDAR_OVER_SAMPLE_8                      (8u)
#define UART_LIDAR_OVER_SAMPLE_16                     (16u)

#define UART_LIDAR_BIT_CENTER                         (UART_LIDAR_OVER_SAMPLE_COUNT - 2u)

#define UART_LIDAR_FIFO_LENGTH                        (4u)
#define UART_LIDAR_NUMBER_OF_START_BIT                (1u)
#define UART_LIDAR_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define UART_LIDAR_TXBITCTR_BREAKBITS8X   ((UART_LIDAR_BREAK_BITS_TX * UART_LIDAR_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define UART_LIDAR_TXBITCTR_BREAKBITS ((UART_LIDAR_BREAK_BITS_TX * UART_LIDAR_OVER_SAMPLE_COUNT) - 1u)

#define UART_LIDAR_HALF_BIT_COUNT   \
                            (((UART_LIDAR_OVER_SAMPLE_COUNT / 2u) + (UART_LIDAR_USE23POLLING * 1u)) - 2u)
#if (UART_LIDAR_OVER_SAMPLE_COUNT == UART_LIDAR_OVER_SAMPLE_8)
    #define UART_LIDAR_HD_TXBITCTR_INIT   (((UART_LIDAR_BREAK_BITS_TX + \
                            UART_LIDAR_NUMBER_OF_START_BIT) * UART_LIDAR_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define UART_LIDAR_RXBITCTR_INIT  ((((UART_LIDAR_BREAK_BITS_RX + UART_LIDAR_NUMBER_OF_START_BIT) \
                            * UART_LIDAR_OVER_SAMPLE_COUNT) + UART_LIDAR_HALF_BIT_COUNT) - 1u)

#else /* UART_LIDAR_OVER_SAMPLE_COUNT == UART_LIDAR_OVER_SAMPLE_16 */
    #define UART_LIDAR_HD_TXBITCTR_INIT   ((8u * UART_LIDAR_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define UART_LIDAR_RXBITCTR_INIT      (((7u * UART_LIDAR_OVER_SAMPLE_COUNT) - 1u) + \
                                                      UART_LIDAR_HALF_BIT_COUNT)
#endif /* End UART_LIDAR_OVER_SAMPLE_COUNT */

#define UART_LIDAR_HD_RXBITCTR_INIT                   UART_LIDAR_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 UART_LIDAR_initVar;
#if (UART_LIDAR_TX_INTERRUPT_ENABLED && UART_LIDAR_TX_ENABLED)
    extern volatile uint8 UART_LIDAR_txBuffer[UART_LIDAR_TX_BUFFER_SIZE];
    extern volatile uint8 UART_LIDAR_txBufferRead;
    extern uint8 UART_LIDAR_txBufferWrite;
#endif /* (UART_LIDAR_TX_INTERRUPT_ENABLED && UART_LIDAR_TX_ENABLED) */
#if (UART_LIDAR_RX_INTERRUPT_ENABLED && (UART_LIDAR_RX_ENABLED || UART_LIDAR_HD_ENABLED))
    extern uint8 UART_LIDAR_errorStatus;
    extern volatile uint8 UART_LIDAR_rxBuffer[UART_LIDAR_RX_BUFFER_SIZE];
    extern volatile uint8 UART_LIDAR_rxBufferRead;
    extern volatile uint8 UART_LIDAR_rxBufferWrite;
    extern volatile uint8 UART_LIDAR_rxBufferLoopDetect;
    extern volatile uint8 UART_LIDAR_rxBufferOverflow;
    #if (UART_LIDAR_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 UART_LIDAR_rxAddressMode;
        extern volatile uint8 UART_LIDAR_rxAddressDetected;
    #endif /* (UART_LIDAR_RXHW_ADDRESS_ENABLED) */
#endif /* (UART_LIDAR_RX_INTERRUPT_ENABLED && (UART_LIDAR_RX_ENABLED || UART_LIDAR_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define UART_LIDAR__B_UART__AM_SW_BYTE_BYTE 1
#define UART_LIDAR__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define UART_LIDAR__B_UART__AM_HW_BYTE_BY_BYTE 3
#define UART_LIDAR__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define UART_LIDAR__B_UART__AM_NONE 0

#define UART_LIDAR__B_UART__NONE_REVB 0
#define UART_LIDAR__B_UART__EVEN_REVB 1
#define UART_LIDAR__B_UART__ODD_REVB 2
#define UART_LIDAR__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define UART_LIDAR_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define UART_LIDAR_NUMBER_OF_STOP_BITS    (1u)

#if (UART_LIDAR_RXHW_ADDRESS_ENABLED)
    #define UART_LIDAR_RX_ADDRESS_MODE    (0u)
    #define UART_LIDAR_RX_HW_ADDRESS1     (0u)
    #define UART_LIDAR_RX_HW_ADDRESS2     (0u)
#endif /* (UART_LIDAR_RXHW_ADDRESS_ENABLED) */

#define UART_LIDAR_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << UART_LIDAR_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << UART_LIDAR_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << UART_LIDAR_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << UART_LIDAR_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << UART_LIDAR_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << UART_LIDAR_RX_STS_BREAK_SHIFT) \
                                        | (0 << UART_LIDAR_RX_STS_OVERRUN_SHIFT))

#define UART_LIDAR_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << UART_LIDAR_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << UART_LIDAR_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << UART_LIDAR_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << UART_LIDAR_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define UART_LIDAR_CONTROL_REG \
                            (* (reg8 *) UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define UART_LIDAR_CONTROL_PTR \
                            (  (reg8 *) UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(UART_LIDAR_TX_ENABLED)
    #define UART_LIDAR_TXDATA_REG          (* (reg8 *) UART_LIDAR_BUART_sTX_TxShifter_u0__F0_REG)
    #define UART_LIDAR_TXDATA_PTR          (  (reg8 *) UART_LIDAR_BUART_sTX_TxShifter_u0__F0_REG)
    #define UART_LIDAR_TXDATA_AUX_CTL_REG  (* (reg8 *) UART_LIDAR_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define UART_LIDAR_TXDATA_AUX_CTL_PTR  (  (reg8 *) UART_LIDAR_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define UART_LIDAR_TXSTATUS_REG        (* (reg8 *) UART_LIDAR_BUART_sTX_TxSts__STATUS_REG)
    #define UART_LIDAR_TXSTATUS_PTR        (  (reg8 *) UART_LIDAR_BUART_sTX_TxSts__STATUS_REG)
    #define UART_LIDAR_TXSTATUS_MASK_REG   (* (reg8 *) UART_LIDAR_BUART_sTX_TxSts__MASK_REG)
    #define UART_LIDAR_TXSTATUS_MASK_PTR   (  (reg8 *) UART_LIDAR_BUART_sTX_TxSts__MASK_REG)
    #define UART_LIDAR_TXSTATUS_ACTL_REG   (* (reg8 *) UART_LIDAR_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define UART_LIDAR_TXSTATUS_ACTL_PTR   (  (reg8 *) UART_LIDAR_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(UART_LIDAR_TXCLKGEN_DP)
        #define UART_LIDAR_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define UART_LIDAR_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define UART_LIDAR_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define UART_LIDAR_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define UART_LIDAR_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define UART_LIDAR_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define UART_LIDAR_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define UART_LIDAR_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define UART_LIDAR_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define UART_LIDAR_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) UART_LIDAR_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* UART_LIDAR_TXCLKGEN_DP */

#endif /* End UART_LIDAR_TX_ENABLED */

#if(UART_LIDAR_HD_ENABLED)

    #define UART_LIDAR_TXDATA_REG             (* (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__F1_REG )
    #define UART_LIDAR_TXDATA_PTR             (  (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__F1_REG )
    #define UART_LIDAR_TXDATA_AUX_CTL_REG     (* (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define UART_LIDAR_TXDATA_AUX_CTL_PTR     (  (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define UART_LIDAR_TXSTATUS_REG           (* (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_REG )
    #define UART_LIDAR_TXSTATUS_PTR           (  (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_REG )
    #define UART_LIDAR_TXSTATUS_MASK_REG      (* (reg8 *) UART_LIDAR_BUART_sRX_RxSts__MASK_REG )
    #define UART_LIDAR_TXSTATUS_MASK_PTR      (  (reg8 *) UART_LIDAR_BUART_sRX_RxSts__MASK_REG )
    #define UART_LIDAR_TXSTATUS_ACTL_REG      (* (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define UART_LIDAR_TXSTATUS_ACTL_PTR      (  (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End UART_LIDAR_HD_ENABLED */

#if( (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) )
    #define UART_LIDAR_RXDATA_REG             (* (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__F0_REG )
    #define UART_LIDAR_RXDATA_PTR             (  (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__F0_REG )
    #define UART_LIDAR_RXADDRESS1_REG         (* (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__D0_REG )
    #define UART_LIDAR_RXADDRESS1_PTR         (  (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__D0_REG )
    #define UART_LIDAR_RXADDRESS2_REG         (* (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__D1_REG )
    #define UART_LIDAR_RXADDRESS2_PTR         (  (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__D1_REG )
    #define UART_LIDAR_RXDATA_AUX_CTL_REG     (* (reg8 *) UART_LIDAR_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define UART_LIDAR_RXBITCTR_PERIOD_REG    (* (reg8 *) UART_LIDAR_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define UART_LIDAR_RXBITCTR_PERIOD_PTR    (  (reg8 *) UART_LIDAR_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define UART_LIDAR_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) UART_LIDAR_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define UART_LIDAR_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) UART_LIDAR_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define UART_LIDAR_RXBITCTR_COUNTER_REG   (* (reg8 *) UART_LIDAR_BUART_sRX_RxBitCounter__COUNT_REG )
    #define UART_LIDAR_RXBITCTR_COUNTER_PTR   (  (reg8 *) UART_LIDAR_BUART_sRX_RxBitCounter__COUNT_REG )

    #define UART_LIDAR_RXSTATUS_REG           (* (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_REG )
    #define UART_LIDAR_RXSTATUS_PTR           (  (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_REG )
    #define UART_LIDAR_RXSTATUS_MASK_REG      (* (reg8 *) UART_LIDAR_BUART_sRX_RxSts__MASK_REG )
    #define UART_LIDAR_RXSTATUS_MASK_PTR      (  (reg8 *) UART_LIDAR_BUART_sRX_RxSts__MASK_REG )
    #define UART_LIDAR_RXSTATUS_ACTL_REG      (* (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define UART_LIDAR_RXSTATUS_ACTL_PTR      (  (reg8 *) UART_LIDAR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) */

#if(UART_LIDAR_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define UART_LIDAR_INTCLOCK_CLKEN_REG     (* (reg8 *) UART_LIDAR_IntClock__PM_ACT_CFG)
    #define UART_LIDAR_INTCLOCK_CLKEN_PTR     (  (reg8 *) UART_LIDAR_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define UART_LIDAR_INTCLOCK_CLKEN_MASK    UART_LIDAR_IntClock__PM_ACT_MSK
#endif /* End UART_LIDAR_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(UART_LIDAR_TX_ENABLED)
    #define UART_LIDAR_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End UART_LIDAR_TX_ENABLED */

#if(UART_LIDAR_HD_ENABLED)
    #define UART_LIDAR_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End UART_LIDAR_HD_ENABLED */

#if( (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) )
    #define UART_LIDAR_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define UART_LIDAR_WAIT_1_MS      UART_LIDAR_BL_CHK_DELAY_MS   

#define UART_LIDAR_TXBUFFERSIZE   UART_LIDAR_TX_BUFFER_SIZE
#define UART_LIDAR_RXBUFFERSIZE   UART_LIDAR_RX_BUFFER_SIZE

#if (UART_LIDAR_RXHW_ADDRESS_ENABLED)
    #define UART_LIDAR_RXADDRESSMODE  UART_LIDAR_RX_ADDRESS_MODE
    #define UART_LIDAR_RXHWADDRESS1   UART_LIDAR_RX_HW_ADDRESS1
    #define UART_LIDAR_RXHWADDRESS2   UART_LIDAR_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define UART_LIDAR_RXAddressMode  UART_LIDAR_RXADDRESSMODE
#endif /* (UART_LIDAR_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define UART_LIDAR_initvar                    UART_LIDAR_initVar

#define UART_LIDAR_RX_Enabled                 UART_LIDAR_RX_ENABLED
#define UART_LIDAR_TX_Enabled                 UART_LIDAR_TX_ENABLED
#define UART_LIDAR_HD_Enabled                 UART_LIDAR_HD_ENABLED
#define UART_LIDAR_RX_IntInterruptEnabled     UART_LIDAR_RX_INTERRUPT_ENABLED
#define UART_LIDAR_TX_IntInterruptEnabled     UART_LIDAR_TX_INTERRUPT_ENABLED
#define UART_LIDAR_InternalClockUsed          UART_LIDAR_INTERNAL_CLOCK_USED
#define UART_LIDAR_RXHW_Address_Enabled       UART_LIDAR_RXHW_ADDRESS_ENABLED
#define UART_LIDAR_OverSampleCount            UART_LIDAR_OVER_SAMPLE_COUNT
#define UART_LIDAR_ParityType                 UART_LIDAR_PARITY_TYPE

#if( UART_LIDAR_TX_ENABLED && (UART_LIDAR_TXBUFFERSIZE > UART_LIDAR_FIFO_LENGTH))
    #define UART_LIDAR_TXBUFFER               UART_LIDAR_txBuffer
    #define UART_LIDAR_TXBUFFERREAD           UART_LIDAR_txBufferRead
    #define UART_LIDAR_TXBUFFERWRITE          UART_LIDAR_txBufferWrite
#endif /* End UART_LIDAR_TX_ENABLED */
#if( ( UART_LIDAR_RX_ENABLED || UART_LIDAR_HD_ENABLED ) && \
     (UART_LIDAR_RXBUFFERSIZE > UART_LIDAR_FIFO_LENGTH) )
    #define UART_LIDAR_RXBUFFER               UART_LIDAR_rxBuffer
    #define UART_LIDAR_RXBUFFERREAD           UART_LIDAR_rxBufferRead
    #define UART_LIDAR_RXBUFFERWRITE          UART_LIDAR_rxBufferWrite
    #define UART_LIDAR_RXBUFFERLOOPDETECT     UART_LIDAR_rxBufferLoopDetect
    #define UART_LIDAR_RXBUFFER_OVERFLOW      UART_LIDAR_rxBufferOverflow
#endif /* End UART_LIDAR_RX_ENABLED */

#ifdef UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define UART_LIDAR_CONTROL                UART_LIDAR_CONTROL_REG
#endif /* End UART_LIDAR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(UART_LIDAR_TX_ENABLED)
    #define UART_LIDAR_TXDATA                 UART_LIDAR_TXDATA_REG
    #define UART_LIDAR_TXSTATUS               UART_LIDAR_TXSTATUS_REG
    #define UART_LIDAR_TXSTATUS_MASK          UART_LIDAR_TXSTATUS_MASK_REG
    #define UART_LIDAR_TXSTATUS_ACTL          UART_LIDAR_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(UART_LIDAR_TXCLKGEN_DP)
        #define UART_LIDAR_TXBITCLKGEN_CTR        UART_LIDAR_TXBITCLKGEN_CTR_REG
        #define UART_LIDAR_TXBITCLKTX_COMPLETE    UART_LIDAR_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define UART_LIDAR_TXBITCTR_PERIOD        UART_LIDAR_TXBITCTR_PERIOD_REG
        #define UART_LIDAR_TXBITCTR_CONTROL       UART_LIDAR_TXBITCTR_CONTROL_REG
        #define UART_LIDAR_TXBITCTR_COUNTER       UART_LIDAR_TXBITCTR_COUNTER_REG
    #endif /* UART_LIDAR_TXCLKGEN_DP */
#endif /* End UART_LIDAR_TX_ENABLED */

#if(UART_LIDAR_HD_ENABLED)
    #define UART_LIDAR_TXDATA                 UART_LIDAR_TXDATA_REG
    #define UART_LIDAR_TXSTATUS               UART_LIDAR_TXSTATUS_REG
    #define UART_LIDAR_TXSTATUS_MASK          UART_LIDAR_TXSTATUS_MASK_REG
    #define UART_LIDAR_TXSTATUS_ACTL          UART_LIDAR_TXSTATUS_ACTL_REG
#endif /* End UART_LIDAR_HD_ENABLED */

#if( (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) )
    #define UART_LIDAR_RXDATA                 UART_LIDAR_RXDATA_REG
    #define UART_LIDAR_RXADDRESS1             UART_LIDAR_RXADDRESS1_REG
    #define UART_LIDAR_RXADDRESS2             UART_LIDAR_RXADDRESS2_REG
    #define UART_LIDAR_RXBITCTR_PERIOD        UART_LIDAR_RXBITCTR_PERIOD_REG
    #define UART_LIDAR_RXBITCTR_CONTROL       UART_LIDAR_RXBITCTR_CONTROL_REG
    #define UART_LIDAR_RXBITCTR_COUNTER       UART_LIDAR_RXBITCTR_COUNTER_REG
    #define UART_LIDAR_RXSTATUS               UART_LIDAR_RXSTATUS_REG
    #define UART_LIDAR_RXSTATUS_MASK          UART_LIDAR_RXSTATUS_MASK_REG
    #define UART_LIDAR_RXSTATUS_ACTL          UART_LIDAR_RXSTATUS_ACTL_REG
#endif /* End  (UART_LIDAR_RX_ENABLED) || (UART_LIDAR_HD_ENABLED) */

#if(UART_LIDAR_INTERNAL_CLOCK_USED)
    #define UART_LIDAR_INTCLOCK_CLKEN         UART_LIDAR_INTCLOCK_CLKEN_REG
#endif /* End UART_LIDAR_INTERNAL_CLOCK_USED */

#define UART_LIDAR_WAIT_FOR_COMLETE_REINIT    UART_LIDAR_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_UART_LIDAR_H */


/* [] END OF FILE */

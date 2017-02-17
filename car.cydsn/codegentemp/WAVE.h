/*******************************************************************************
* File Name: WAVE.h
* Version 3.50
*
* Description:
*  This file provides constants and parameter values for the I2C component.

*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_I2C_WAVE_H)
#define CY_I2C_WAVE_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"

/* Check if required defines such as CY_PSOC5LP are available in cy_boot */
#if !defined (CY_PSOC5LP)
    #error Component I2C_v3_50 requires cy_boot v3.10 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define WAVE_IMPLEMENTATION     (1u)
#define WAVE_MODE               (2u)
#define WAVE_ENABLE_WAKEUP      (0u)
#define WAVE_ADDR_DECODE        (1u)
#define WAVE_UDB_INTRN_CLOCK    (1u)


/* I2C implementation enum */
#define WAVE_UDB    (0x00u)
#define WAVE_FF     (0x01u)

#define WAVE_FF_IMPLEMENTED     (WAVE_FF  == WAVE_IMPLEMENTATION)
#define WAVE_UDB_IMPLEMENTED    (WAVE_UDB == WAVE_IMPLEMENTATION)

#define WAVE_UDB_INTRN_CLOCK_ENABLED    (WAVE_UDB_IMPLEMENTED && \
                                                     (0u != WAVE_UDB_INTRN_CLOCK))
/* I2C modes enum */
#define WAVE_MODE_SLAVE                 (0x01u)
#define WAVE_MODE_MASTER                (0x02u)
#define WAVE_MODE_MULTI_MASTER          (0x06u)
#define WAVE_MODE_MULTI_MASTER_SLAVE    (0x07u)
#define WAVE_MODE_MULTI_MASTER_MASK     (0x04u)

#define WAVE_MODE_SLAVE_ENABLED         (0u != (WAVE_MODE_SLAVE  & WAVE_MODE))
#define WAVE_MODE_MASTER_ENABLED        (0u != (WAVE_MODE_MASTER & WAVE_MODE))
#define WAVE_MODE_MULTI_MASTER_ENABLED  (0u != (WAVE_MODE_MULTI_MASTER_MASK & \
                                                            WAVE_MODE))
#define WAVE_MODE_MULTI_MASTER_SLAVE_ENABLED    (WAVE_MODE_MULTI_MASTER_SLAVE == \
                                                             WAVE_MODE)

/* Address detection enum */
#define WAVE_SW_DECODE      (0x00u)
#define WAVE_HW_DECODE      (0x01u)

#define WAVE_SW_ADRR_DECODE             (WAVE_SW_DECODE == WAVE_ADDR_DECODE)
#define WAVE_HW_ADRR_DECODE             (WAVE_HW_DECODE == WAVE_ADDR_DECODE)

/* Wakeup enabled */
#define WAVE_WAKEUP_ENABLED             (0u != WAVE_ENABLE_WAKEUP)

/* Adds bootloader APIs to component */
#define WAVE_BOOTLOADER_INTERFACE_ENABLED   (WAVE_MODE_SLAVE_ENABLED && \
                                                            ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_WAVE) || \
                                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface)))

/* Timeout functionality */
#define WAVE_TIMEOUT_ENABLE             (0u)
#define WAVE_TIMEOUT_SCL_TMOUT_ENABLE   (0u)
#define WAVE_TIMEOUT_SDA_TMOUT_ENABLE   (0u)
#define WAVE_TIMEOUT_PRESCALER_ENABLE   (0u)
#define WAVE_TIMEOUT_IMPLEMENTATION     (0u)

/* Convert to boolean */
#define WAVE_TIMEOUT_ENABLED            (0u != WAVE_TIMEOUT_ENABLE)
#define WAVE_TIMEOUT_SCL_TMOUT_ENABLED  (0u != WAVE_TIMEOUT_SCL_TMOUT_ENABLE)
#define WAVE_TIMEOUT_SDA_TMOUT_ENABLED  (0u != WAVE_TIMEOUT_SDA_TMOUT_ENABLE)
#define WAVE_TIMEOUT_PRESCALER_ENABLED  (0u != WAVE_TIMEOUT_PRESCALER_ENABLE)

/* Timeout implementation enum. */
#define WAVE_TIMEOUT_UDB    (0x00u)
#define WAVE_TIMEOUT_FF     (0x01u)

#define WAVE_TIMEOUT_FF_IMPLEMENTED     (WAVE_TIMEOUT_FF  == \
                                                        WAVE_TIMEOUT_IMPLEMENTATION)
#define WAVE_TIMEOUT_UDB_IMPLEMENTED    (WAVE_TIMEOUT_UDB == \
                                                        WAVE_TIMEOUT_IMPLEMENTATION)

#define WAVE_TIMEOUT_FF_ENABLED         (WAVE_TIMEOUT_ENABLED && \
                                                     WAVE_TIMEOUT_FF_IMPLEMENTED)

#define WAVE_TIMEOUT_UDB_ENABLED        (WAVE_TIMEOUT_ENABLED && \
                                                     WAVE_TIMEOUT_UDB_IMPLEMENTED)

#define WAVE_EXTERN_I2C_INTR_HANDLER    (0u)
#define WAVE_EXTERN_TMOUT_INTR_HANDLER  (0u)

#define WAVE_INTERN_I2C_INTR_HANDLER    (0u == WAVE_EXTERN_I2C_INTR_HANDLER)
#define WAVE_INTERN_TMOUT_INTR_HANDLER  (0u == WAVE_EXTERN_TMOUT_INTR_HANDLER)


/***************************************
*       Type defines
***************************************/

/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;

#if (WAVE_FF_IMPLEMENTED)
    uint8 xcfg;
    uint8 cfg;
    uint8 addr;
    uint8 clkDiv1;
    uint8 clkDiv2;
#else
    uint8 control;
#endif /* (WAVE_FF_IMPLEMENTED) */

#if (WAVE_TIMEOUT_ENABLED)
    uint16 tmoutCfg;
    uint8  tmoutIntr;
#endif /* (WAVE_TIMEOUT_ENABLED) */

} WAVE_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void WAVE_Init(void)                            ;
void WAVE_Enable(void)                          ;

void WAVE_Start(void)                           ;
void WAVE_Stop(void)                            ;

#define WAVE_EnableInt()        CyIntEnable      (WAVE_ISR_NUMBER)
#define WAVE_DisableInt()       CyIntDisable     (WAVE_ISR_NUMBER)
#define WAVE_ClearPendingInt()  CyIntClearPending(WAVE_ISR_NUMBER)
#define WAVE_SetPendingInt()    CyIntSetPending  (WAVE_ISR_NUMBER)

void WAVE_SaveConfig(void)                      ;
void WAVE_Sleep(void)                           ;
void WAVE_RestoreConfig(void)                   ;
void WAVE_Wakeup(void)                          ;

/* I2C Master functions prototypes */
#if (WAVE_MODE_MASTER_ENABLED)
    /* Read and Clear status functions */
    uint8 WAVE_MasterStatus(void)                ;
    uint8 WAVE_MasterClearStatus(void)           ;

    /* Interrupt based operation functions */
    uint8 WAVE_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode) \
                                                            ;
    uint8 WAVE_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode) \
                                                            ;
    uint8 WAVE_MasterGetReadBufSize(void)       ;
    uint8 WAVE_MasterGetWriteBufSize(void)      ;
    void  WAVE_MasterClearReadBuf(void)         ;
    void  WAVE_MasterClearWriteBuf(void)        ;

    /* Manual operation functions */
    uint8 WAVE_MasterSendStart(uint8 slaveAddress, uint8 R_nW) \
                                                            ;
    uint8 WAVE_MasterSendRestart(uint8 slaveAddress, uint8 R_nW) \
                                                            ;
    uint8 WAVE_MasterSendStop(void)             ;
    uint8 WAVE_MasterWriteByte(uint8 theByte)   ;
    uint8 WAVE_MasterReadByte(uint8 acknNak)    ;

#endif /* (WAVE_MODE_MASTER_ENABLED) */

/* I2C Slave functions prototypes */
#if (WAVE_MODE_SLAVE_ENABLED)
    /* Read and Clear status functions */
    uint8 WAVE_SlaveStatus(void)                ;
    uint8 WAVE_SlaveClearReadStatus(void)       ;
    uint8 WAVE_SlaveClearWriteStatus(void)      ;

    void  WAVE_SlaveSetAddress(uint8 address)   ;

    /* Interrupt based operation functions */
    void  WAVE_SlaveInitReadBuf(uint8 * rdBuf, uint8 bufSize) \
                                                            ;
    void  WAVE_SlaveInitWriteBuf(uint8 * wrBuf, uint8 bufSize) \
                                                            ;
    uint8 WAVE_SlaveGetReadBufSize(void)        ;
    uint8 WAVE_SlaveGetWriteBufSize(void)       ;
    void  WAVE_SlaveClearReadBuf(void)          ;
    void  WAVE_SlaveClearWriteBuf(void)         ;

    /* Communication bootloader I2C Slave APIs */
    #if defined(CYDEV_BOOTLOADER_IO_COMP) && (WAVE_BOOTLOADER_INTERFACE_ENABLED)
        /* Physical layer functions */
        void     WAVE_CyBtldrCommStart(void) CYSMALL \
                                                            ;
        void     WAVE_CyBtldrCommStop(void)  CYSMALL \
                                                            ;
        void     WAVE_CyBtldrCommReset(void) CYSMALL \
                                                            ;
        cystatus WAVE_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) \
                                                        CYSMALL ;
        cystatus WAVE_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)  CYSMALL \
                                                            ;

        #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_WAVE)
            #define CyBtldrCommStart    WAVE_CyBtldrCommStart
            #define CyBtldrCommStop     WAVE_CyBtldrCommStop
            #define CyBtldrCommReset    WAVE_CyBtldrCommReset
            #define CyBtldrCommWrite    WAVE_CyBtldrCommWrite
            #define CyBtldrCommRead     WAVE_CyBtldrCommRead
        #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_WAVE) */

        /* Size of Read/Write buffers for I2C bootloader  */
        #define WAVE_BTLDR_SIZEOF_READ_BUFFER   (0x80u)
        #define WAVE_BTLDR_SIZEOF_WRITE_BUFFER  (0x80u)
        #define WAVE_MIN_UNT16(a, b)            ( ((uint16)(a) < (b)) ? ((uint16) (a)) : ((uint16) (b)) )
        #define WAVE_WAIT_1_MS                  (1u)

    #endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (WAVE_BOOTLOADER_INTERFACE_ENABLED) */

#endif /* (WAVE_MODE_SLAVE_ENABLED) */

/* Component interrupt handlers */
CY_ISR_PROTO(WAVE_ISR);
#if ((WAVE_FF_IMPLEMENTED) || (WAVE_WAKEUP_ENABLED))
    CY_ISR_PROTO(WAVE_WAKEUP_ISR);
#endif /* ((WAVE_FF_IMPLEMENTED) || (WAVE_WAKEUP_ENABLED)) */


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 WAVE_initVar;


/***************************************
*   Initial Parameter Constants
***************************************/

#define WAVE_DATA_RATE          (50u)
#define WAVE_DEFAULT_ADDR       (8u)
#define WAVE_I2C_PAIR_SELECTED  (0u)

/* I2C pair enum */
#define WAVE_I2C_PAIR_ANY   (0x01u) /* Any pins for I2C */
#define WAVE_I2C_PAIR0      (0x01u) /* I2C0: (SCL = P12[4]) && (SCL = P12[5]) */
#define WAVE_I2C_PAIR1      (0x02u) /* I2C1: (SCL = P12[0]) && (SDA = P12[1]) */

#define WAVE_I2C1_SIO_PAIR  (WAVE_I2C_PAIR1 == WAVE_I2C_PAIR_SELECTED)
#define WAVE_I2C0_SIO_PAIR  (WAVE_I2C_PAIR0 == WAVE_I2C_PAIR_SELECTED)


/***************************************
*            API Constants
***************************************/

/* Master/Slave control constants */
#define WAVE_READ_XFER_MODE     (0x01u) /* Read */
#define WAVE_WRITE_XFER_MODE    (0x00u) /* Write */
#define WAVE_ACK_DATA           (0x01u) /* Send ACK */
#define WAVE_NAK_DATA           (0x00u) /* Send NAK */
#define WAVE_OVERFLOW_RETURN    (0xFFu) /* Send on bus in case of overflow */

#if (WAVE_MODE_MASTER_ENABLED)
    /* "Mode" constants for MasterWriteBuf() or MasterReadBuf() function */
    #define WAVE_MODE_COMPLETE_XFER     (0x00u) /* Full transfer with Start and Stop */
    #define WAVE_MODE_REPEAT_START      (0x01u) /* Begin with a ReStart instead of a Start */
    #define WAVE_MODE_NO_STOP           (0x02u) /* Complete the transfer without a Stop */

    /* Master status */
    #define WAVE_MSTAT_CLEAR            (0x00u) /* Clear (initialize) status value */

    #define WAVE_MSTAT_RD_CMPLT         (0x01u) /* Read complete */
    #define WAVE_MSTAT_WR_CMPLT         (0x02u) /* Write complete */
    #define WAVE_MSTAT_XFER_INP         (0x04u) /* Master transfer in progress */
    #define WAVE_MSTAT_XFER_HALT        (0x08u) /* Transfer is halted */

    #define WAVE_MSTAT_ERR_MASK         (0xF0u) /* Mask for all errors */
    #define WAVE_MSTAT_ERR_SHORT_XFER   (0x10u) /* Master NAKed before end of packet */
    #define WAVE_MSTAT_ERR_ADDR_NAK     (0x20u) /* Slave did not ACK */
    #define WAVE_MSTAT_ERR_ARB_LOST     (0x40u) /* Master lost arbitration during communication */
    #define WAVE_MSTAT_ERR_XFER         (0x80u) /* Error during transfer */

    /* Master API returns */
    #define WAVE_MSTR_NO_ERROR          (0x00u) /* Function complete without error */
    #define WAVE_MSTR_BUS_BUSY          (0x01u) /* Bus is busy, process not started */
    #define WAVE_MSTR_NOT_READY         (0x02u) /* Master not Master on the bus or */
                                                            /*  Slave operation in progress */
    #define WAVE_MSTR_ERR_LB_NAK        (0x03u) /* Last Byte Naked */
    #define WAVE_MSTR_ERR_ARB_LOST      (0x04u) /* Master lost arbitration during communication */
    #define WAVE_MSTR_ERR_ABORT_START_GEN  (0x05u) /* Master did not generate Start, the Slave */
                                                               /* was addressed before */

#endif /* (WAVE_MODE_MASTER_ENABLED) */

#if (WAVE_MODE_SLAVE_ENABLED)
    /* Slave Status Constants */
    #define WAVE_SSTAT_RD_CMPLT     (0x01u) /* Read transfer complete */
    #define WAVE_SSTAT_RD_BUSY      (0x02u) /* Read transfer in progress */
    #define WAVE_SSTAT_RD_ERR_OVFL  (0x04u) /* Read overflow Error */
    #define WAVE_SSTAT_RD_MASK      (0x0Fu) /* Read Status Mask */
    #define WAVE_SSTAT_RD_NO_ERR    (0x00u) /* Read no Error */

    #define WAVE_SSTAT_WR_CMPLT     (0x10u) /* Write transfer complete */
    #define WAVE_SSTAT_WR_BUSY      (0x20u) /* Write transfer in progress */
    #define WAVE_SSTAT_WR_ERR_OVFL  (0x40u) /* Write overflow Error */
    #define WAVE_SSTAT_WR_MASK      (0xF0u) /* Write Status Mask  */
    #define WAVE_SSTAT_WR_NO_ERR    (0x00u) /* Write no Error */

    #define WAVE_SSTAT_RD_CLEAR     (0x0Du) /* Read Status clear */
    #define WAVE_SSTAT_WR_CLEAR     (0xD0u) /* Write Status Clear */

#endif /* (WAVE_MODE_SLAVE_ENABLED) */


/***************************************
*       I2C state machine constants
***************************************/

/* Default slave address states */
#define  WAVE_SM_IDLE           (0x10u) /* Default state - IDLE */
#define  WAVE_SM_EXIT_IDLE      (0x00u) /* Pass master and slave processing and go to IDLE */

/* Slave mode states */
#define  WAVE_SM_SLAVE          (WAVE_SM_IDLE) /* Any Slave state */
#define  WAVE_SM_SL_WR_DATA     (0x11u) /* Master writes data to slave  */
#define  WAVE_SM_SL_RD_DATA     (0x12u) /* Master reads data from slave */

/* Master mode states */
#define  WAVE_SM_MASTER         (0x40u) /* Any master state */

#define  WAVE_SM_MSTR_RD        (0x08u) /* Any master read state          */
#define  WAVE_SM_MSTR_RD_ADDR   (0x49u) /* Master sends address with read */
#define  WAVE_SM_MSTR_RD_DATA   (0x4Au) /* Master reads data              */

#define  WAVE_SM_MSTR_WR        (0x04u) /* Any master read state           */
#define  WAVE_SM_MSTR_WR_ADDR   (0x45u) /* Master sends address with write */
#define  WAVE_SM_MSTR_WR_DATA   (0x46u) /* Master writes data              */

#define  WAVE_SM_MSTR_HALT      (0x60u) /* Master waits for ReStart */

#define WAVE_CHECK_SM_MASTER    (0u != (WAVE_SM_MASTER & WAVE_state))
#define WAVE_CHECK_SM_SLAVE     (0u != (WAVE_SM_SLAVE  & WAVE_state))


/***************************************
*              Registers
***************************************/

#if (WAVE_FF_IMPLEMENTED)
    /* Fixed Function registers */
    #define WAVE_XCFG_REG           (*(reg8 *) WAVE_I2C_FF__XCFG)
    #define WAVE_XCFG_PTR           ( (reg8 *) WAVE_I2C_FF__XCFG)

    #define WAVE_ADDR_REG           (*(reg8 *) WAVE_I2C_FF__ADR)
    #define WAVE_ADDR_PTR           ( (reg8 *) WAVE_I2C_FF__ADR)

    #define WAVE_CFG_REG            (*(reg8 *) WAVE_I2C_FF__CFG)
    #define WAVE_CFG_PTR            ( (reg8 *) WAVE_I2C_FF__CFG)

    #define WAVE_CSR_REG            (*(reg8 *) WAVE_I2C_FF__CSR)
    #define WAVE_CSR_PTR            ( (reg8 *) WAVE_I2C_FF__CSR)

    #define WAVE_DATA_REG           (*(reg8 *) WAVE_I2C_FF__D)
    #define WAVE_DATA_PTR           ( (reg8 *) WAVE_I2C_FF__D)

    #define WAVE_MCSR_REG           (*(reg8 *) WAVE_I2C_FF__MCSR)
    #define WAVE_MCSR_PTR           ( (reg8 *) WAVE_I2C_FF__MCSR)

    #define WAVE_ACT_PWRMGR_REG     (*(reg8 *) WAVE_I2C_FF__PM_ACT_CFG)
    #define WAVE_ACT_PWRMGR_PTR     ( (reg8 *) WAVE_I2C_FF__PM_ACT_CFG)
    #define WAVE_ACT_PWR_EN         ( (uint8)  WAVE_I2C_FF__PM_ACT_MSK)

    #define WAVE_STBY_PWRMGR_REG    (*(reg8 *) WAVE_I2C_FF__PM_STBY_CFG)
    #define WAVE_STBY_PWRMGR_PTR    ( (reg8 *) WAVE_I2C_FF__PM_STBY_CFG)
    #define WAVE_STBY_PWR_EN        ( (uint8)  WAVE_I2C_FF__PM_STBY_MSK)

    #define WAVE_PWRSYS_CR1_REG     (*(reg8 *) CYREG_PWRSYS_CR1)
    #define WAVE_PWRSYS_CR1_PTR     ( (reg8 *) CYREG_PWRSYS_CR1)

    #define WAVE_CLKDIV1_REG    (*(reg8 *) WAVE_I2C_FF__CLK_DIV1)
    #define WAVE_CLKDIV1_PTR    ( (reg8 *) WAVE_I2C_FF__CLK_DIV1)

    #define WAVE_CLKDIV2_REG    (*(reg8 *) WAVE_I2C_FF__CLK_DIV2)
    #define WAVE_CLKDIV2_PTR    ( (reg8 *) WAVE_I2C_FF__CLK_DIV2)

#else
    /* UDB implementation registers */
    #define WAVE_CFG_REG \
            (*(reg8 *) WAVE_bI2C_UDB_SyncCtl_CtrlReg__CONTROL_REG)
    #define WAVE_CFG_PTR \
            ( (reg8 *) WAVE_bI2C_UDB_SyncCtl_CtrlReg__CONTROL_REG)

    #define WAVE_CSR_REG        (*(reg8 *) WAVE_bI2C_UDB_StsReg__STATUS_REG)
    #define WAVE_CSR_PTR        ( (reg8 *) WAVE_bI2C_UDB_StsReg__STATUS_REG)

    #define WAVE_INT_MASK_REG   (*(reg8 *) WAVE_bI2C_UDB_StsReg__MASK_REG)
    #define WAVE_INT_MASK_PTR   ( (reg8 *) WAVE_bI2C_UDB_StsReg__MASK_REG)

    #define WAVE_INT_ENABLE_REG (*(reg8 *) WAVE_bI2C_UDB_StsReg__STATUS_AUX_CTL_REG)
    #define WAVE_INT_ENABLE_PTR ( (reg8 *) WAVE_bI2C_UDB_StsReg__STATUS_AUX_CTL_REG)

    #define WAVE_DATA_REG       (*(reg8 *) WAVE_bI2C_UDB_Shifter_u0__A0_REG)
    #define WAVE_DATA_PTR       ( (reg8 *) WAVE_bI2C_UDB_Shifter_u0__A0_REG)

    #define WAVE_GO_REG         (*(reg8 *) WAVE_bI2C_UDB_Shifter_u0__F1_REG)
    #define WAVE_GO_PTR         ( (reg8 *) WAVE_bI2C_UDB_Shifter_u0__F1_REG)

    #define WAVE_GO_DONE_REG    (*(reg8 *) WAVE_bI2C_UDB_Shifter_u0__A1_REG)
    #define WAVE_GO_DONE_PTR    ( (reg8 *) WAVE_bI2C_UDB_Shifter_u0__A1_REG)

    #define WAVE_MCLK_PRD_REG   (*(reg8 *) WAVE_bI2C_UDB_Master_ClkGen_u0__D0_REG)
    #define WAVE_MCLK_PRD_PTR   ( (reg8 *) WAVE_bI2C_UDB_Master_ClkGen_u0__D0_REG)

    #define WAVE_MCLK_CMP_REG   (*(reg8 *) WAVE_bI2C_UDB_Master_ClkGen_u0__D1_REG)
    #define WAVE_MCLK_CMP_PTR   ( (reg8 *) WAVE_bI2C_UDB_Master_ClkGen_u0__D1_REG)

    #if (WAVE_MODE_SLAVE_ENABLED)
        #define WAVE_ADDR_REG       (*(reg8 *) WAVE_bI2C_UDB_Shifter_u0__D0_REG)
        #define WAVE_ADDR_PTR       ( (reg8 *) WAVE_bI2C_UDB_Shifter_u0__D0_REG)

        #define WAVE_PERIOD_REG     (*(reg8 *) WAVE_bI2C_UDB_Slave_BitCounter__PERIOD_REG)
        #define WAVE_PERIOD_PTR     ( (reg8 *) WAVE_bI2C_UDB_Slave_BitCounter__PERIOD_REG)

        #define WAVE_COUNTER_REG    (*(reg8 *) WAVE_bI2C_UDB_Slave_BitCounter__COUNT_REG)
        #define WAVE_COUNTER_PTR    ( (reg8 *) WAVE_bI2C_UDB_Slave_BitCounter__COUNT_REG)

        #define WAVE_COUNTER_AUX_CTL_REG \
                                    (*(reg8 *) WAVE_bI2C_UDB_Slave_BitCounter__CONTROL_AUX_CTL_REG)
        #define WAVE_COUNTER_AUX_CTL_PTR \
                                    ( (reg8 *) WAVE_bI2C_UDB_Slave_BitCounter__CONTROL_AUX_CTL_REG)

    #endif /* (WAVE_MODE_SLAVE_ENABLED) */

#endif /* (WAVE_FF_IMPLEMENTED) */


/***************************************
*        Registers Constants
***************************************/

/* WAVE_I2C_IRQ */
#define WAVE_ISR_NUMBER     ((uint8) WAVE_I2C_IRQ__INTC_NUMBER)
#define WAVE_ISR_PRIORITY   ((uint8) WAVE_I2C_IRQ__INTC_PRIOR_NUM)

/* I2C Slave Data Register */
#define WAVE_SLAVE_ADDR_MASK    (0x7Fu)
#define WAVE_SLAVE_ADDR_SHIFT   (0x01u)
#define WAVE_DATA_MASK          (0xFFu)
#define WAVE_READ_FLAG          (0x01u)

/* Block reset constants */
#define WAVE_CLEAR_REG          (0x00u)
#define WAVE_BLOCK_RESET_DELAY  (2u)
#define WAVE_FF_RESET_DELAY     (WAVE_BLOCK_RESET_DELAY)
#define WAVE_RESTORE_TIMEOUT    (255u)

#if (WAVE_FF_IMPLEMENTED)
    /* XCFG I2C Extended Configuration Register */
    #define WAVE_XCFG_CLK_EN        (0x80u) /* Enable gated clock to block */
    #define WAVE_XCFG_I2C_ON        (0x40u) /* Enable I2C as wake up source*/
    #define WAVE_XCFG_RDY_TO_SLEEP  (0x20u) /* I2C ready go to sleep */
    #define WAVE_XCFG_FORCE_NACK    (0x10u) /* Force NACK all incoming transactions */
    #define WAVE_XCFG_NO_BC_INT     (0x08u) /* No interrupt on byte complete */
    #define WAVE_XCFG_BUF_MODE      (0x02u) /* Enable buffer mode */
    #define WAVE_XCFG_HDWR_ADDR_EN  (0x01u) /* Enable Hardware address match */

    /* CFG I2C Configuration Register */
    #define WAVE_CFG_SIO_SELECT     (0x80u) /* Pin Select for SCL/SDA lines */
    #define WAVE_CFG_PSELECT        (0x40u) /* Pin Select */
    #define WAVE_CFG_BUS_ERR_IE     (0x20u) /* Bus Error Interrupt Enable */
    #define WAVE_CFG_STOP_IE        (0x10u) /* Enable Interrupt on STOP condition */
    #define WAVE_CFG_CLK_RATE_MSK   (0x0Cu) /* Clock rate select */
    #define WAVE_CFG_CLK_RATE_100   (0x00u) /* Clock rate select 100K */
    #define WAVE_CFG_CLK_RATE_400   (0x04u) /* Clock rate select 400K */
    #define WAVE_CFG_CLK_RATE_050   (0x08u) /* Clock rate select 50K  */
    #define WAVE_CFG_CLK_RATE_RSVD  (0x0Cu) /* Clock rate select Invalid */
    #define WAVE_CFG_EN_MSTR        (0x02u) /* Enable Master operation */
    #define WAVE_CFG_EN_SLAVE       (0x01u) /* Enable Slave operation */

    #define WAVE_CFG_CLK_RATE_LESS_EQUAL_50 (0x04u) /* Clock rate select <= 50kHz */
    #define WAVE_CFG_CLK_RATE_GRATER_50     (0x00u) /* Clock rate select > 50kHz */

    /* CSR I2C Control and Status Register */
    #define WAVE_CSR_BUS_ERROR      (0x80u) /* Active high when bus error has occurred */
    #define WAVE_CSR_LOST_ARB       (0x40u) /* Set to 1 if lost arbitration in host mode */
    #define WAVE_CSR_STOP_STATUS    (0x20u) /* Set if Stop has been detected */
    #define WAVE_CSR_ACK            (0x10u) /* ACK response */
    #define WAVE_CSR_NAK            (0x00u) /* NAK response */
    #define WAVE_CSR_ADDRESS        (0x08u) /* Set in firmware 0 = status bit, 1 Address is slave */
    #define WAVE_CSR_TRANSMIT       (0x04u) /* Set in firmware 1 = transmit, 0 = receive */
    #define WAVE_CSR_LRB            (0x02u) /* Last received bit */
    #define WAVE_CSR_LRB_ACK        (0x00u) /* Last received bit was an ACK */
    #define WAVE_CSR_LRB_NAK        (0x02u) /* Last received bit was an NAK */
    #define WAVE_CSR_BYTE_COMPLETE  (0x01u) /* Informs that last byte has been sent */
    #define WAVE_CSR_STOP_GEN       (0x00u) /* Generate a stop condition */
    #define WAVE_CSR_RDY_TO_RD      (0x00u) /* Set to receive mode */

    /* MCSR I2C Master Control and Status Register */
    #define WAVE_MCSR_STOP_GEN      (0x10u) /* Firmware sets this bit to initiate a Stop condition */
    #define WAVE_MCSR_BUS_BUSY      (0x08u) /* Status bit, Set at Start and cleared at Stop condition */
    #define WAVE_MCSR_MSTR_MODE     (0x04u) /* Status bit, Set at Start and cleared at Stop condition */
    #define WAVE_MCSR_RESTART_GEN   (0x02u) /* Firmware sets this bit to initiate a ReStart condition */
    #define WAVE_MCSR_START_GEN     (0x01u) /* Firmware sets this bit to initiate a Start condition */

    /* PWRSYS_CR1 to handle Sleep */
    #define WAVE_PWRSYS_CR1_I2C_REG_BACKUP  (0x04u) /* Enables, power to I2C regs while sleep */

#else
    /* CONTROL REG bits location */
    #define WAVE_CTRL_START_SHIFT           (7u)
    #define WAVE_CTRL_STOP_SHIFT            (6u)
    #define WAVE_CTRL_RESTART_SHIFT         (5u)
    #define WAVE_CTRL_NACK_SHIFT            (4u)
    #define WAVE_CTRL_ANY_ADDRESS_SHIFT     (3u)
    #define WAVE_CTRL_TRANSMIT_SHIFT        (2u)
    #define WAVE_CTRL_ENABLE_MASTER_SHIFT   (1u)
    #define WAVE_CTRL_ENABLE_SLAVE_SHIFT    (0u)
    #define WAVE_CTRL_START_MASK            ((uint8) (0x01u << WAVE_CTRL_START_SHIFT))
    #define WAVE_CTRL_STOP_MASK             ((uint8) (0x01u << WAVE_CTRL_STOP_SHIFT))
    #define WAVE_CTRL_RESTART_MASK          ((uint8) (0x01u << WAVE_CTRL_RESTART_SHIFT))
    #define WAVE_CTRL_NACK_MASK             ((uint8) (0x01u << WAVE_CTRL_NACK_SHIFT))
    #define WAVE_CTRL_ANY_ADDRESS_MASK      ((uint8) (0x01u << WAVE_CTRL_ANY_ADDRESS_SHIFT))
    #define WAVE_CTRL_TRANSMIT_MASK         ((uint8) (0x01u << WAVE_CTRL_TRANSMIT_SHIFT))
    #define WAVE_CTRL_ENABLE_MASTER_MASK    ((uint8) (0x01u << WAVE_CTRL_ENABLE_MASTER_SHIFT))
    #define WAVE_CTRL_ENABLE_SLAVE_MASK     ((uint8) (0x01u << WAVE_CTRL_ENABLE_SLAVE_SHIFT))

    /* STATUS REG bits location */
    #define WAVE_STS_LOST_ARB_SHIFT         (6u)
    #define WAVE_STS_STOP_SHIFT             (5u)
    #define WAVE_STS_BUSY_SHIFT             (4u)
    #define WAVE_STS_ADDR_SHIFT             (3u)
    #define WAVE_STS_MASTER_MODE_SHIFT      (2u)
    #define WAVE_STS_LRB_SHIFT              (1u)
    #define WAVE_STS_BYTE_COMPLETE_SHIFT    (0u)
    #define WAVE_STS_LOST_ARB_MASK          ((uint8) (0x01u << WAVE_STS_LOST_ARB_SHIFT))
    #define WAVE_STS_STOP_MASK              ((uint8) (0x01u << WAVE_STS_STOP_SHIFT))
    #define WAVE_STS_BUSY_MASK              ((uint8) (0x01u << WAVE_STS_BUSY_SHIFT))
    #define WAVE_STS_ADDR_MASK              ((uint8) (0x01u << WAVE_STS_ADDR_SHIFT))
    #define WAVE_STS_MASTER_MODE_MASK       ((uint8) (0x01u << WAVE_STS_MASTER_MODE_SHIFT))
    #define WAVE_STS_LRB_MASK               ((uint8) (0x01u << WAVE_STS_LRB_SHIFT))
    #define WAVE_STS_BYTE_COMPLETE_MASK     ((uint8) (0x01u << WAVE_STS_BYTE_COMPLETE_SHIFT))

    /* AUX_CTL bits definition */
    #define WAVE_COUNTER_ENABLE_MASK        (0x20u) /* Enable 7-bit counter */
    #define WAVE_INT_ENABLE_MASK            (0x10u) /* Enable interrupt from status register */
    #define WAVE_CNT7_ENABLE                (WAVE_COUNTER_ENABLE_MASK)
    #define WAVE_INTR_ENABLE                (WAVE_INT_ENABLE_MASK)

#endif /* (WAVE_FF_IMPLEMENTED) */


/***************************************
*        Marco
***************************************/

/* ACK and NACK for data and address checks */
#define WAVE_CHECK_ADDR_ACK(csr)    ((WAVE_CSR_LRB_ACK | WAVE_CSR_ADDRESS) == \
                                                 ((WAVE_CSR_LRB    | WAVE_CSR_ADDRESS) &  \
                                                  (csr)))


#define WAVE_CHECK_ADDR_NAK(csr)    ((WAVE_CSR_LRB_NAK | WAVE_CSR_ADDRESS) == \
                                                 ((WAVE_CSR_LRB    | WAVE_CSR_ADDRESS) &  \
                                                  (csr)))

#define WAVE_CHECK_DATA_ACK(csr)    (0u == ((csr) & WAVE_CSR_LRB_NAK))

/* MCSR conditions check */
#define WAVE_CHECK_BUS_FREE(mcsr)       (0u == ((mcsr) & WAVE_MCSR_BUS_BUSY))
#define WAVE_CHECK_MASTER_MODE(mcsr)    (0u != ((mcsr) & WAVE_MCSR_MSTR_MODE))

/* CSR conditions check */
#define WAVE_WAIT_BYTE_COMPLETE(csr)    (0u == ((csr) & WAVE_CSR_BYTE_COMPLETE))
#define WAVE_WAIT_STOP_COMPLETE(csr)    (0u == ((csr) & (WAVE_CSR_BYTE_COMPLETE | \
                                                                     WAVE_CSR_STOP_STATUS)))
#define WAVE_CHECK_BYTE_COMPLETE(csr)   (0u != ((csr) & WAVE_CSR_BYTE_COMPLETE))
#define WAVE_CHECK_STOP_STS(csr)        (0u != ((csr) & WAVE_CSR_STOP_STATUS))
#define WAVE_CHECK_LOST_ARB(csr)        (0u != ((csr) & WAVE_CSR_LOST_ARB))
#define WAVE_CHECK_ADDRESS_STS(csr)     (0u != ((csr) & WAVE_CSR_ADDRESS))

/* Software start and end of transaction check */
#define WAVE_CHECK_RESTART(mstrCtrl)    (0u != ((mstrCtrl) & WAVE_MODE_REPEAT_START))
#define WAVE_CHECK_NO_STOP(mstrCtrl)    (0u != ((mstrCtrl) & WAVE_MODE_NO_STOP))

/* Send read or write completion depends on state */
#define WAVE_GET_MSTAT_CMPLT ((0u != (WAVE_state & WAVE_SM_MSTR_RD)) ? \
                                                 (WAVE_MSTAT_RD_CMPLT) : (WAVE_MSTAT_WR_CMPLT))

/* Returns 7-bit slave address */
#define WAVE_GET_SLAVE_ADDR(dataReg)   (((dataReg) >> WAVE_SLAVE_ADDR_SHIFT) & \
                                                                  WAVE_SLAVE_ADDR_MASK)

#if (WAVE_FF_IMPLEMENTED)
    /* Check enable of module */
    #define WAVE_I2C_ENABLE_REG     (WAVE_ACT_PWRMGR_REG)
    #define WAVE_IS_I2C_ENABLE(reg) (0u != ((reg) & WAVE_ACT_PWR_EN))
    #define WAVE_IS_ENABLED         (0u != (WAVE_ACT_PWRMGR_REG & WAVE_ACT_PWR_EN))

    #define WAVE_CHECK_PWRSYS_I2C_BACKUP    (0u != (WAVE_PWRSYS_CR1_I2C_REG_BACKUP & \
                                                                WAVE_PWRSYS_CR1_REG))

    /* Check start condition generation */
    #define WAVE_CHECK_START_GEN(mcsr)  ((0u != ((mcsr) & WAVE_MCSR_START_GEN)) && \
                                                     (0u == ((mcsr) & WAVE_MCSR_MSTR_MODE)))

    #define WAVE_CLEAR_START_GEN        do{ \
                                                        WAVE_MCSR_REG &=                                   \
                                                                           ((uint8) ~WAVE_MCSR_START_GEN); \
                                                    }while(0)

    /* Stop interrupt */
    #define WAVE_ENABLE_INT_ON_STOP     do{ \
                                                        WAVE_CFG_REG |= WAVE_CFG_STOP_IE; \
                                                    }while(0)

    #define WAVE_DISABLE_INT_ON_STOP    do{ \
                                                        WAVE_CFG_REG &=                                 \
                                                                           ((uint8) ~WAVE_CFG_STOP_IE); \
                                                    }while(0)

    /* Transmit data */
    #define WAVE_TRANSMIT_DATA          do{ \
                                                        WAVE_CSR_REG = WAVE_CSR_TRANSMIT; \
                                                    }while(0)

    #define WAVE_ACK_AND_TRANSMIT       do{ \
                                                        WAVE_CSR_REG = (WAVE_CSR_ACK |      \
                                                                                    WAVE_CSR_TRANSMIT); \
                                                    }while(0)

    #define WAVE_NAK_AND_TRANSMIT       do{ \
                                                        WAVE_CSR_REG = WAVE_CSR_NAK; \
                                                    }while(0)

    /* Special case: udb needs to ack, ff needs to nak */
    #define WAVE_ACKNAK_AND_TRANSMIT    do{ \
                                                        WAVE_CSR_REG  = (WAVE_CSR_NAK |      \
                                                                                     WAVE_CSR_TRANSMIT); \
                                                    }while(0)
    /* Receive data */
    #define WAVE_ACK_AND_RECEIVE        do{ \
                                                        WAVE_CSR_REG = WAVE_CSR_ACK; \
                                                    }while(0)

    #define WAVE_NAK_AND_RECEIVE        do{ \
                                                        WAVE_CSR_REG = WAVE_CSR_NAK; \
                                                    }while(0)

    #define WAVE_READY_TO_READ          do{ \
                                                        WAVE_CSR_REG = WAVE_CSR_RDY_TO_RD; \
                                                    }while(0)

    /* Release bus after lost arbitration */
    #define WAVE_BUS_RELEASE    WAVE_READY_TO_READ

    /* Master Start/ReStart/Stop conditions generation */
    #define WAVE_GENERATE_START         do{ \
                                                        WAVE_MCSR_REG = WAVE_MCSR_START_GEN; \
                                                    }while(0)

    #define WAVE_GENERATE_RESTART \
                    do{                       \
                        WAVE_MCSR_REG = (WAVE_MCSR_RESTART_GEN | \
                                                     WAVE_MCSR_STOP_GEN);    \
                        WAVE_CSR_REG  = WAVE_CSR_TRANSMIT;       \
                    }while(0)

    #define WAVE_GENERATE_STOP \
                    do{                    \
                        WAVE_MCSR_REG = WAVE_MCSR_STOP_GEN; \
                        WAVE_CSR_REG  = WAVE_CSR_TRANSMIT;  \
                    }while(0)

    /* Master manual APIs compatible defines */
    #define WAVE_GENERATE_START_MANUAL      WAVE_GENERATE_START
    #define WAVE_GENERATE_RESTART_MANUAL    WAVE_GENERATE_RESTART
    #define WAVE_GENERATE_STOP_MANUAL       WAVE_GENERATE_STOP
    #define WAVE_TRANSMIT_DATA_MANUAL       WAVE_TRANSMIT_DATA
    #define WAVE_READY_TO_READ_MANUAL       WAVE_READY_TO_READ
    #define WAVE_ACK_AND_RECEIVE_MANUAL     WAVE_ACK_AND_RECEIVE
    #define WAVE_BUS_RELEASE_MANUAL         WAVE_BUS_RELEASE

#else

    /* Masks to enable interrupts from Status register */
    #define WAVE_STOP_IE_MASK           (WAVE_STS_STOP_MASK)
    #define WAVE_BYTE_COMPLETE_IE_MASK  (WAVE_STS_BYTE_COMPLETE_MASK)

    /* FF compatibility: CSR register bit-fields */
    #define WAVE_CSR_LOST_ARB       (WAVE_STS_LOST_ARB_MASK)
    #define WAVE_CSR_STOP_STATUS    (WAVE_STS_STOP_MASK)
    #define WAVE_CSR_BUS_ERROR      (0x00u)
    #define WAVE_CSR_ADDRESS        (WAVE_STS_ADDR_MASK)
    #define WAVE_CSR_TRANSMIT       (WAVE_CTRL_TRANSMIT_MASK)
    #define WAVE_CSR_LRB            (WAVE_STS_LRB_MASK)
    #define WAVE_CSR_LRB_NAK        (WAVE_STS_LRB_MASK)
    #define WAVE_CSR_LRB_ACK        (0x00u)
    #define WAVE_CSR_BYTE_COMPLETE  (WAVE_STS_BYTE_COMPLETE_MASK)

    /* FF compatibility: MCSR registers bit-fields */
    #define WAVE_MCSR_REG           (WAVE_CSR_REG)  /* UDB incorporates master and slave regs */
    #define WAVE_MCSR_BUS_BUSY      (WAVE_STS_BUSY_MASK)      /* Is bus is busy               */
    #define WAVE_MCSR_START_GEN     (WAVE_CTRL_START_MASK)    /* Generate Start condition     */
    #define WAVE_MCSR_RESTART_GEN   (WAVE_CTRL_RESTART_MASK)  /* Generates RESTART condition  */
    #define WAVE_MCSR_MSTR_MODE     (WAVE_STS_MASTER_MODE_MASK)/* Define if active Master     */

    /* Data to write into TX FIFO to release FSM */
    #define WAVE_PREPARE_TO_RELEASE (0xFFu)
    #define WAVE_RELEASE_FSM        (0x00u)

    /* Define release command done: history of byte complete status is cleared */
    #define WAVE_WAIT_RELEASE_CMD_DONE  (WAVE_RELEASE_FSM != WAVE_GO_DONE_REG)

    /* Check enable of module */
    #define WAVE_I2C_ENABLE_REG     (WAVE_CFG_REG)
    #define WAVE_IS_I2C_ENABLE(reg) ((0u != ((reg) & WAVE_ENABLE_MASTER)) || \
                                                 (0u != ((reg) & WAVE_ENABLE_SLAVE)))

    #define WAVE_IS_ENABLED         (0u != (WAVE_CFG_REG & WAVE_ENABLE_MS))

    /* Check start condition generation */
    #define WAVE_CHECK_START_GEN(mcsr)  ((0u != (WAVE_CFG_REG &        \
                                                             WAVE_MCSR_START_GEN)) \
                                                    &&                                         \
                                                    (0u == ((mcsr) & WAVE_MCSR_MSTR_MODE)))

    #define WAVE_CLEAR_START_GEN        do{ \
                                                        WAVE_CFG_REG &=                 \
                                                        ((uint8) ~WAVE_MCSR_START_GEN); \
                                                    }while(0)

    /* Stop interrupt */
    #define WAVE_ENABLE_INT_ON_STOP     do{ \
                                                       WAVE_INT_MASK_REG |= WAVE_STOP_IE_MASK; \
                                                    }while(0)

    #define WAVE_DISABLE_INT_ON_STOP    do{ \
                                                        WAVE_INT_MASK_REG &=                               \
                                                                             ((uint8) ~WAVE_STOP_IE_MASK); \
                                                    }while(0)

    /* Transmit data */
    #define WAVE_TRANSMIT_DATA \
                                    do{    \
                                        WAVE_CFG_REG     = (WAVE_CTRL_TRANSMIT_MASK | \
                                                                       WAVE_CTRL_DEFAULT);        \
                                        WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE;   \
                                        WAVE_GO_REG      = WAVE_RELEASE_FSM;          \
                                    }while(0)

    #define WAVE_ACK_AND_TRANSMIT   WAVE_TRANSMIT_DATA

    #define WAVE_NAK_AND_TRANSMIT \
                                        do{   \
                                            WAVE_CFG_REG     = (WAVE_CTRL_NACK_MASK     | \
                                                                            WAVE_CTRL_TRANSMIT_MASK | \
                                                                            WAVE_CTRL_DEFAULT);       \
                                            WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE;   \
                                            WAVE_GO_REG      = WAVE_RELEASE_FSM;          \
                                        }while(0)

    /* Receive data */
    #define WAVE_READY_TO_READ  \
                                        do{ \
                                            WAVE_CFG_REG     = WAVE_CTRL_DEFAULT;       \
                                            WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE; \
                                            WAVE_GO_REG      = WAVE_RELEASE_FSM;       \
                                        }while(0)

    #define WAVE_ACK_AND_RECEIVE    WAVE_READY_TO_READ

    /* Release bus after arbitration is lost */
    #define WAVE_BUS_RELEASE    WAVE_READY_TO_READ

    #define WAVE_NAK_AND_RECEIVE \
                                        do{  \
                                            WAVE_CFG_REG     = (WAVE_CTRL_NACK_MASK |   \
                                                                            WAVE_CTRL_DEFAULT);     \
                                            WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE; \
                                            WAVE_GO_REG      = WAVE_RELEASE_FSM;       \
                                        }while(0)

    /* Master condition generation */
    #define WAVE_GENERATE_START \
                                        do{ \
                                            WAVE_CFG_REG     = (WAVE_CTRL_START_MASK |  \
                                                                            WAVE_CTRL_DEFAULT);     \
                                            WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE; \
                                            WAVE_GO_REG      = WAVE_RELEASE_FSM;       \
                                        }while(0)

    #define WAVE_GENERATE_RESTART \
                                        do{   \
                                            WAVE_CFG_REG     = (WAVE_CTRL_RESTART_MASK | \
                                                                            WAVE_CTRL_NACK_MASK    | \
                                                                            WAVE_CTRL_DEFAULT);      \
                                            WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE;  \
                                            WAVE_GO_REG  =     WAVE_RELEASE_FSM;         \
                                        }while(0)

    #define WAVE_GENERATE_STOP  \
                                        do{ \
                                            WAVE_CFG_REG    = (WAVE_CTRL_NACK_MASK |    \
                                                                           WAVE_CTRL_STOP_MASK |    \
                                                                           WAVE_CTRL_DEFAULT);      \
                                            WAVE_GO_DONE_REG = WAVE_PREPARE_TO_RELEASE; \
                                            WAVE_GO_REG      = WAVE_RELEASE_FSM;        \
                                        }while(0)

    /* Master manual APIs compatible macros */
    /* These macros wait until byte complete history is cleared after command issued */
    #define WAVE_GENERATE_START_MANUAL \
                                        do{ \
                                            WAVE_GENERATE_START;                  \
                                            /* Wait until byte complete history is cleared */ \
                                            while(WAVE_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)
                                        
    #define WAVE_GENERATE_RESTART_MANUAL \
                                        do{          \
                                            WAVE_GENERATE_RESTART;                \
                                            /* Wait until byte complete history is cleared */ \
                                            while(WAVE_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define WAVE_GENERATE_STOP_MANUAL \
                                        do{       \
                                            WAVE_GENERATE_STOP;                   \
                                            /* Wait until byte complete history is cleared */ \
                                            while(WAVE_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define WAVE_TRANSMIT_DATA_MANUAL \
                                        do{       \
                                            WAVE_TRANSMIT_DATA;                   \
                                            /* Wait until byte complete history is cleared */ \
                                            while(WAVE_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define WAVE_READY_TO_READ_MANUAL \
                                        do{       \
                                            WAVE_READY_TO_READ;                   \
                                            /* Wait until byte complete history is cleared */ \
                                            while(WAVE_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define WAVE_ACK_AND_RECEIVE_MANUAL \
                                        do{         \
                                            WAVE_ACK_AND_RECEIVE;                 \
                                            /* Wait until byte complete history is cleared */ \
                                            while(WAVE_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define WAVE_BUS_RELEASE_MANUAL WAVE_READY_TO_READ_MANUAL

#endif /* (WAVE_FF_IMPLEMENTED) */


/***************************************
*     Default register init constants
***************************************/

#define WAVE_DISABLE    (0u)
#define WAVE_ENABLE     (1u)

#if (WAVE_FF_IMPLEMENTED)
    /* WAVE_XCFG_REG: bits definition */
    #define WAVE_DEFAULT_XCFG_HW_ADDR_EN ((WAVE_HW_ADRR_DECODE) ? \
                                                        (WAVE_XCFG_HDWR_ADDR_EN) : (0u))

    #define WAVE_DEFAULT_XCFG_I2C_ON    ((WAVE_WAKEUP_ENABLED) ? \
                                                        (WAVE_XCFG_I2C_ON) : (0u))


    #define WAVE_DEFAULT_CFG_SIO_SELECT ((WAVE_I2C1_SIO_PAIR) ? \
                                                        (WAVE_CFG_SIO_SELECT) : (0u))


    /* WAVE_CFG_REG: bits definition */
    #define WAVE_DEFAULT_CFG_PSELECT    ((WAVE_WAKEUP_ENABLED) ? \
                                                        (WAVE_CFG_PSELECT) : (0u))

    #define WAVE_DEFAULT_CLK_RATE0  ((WAVE_DATA_RATE <= 50u) ?        \
                                                    (WAVE_CFG_CLK_RATE_050) :     \
                                                    ((WAVE_DATA_RATE <= 100u) ?   \
                                                        (WAVE_CFG_CLK_RATE_100) : \
                                                        (WAVE_CFG_CLK_RATE_400)))

    #define WAVE_DEFAULT_CLK_RATE1  ((WAVE_DATA_RATE <= 50u) ?           \
                                                 (WAVE_CFG_CLK_RATE_LESS_EQUAL_50) : \
                                                 (WAVE_CFG_CLK_RATE_GRATER_50))

    #define WAVE_DEFAULT_CLK_RATE   (WAVE_DEFAULT_CLK_RATE1)


    #define WAVE_ENABLE_MASTER      ((WAVE_MODE_MASTER_ENABLED) ? \
                                                 (WAVE_CFG_EN_MSTR) : (0u))

    #define WAVE_ENABLE_SLAVE       ((WAVE_MODE_SLAVE_ENABLED) ? \
                                                 (WAVE_CFG_EN_SLAVE) : (0u))

    #define WAVE_ENABLE_MS      (WAVE_ENABLE_MASTER | WAVE_ENABLE_SLAVE)


    /* WAVE_DEFAULT_XCFG_REG */
    #define WAVE_DEFAULT_XCFG   (WAVE_XCFG_CLK_EN         | \
                                             WAVE_DEFAULT_XCFG_I2C_ON | \
                                             WAVE_DEFAULT_XCFG_HW_ADDR_EN)

    /* WAVE_DEFAULT_CFG_REG */
    #define WAVE_DEFAULT_CFG    (WAVE_DEFAULT_CFG_SIO_SELECT | \
                                             WAVE_DEFAULT_CFG_PSELECT    | \
                                             WAVE_DEFAULT_CLK_RATE       | \
                                             WAVE_ENABLE_MASTER          | \
                                             WAVE_ENABLE_SLAVE)

    /*WAVE_DEFAULT_DIVIDE_FACTOR_REG */
    #define WAVE_DEFAULT_DIVIDE_FACTOR  ((uint16) 15u)

#else
    /* WAVE_CFG_REG: bits definition  */
    #define WAVE_ENABLE_MASTER  ((WAVE_MODE_MASTER_ENABLED) ? \
                                             (WAVE_CTRL_ENABLE_MASTER_MASK) : (0u))

    #define WAVE_ENABLE_SLAVE   ((WAVE_MODE_SLAVE_ENABLED) ? \
                                             (WAVE_CTRL_ENABLE_SLAVE_MASK) : (0u))

    #define WAVE_ENABLE_MS      (WAVE_ENABLE_MASTER | WAVE_ENABLE_SLAVE)


    #define WAVE_DEFAULT_CTRL_ANY_ADDR   ((WAVE_HW_ADRR_DECODE) ? \
                                                      (0u) : (WAVE_CTRL_ANY_ADDRESS_MASK))

    /* WAVE_DEFAULT_CFG_REG */
    #define WAVE_DEFAULT_CFG    (WAVE_DEFAULT_CTRL_ANY_ADDR)

    /* All CTRL default bits to be used in macro */
    #define WAVE_CTRL_DEFAULT   (WAVE_DEFAULT_CTRL_ANY_ADDR | WAVE_ENABLE_MS)

    /* Master clock generator: d0 and d1 */
    #define WAVE_MCLK_PERIOD_VALUE  (0x0Fu)
    #define WAVE_MCLK_COMPARE_VALUE (0x08u)

    /* Slave bit-counter: control period */
    #define WAVE_PERIOD_VALUE       (0x07u)

    /* WAVE_DEFAULT_INT_MASK */
    #define WAVE_DEFAULT_INT_MASK   (WAVE_BYTE_COMPLETE_IE_MASK)

    /* WAVE_DEFAULT_MCLK_PRD_REG */
    #define WAVE_DEFAULT_MCLK_PRD   (WAVE_MCLK_PERIOD_VALUE)

    /* WAVE_DEFAULT_MCLK_CMP_REG */
    #define WAVE_DEFAULT_MCLK_CMP   (WAVE_MCLK_COMPARE_VALUE)

    /* WAVE_DEFAULT_PERIOD_REG */
    #define WAVE_DEFAULT_PERIOD     (WAVE_PERIOD_VALUE)

#endif /* (WAVE_FF_IMPLEMENTED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

#define WAVE_SSTAT_RD_ERR       (0x08u)
#define WAVE_SSTAT_WR_ERR       (0x80u)
#define WAVE_MSTR_SLAVE_BUSY    (WAVE_MSTR_NOT_READY)
#define WAVE_MSTAT_ERR_BUF_OVFL (0x80u)
#define WAVE_SSTAT_RD_CMPT      (WAVE_SSTAT_RD_CMPLT)
#define WAVE_SSTAT_WR_CMPT      (WAVE_SSTAT_WR_CMPLT)
#define WAVE_MODE_MULTI_MASTER_ENABLE    (WAVE_MODE_MULTI_MASTER_MASK)
#define WAVE_DATA_RATE_50       (50u)
#define WAVE_DATA_RATE_100      (100u)
#define WAVE_DEV_MASK           (0xF0u)
#define WAVE_SM_SL_STOP         (0x14u)
#define WAVE_SM_MASTER_IDLE     (0x40u)
#define WAVE_HDWR_DECODE        (0x01u)

#endif /* CY_I2C_WAVE_H */


/* [] END OF FILE */

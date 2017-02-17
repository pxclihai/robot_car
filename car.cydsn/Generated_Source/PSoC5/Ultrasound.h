/*******************************************************************************
* File Name: Ultrasound.h
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

#if !defined(CY_I2C_Ultrasound_H)
#define CY_I2C_Ultrasound_H

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

#define Ultrasound_IMPLEMENTATION     (1u)
#define Ultrasound_MODE               (2u)
#define Ultrasound_ENABLE_WAKEUP      (0u)
#define Ultrasound_ADDR_DECODE        (1u)
#define Ultrasound_UDB_INTRN_CLOCK    (1u)


/* I2C implementation enum */
#define Ultrasound_UDB    (0x00u)
#define Ultrasound_FF     (0x01u)

#define Ultrasound_FF_IMPLEMENTED     (Ultrasound_FF  == Ultrasound_IMPLEMENTATION)
#define Ultrasound_UDB_IMPLEMENTED    (Ultrasound_UDB == Ultrasound_IMPLEMENTATION)

#define Ultrasound_UDB_INTRN_CLOCK_ENABLED    (Ultrasound_UDB_IMPLEMENTED && \
                                                     (0u != Ultrasound_UDB_INTRN_CLOCK))
/* I2C modes enum */
#define Ultrasound_MODE_SLAVE                 (0x01u)
#define Ultrasound_MODE_MASTER                (0x02u)
#define Ultrasound_MODE_MULTI_MASTER          (0x06u)
#define Ultrasound_MODE_MULTI_MASTER_SLAVE    (0x07u)
#define Ultrasound_MODE_MULTI_MASTER_MASK     (0x04u)

#define Ultrasound_MODE_SLAVE_ENABLED         (0u != (Ultrasound_MODE_SLAVE  & Ultrasound_MODE))
#define Ultrasound_MODE_MASTER_ENABLED        (0u != (Ultrasound_MODE_MASTER & Ultrasound_MODE))
#define Ultrasound_MODE_MULTI_MASTER_ENABLED  (0u != (Ultrasound_MODE_MULTI_MASTER_MASK & \
                                                            Ultrasound_MODE))
#define Ultrasound_MODE_MULTI_MASTER_SLAVE_ENABLED    (Ultrasound_MODE_MULTI_MASTER_SLAVE == \
                                                             Ultrasound_MODE)

/* Address detection enum */
#define Ultrasound_SW_DECODE      (0x00u)
#define Ultrasound_HW_DECODE      (0x01u)

#define Ultrasound_SW_ADRR_DECODE             (Ultrasound_SW_DECODE == Ultrasound_ADDR_DECODE)
#define Ultrasound_HW_ADRR_DECODE             (Ultrasound_HW_DECODE == Ultrasound_ADDR_DECODE)

/* Wakeup enabled */
#define Ultrasound_WAKEUP_ENABLED             (0u != Ultrasound_ENABLE_WAKEUP)

/* Adds bootloader APIs to component */
#define Ultrasound_BOOTLOADER_INTERFACE_ENABLED   (Ultrasound_MODE_SLAVE_ENABLED && \
                                                            ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Ultrasound) || \
                                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface)))

/* Timeout functionality */
#define Ultrasound_TIMEOUT_ENABLE             (0u)
#define Ultrasound_TIMEOUT_SCL_TMOUT_ENABLE   (0u)
#define Ultrasound_TIMEOUT_SDA_TMOUT_ENABLE   (0u)
#define Ultrasound_TIMEOUT_PRESCALER_ENABLE   (0u)
#define Ultrasound_TIMEOUT_IMPLEMENTATION     (0u)

/* Convert to boolean */
#define Ultrasound_TIMEOUT_ENABLED            (0u != Ultrasound_TIMEOUT_ENABLE)
#define Ultrasound_TIMEOUT_SCL_TMOUT_ENABLED  (0u != Ultrasound_TIMEOUT_SCL_TMOUT_ENABLE)
#define Ultrasound_TIMEOUT_SDA_TMOUT_ENABLED  (0u != Ultrasound_TIMEOUT_SDA_TMOUT_ENABLE)
#define Ultrasound_TIMEOUT_PRESCALER_ENABLED  (0u != Ultrasound_TIMEOUT_PRESCALER_ENABLE)

/* Timeout implementation enum. */
#define Ultrasound_TIMEOUT_UDB    (0x00u)
#define Ultrasound_TIMEOUT_FF     (0x01u)

#define Ultrasound_TIMEOUT_FF_IMPLEMENTED     (Ultrasound_TIMEOUT_FF  == \
                                                        Ultrasound_TIMEOUT_IMPLEMENTATION)
#define Ultrasound_TIMEOUT_UDB_IMPLEMENTED    (Ultrasound_TIMEOUT_UDB == \
                                                        Ultrasound_TIMEOUT_IMPLEMENTATION)

#define Ultrasound_TIMEOUT_FF_ENABLED         (Ultrasound_TIMEOUT_ENABLED && \
                                                     Ultrasound_TIMEOUT_FF_IMPLEMENTED)

#define Ultrasound_TIMEOUT_UDB_ENABLED        (Ultrasound_TIMEOUT_ENABLED && \
                                                     Ultrasound_TIMEOUT_UDB_IMPLEMENTED)

#define Ultrasound_EXTERN_I2C_INTR_HANDLER    (0u)
#define Ultrasound_EXTERN_TMOUT_INTR_HANDLER  (0u)

#define Ultrasound_INTERN_I2C_INTR_HANDLER    (0u == Ultrasound_EXTERN_I2C_INTR_HANDLER)
#define Ultrasound_INTERN_TMOUT_INTR_HANDLER  (0u == Ultrasound_EXTERN_TMOUT_INTR_HANDLER)


/***************************************
*       Type defines
***************************************/

/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;

#if (Ultrasound_FF_IMPLEMENTED)
    uint8 xcfg;
    uint8 cfg;
    uint8 addr;
    uint8 clkDiv1;
    uint8 clkDiv2;
#else
    uint8 control;
#endif /* (Ultrasound_FF_IMPLEMENTED) */

#if (Ultrasound_TIMEOUT_ENABLED)
    uint16 tmoutCfg;
    uint8  tmoutIntr;
#endif /* (Ultrasound_TIMEOUT_ENABLED) */

} Ultrasound_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void Ultrasound_Init(void)                            ;
void Ultrasound_Enable(void)                          ;

void Ultrasound_Start(void)                           ;
void Ultrasound_Stop(void)                            ;

#define Ultrasound_EnableInt()        CyIntEnable      (Ultrasound_ISR_NUMBER)
#define Ultrasound_DisableInt()       CyIntDisable     (Ultrasound_ISR_NUMBER)
#define Ultrasound_ClearPendingInt()  CyIntClearPending(Ultrasound_ISR_NUMBER)
#define Ultrasound_SetPendingInt()    CyIntSetPending  (Ultrasound_ISR_NUMBER)

void Ultrasound_SaveConfig(void)                      ;
void Ultrasound_Sleep(void)                           ;
void Ultrasound_RestoreConfig(void)                   ;
void Ultrasound_Wakeup(void)                          ;

/* I2C Master functions prototypes */
#if (Ultrasound_MODE_MASTER_ENABLED)
    /* Read and Clear status functions */
    uint8 Ultrasound_MasterStatus(void)                ;
    uint8 Ultrasound_MasterClearStatus(void)           ;

    /* Interrupt based operation functions */
    uint8 Ultrasound_MasterWriteBuf(uint8 slaveAddress, uint8 * wrData, uint8 cnt, uint8 mode) \
                                                            ;
    uint8 Ultrasound_MasterReadBuf(uint8 slaveAddress, uint8 * rdData, uint8 cnt, uint8 mode) \
                                                            ;
    uint8 Ultrasound_MasterGetReadBufSize(void)       ;
    uint8 Ultrasound_MasterGetWriteBufSize(void)      ;
    void  Ultrasound_MasterClearReadBuf(void)         ;
    void  Ultrasound_MasterClearWriteBuf(void)        ;

    /* Manual operation functions */
    uint8 Ultrasound_MasterSendStart(uint8 slaveAddress, uint8 R_nW) \
                                                            ;
    uint8 Ultrasound_MasterSendRestart(uint8 slaveAddress, uint8 R_nW) \
                                                            ;
    uint8 Ultrasound_MasterSendStop(void)             ;
    uint8 Ultrasound_MasterWriteByte(uint8 theByte)   ;
    uint8 Ultrasound_MasterReadByte(uint8 acknNak)    ;

#endif /* (Ultrasound_MODE_MASTER_ENABLED) */

/* I2C Slave functions prototypes */
#if (Ultrasound_MODE_SLAVE_ENABLED)
    /* Read and Clear status functions */
    uint8 Ultrasound_SlaveStatus(void)                ;
    uint8 Ultrasound_SlaveClearReadStatus(void)       ;
    uint8 Ultrasound_SlaveClearWriteStatus(void)      ;

    void  Ultrasound_SlaveSetAddress(uint8 address)   ;

    /* Interrupt based operation functions */
    void  Ultrasound_SlaveInitReadBuf(uint8 * rdBuf, uint8 bufSize) \
                                                            ;
    void  Ultrasound_SlaveInitWriteBuf(uint8 * wrBuf, uint8 bufSize) \
                                                            ;
    uint8 Ultrasound_SlaveGetReadBufSize(void)        ;
    uint8 Ultrasound_SlaveGetWriteBufSize(void)       ;
    void  Ultrasound_SlaveClearReadBuf(void)          ;
    void  Ultrasound_SlaveClearWriteBuf(void)         ;

    /* Communication bootloader I2C Slave APIs */
    #if defined(CYDEV_BOOTLOADER_IO_COMP) && (Ultrasound_BOOTLOADER_INTERFACE_ENABLED)
        /* Physical layer functions */
        void     Ultrasound_CyBtldrCommStart(void) CYSMALL \
                                                            ;
        void     Ultrasound_CyBtldrCommStop(void)  CYSMALL \
                                                            ;
        void     Ultrasound_CyBtldrCommReset(void) CYSMALL \
                                                            ;
        cystatus Ultrasound_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) \
                                                        CYSMALL ;
        cystatus Ultrasound_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)  CYSMALL \
                                                            ;

        #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Ultrasound)
            #define CyBtldrCommStart    Ultrasound_CyBtldrCommStart
            #define CyBtldrCommStop     Ultrasound_CyBtldrCommStop
            #define CyBtldrCommReset    Ultrasound_CyBtldrCommReset
            #define CyBtldrCommWrite    Ultrasound_CyBtldrCommWrite
            #define CyBtldrCommRead     Ultrasound_CyBtldrCommRead
        #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Ultrasound) */

        /* Size of Read/Write buffers for I2C bootloader  */
        #define Ultrasound_BTLDR_SIZEOF_READ_BUFFER   (0x80u)
        #define Ultrasound_BTLDR_SIZEOF_WRITE_BUFFER  (0x80u)
        #define Ultrasound_MIN_UNT16(a, b)            ( ((uint16)(a) < (b)) ? ((uint16) (a)) : ((uint16) (b)) )
        #define Ultrasound_WAIT_1_MS                  (1u)

    #endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (Ultrasound_BOOTLOADER_INTERFACE_ENABLED) */

#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */

/* Component interrupt handlers */
CY_ISR_PROTO(Ultrasound_ISR);
#if ((Ultrasound_FF_IMPLEMENTED) || (Ultrasound_WAKEUP_ENABLED))
    CY_ISR_PROTO(Ultrasound_WAKEUP_ISR);
#endif /* ((Ultrasound_FF_IMPLEMENTED) || (Ultrasound_WAKEUP_ENABLED)) */


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 Ultrasound_initVar;


/***************************************
*   Initial Parameter Constants
***************************************/

#define Ultrasound_DATA_RATE          (50u)
#define Ultrasound_DEFAULT_ADDR       (8u)
#define Ultrasound_I2C_PAIR_SELECTED  (0u)

/* I2C pair enum */
#define Ultrasound_I2C_PAIR_ANY   (0x01u) /* Any pins for I2C */
#define Ultrasound_I2C_PAIR0      (0x01u) /* I2C0: (SCL = P12[4]) && (SCL = P12[5]) */
#define Ultrasound_I2C_PAIR1      (0x02u) /* I2C1: (SCL = P12[0]) && (SDA = P12[1]) */

#define Ultrasound_I2C1_SIO_PAIR  (Ultrasound_I2C_PAIR1 == Ultrasound_I2C_PAIR_SELECTED)
#define Ultrasound_I2C0_SIO_PAIR  (Ultrasound_I2C_PAIR0 == Ultrasound_I2C_PAIR_SELECTED)


/***************************************
*            API Constants
***************************************/

/* Master/Slave control constants */
#define Ultrasound_READ_XFER_MODE     (0x01u) /* Read */
#define Ultrasound_WRITE_XFER_MODE    (0x00u) /* Write */
#define Ultrasound_ACK_DATA           (0x01u) /* Send ACK */
#define Ultrasound_NAK_DATA           (0x00u) /* Send NAK */
#define Ultrasound_OVERFLOW_RETURN    (0xFFu) /* Send on bus in case of overflow */

#if (Ultrasound_MODE_MASTER_ENABLED)
    /* "Mode" constants for MasterWriteBuf() or MasterReadBuf() function */
    #define Ultrasound_MODE_COMPLETE_XFER     (0x00u) /* Full transfer with Start and Stop */
    #define Ultrasound_MODE_REPEAT_START      (0x01u) /* Begin with a ReStart instead of a Start */
    #define Ultrasound_MODE_NO_STOP           (0x02u) /* Complete the transfer without a Stop */

    /* Master status */
    #define Ultrasound_MSTAT_CLEAR            (0x00u) /* Clear (initialize) status value */

    #define Ultrasound_MSTAT_RD_CMPLT         (0x01u) /* Read complete */
    #define Ultrasound_MSTAT_WR_CMPLT         (0x02u) /* Write complete */
    #define Ultrasound_MSTAT_XFER_INP         (0x04u) /* Master transfer in progress */
    #define Ultrasound_MSTAT_XFER_HALT        (0x08u) /* Transfer is halted */

    #define Ultrasound_MSTAT_ERR_MASK         (0xF0u) /* Mask for all errors */
    #define Ultrasound_MSTAT_ERR_SHORT_XFER   (0x10u) /* Master NAKed before end of packet */
    #define Ultrasound_MSTAT_ERR_ADDR_NAK     (0x20u) /* Slave did not ACK */
    #define Ultrasound_MSTAT_ERR_ARB_LOST     (0x40u) /* Master lost arbitration during communication */
    #define Ultrasound_MSTAT_ERR_XFER         (0x80u) /* Error during transfer */

    /* Master API returns */
    #define Ultrasound_MSTR_NO_ERROR          (0x00u) /* Function complete without error */
    #define Ultrasound_MSTR_BUS_BUSY          (0x01u) /* Bus is busy, process not started */
    #define Ultrasound_MSTR_NOT_READY         (0x02u) /* Master not Master on the bus or */
                                                            /*  Slave operation in progress */
    #define Ultrasound_MSTR_ERR_LB_NAK        (0x03u) /* Last Byte Naked */
    #define Ultrasound_MSTR_ERR_ARB_LOST      (0x04u) /* Master lost arbitration during communication */
    #define Ultrasound_MSTR_ERR_ABORT_START_GEN  (0x05u) /* Master did not generate Start, the Slave */
                                                               /* was addressed before */

#endif /* (Ultrasound_MODE_MASTER_ENABLED) */

#if (Ultrasound_MODE_SLAVE_ENABLED)
    /* Slave Status Constants */
    #define Ultrasound_SSTAT_RD_CMPLT     (0x01u) /* Read transfer complete */
    #define Ultrasound_SSTAT_RD_BUSY      (0x02u) /* Read transfer in progress */
    #define Ultrasound_SSTAT_RD_ERR_OVFL  (0x04u) /* Read overflow Error */
    #define Ultrasound_SSTAT_RD_MASK      (0x0Fu) /* Read Status Mask */
    #define Ultrasound_SSTAT_RD_NO_ERR    (0x00u) /* Read no Error */

    #define Ultrasound_SSTAT_WR_CMPLT     (0x10u) /* Write transfer complete */
    #define Ultrasound_SSTAT_WR_BUSY      (0x20u) /* Write transfer in progress */
    #define Ultrasound_SSTAT_WR_ERR_OVFL  (0x40u) /* Write overflow Error */
    #define Ultrasound_SSTAT_WR_MASK      (0xF0u) /* Write Status Mask  */
    #define Ultrasound_SSTAT_WR_NO_ERR    (0x00u) /* Write no Error */

    #define Ultrasound_SSTAT_RD_CLEAR     (0x0Du) /* Read Status clear */
    #define Ultrasound_SSTAT_WR_CLEAR     (0xD0u) /* Write Status Clear */

#endif /* (Ultrasound_MODE_SLAVE_ENABLED) */


/***************************************
*       I2C state machine constants
***************************************/

/* Default slave address states */
#define  Ultrasound_SM_IDLE           (0x10u) /* Default state - IDLE */
#define  Ultrasound_SM_EXIT_IDLE      (0x00u) /* Pass master and slave processing and go to IDLE */

/* Slave mode states */
#define  Ultrasound_SM_SLAVE          (Ultrasound_SM_IDLE) /* Any Slave state */
#define  Ultrasound_SM_SL_WR_DATA     (0x11u) /* Master writes data to slave  */
#define  Ultrasound_SM_SL_RD_DATA     (0x12u) /* Master reads data from slave */

/* Master mode states */
#define  Ultrasound_SM_MASTER         (0x40u) /* Any master state */

#define  Ultrasound_SM_MSTR_RD        (0x08u) /* Any master read state          */
#define  Ultrasound_SM_MSTR_RD_ADDR   (0x49u) /* Master sends address with read */
#define  Ultrasound_SM_MSTR_RD_DATA   (0x4Au) /* Master reads data              */

#define  Ultrasound_SM_MSTR_WR        (0x04u) /* Any master read state           */
#define  Ultrasound_SM_MSTR_WR_ADDR   (0x45u) /* Master sends address with write */
#define  Ultrasound_SM_MSTR_WR_DATA   (0x46u) /* Master writes data              */

#define  Ultrasound_SM_MSTR_HALT      (0x60u) /* Master waits for ReStart */

#define Ultrasound_CHECK_SM_MASTER    (0u != (Ultrasound_SM_MASTER & Ultrasound_state))
#define Ultrasound_CHECK_SM_SLAVE     (0u != (Ultrasound_SM_SLAVE  & Ultrasound_state))


/***************************************
*              Registers
***************************************/

#if (Ultrasound_FF_IMPLEMENTED)
    /* Fixed Function registers */
    #define Ultrasound_XCFG_REG           (*(reg8 *) Ultrasound_I2C_FF__XCFG)
    #define Ultrasound_XCFG_PTR           ( (reg8 *) Ultrasound_I2C_FF__XCFG)

    #define Ultrasound_ADDR_REG           (*(reg8 *) Ultrasound_I2C_FF__ADR)
    #define Ultrasound_ADDR_PTR           ( (reg8 *) Ultrasound_I2C_FF__ADR)

    #define Ultrasound_CFG_REG            (*(reg8 *) Ultrasound_I2C_FF__CFG)
    #define Ultrasound_CFG_PTR            ( (reg8 *) Ultrasound_I2C_FF__CFG)

    #define Ultrasound_CSR_REG            (*(reg8 *) Ultrasound_I2C_FF__CSR)
    #define Ultrasound_CSR_PTR            ( (reg8 *) Ultrasound_I2C_FF__CSR)

    #define Ultrasound_DATA_REG           (*(reg8 *) Ultrasound_I2C_FF__D)
    #define Ultrasound_DATA_PTR           ( (reg8 *) Ultrasound_I2C_FF__D)

    #define Ultrasound_MCSR_REG           (*(reg8 *) Ultrasound_I2C_FF__MCSR)
    #define Ultrasound_MCSR_PTR           ( (reg8 *) Ultrasound_I2C_FF__MCSR)

    #define Ultrasound_ACT_PWRMGR_REG     (*(reg8 *) Ultrasound_I2C_FF__PM_ACT_CFG)
    #define Ultrasound_ACT_PWRMGR_PTR     ( (reg8 *) Ultrasound_I2C_FF__PM_ACT_CFG)
    #define Ultrasound_ACT_PWR_EN         ( (uint8)  Ultrasound_I2C_FF__PM_ACT_MSK)

    #define Ultrasound_STBY_PWRMGR_REG    (*(reg8 *) Ultrasound_I2C_FF__PM_STBY_CFG)
    #define Ultrasound_STBY_PWRMGR_PTR    ( (reg8 *) Ultrasound_I2C_FF__PM_STBY_CFG)
    #define Ultrasound_STBY_PWR_EN        ( (uint8)  Ultrasound_I2C_FF__PM_STBY_MSK)

    #define Ultrasound_PWRSYS_CR1_REG     (*(reg8 *) CYREG_PWRSYS_CR1)
    #define Ultrasound_PWRSYS_CR1_PTR     ( (reg8 *) CYREG_PWRSYS_CR1)

    #define Ultrasound_CLKDIV1_REG    (*(reg8 *) Ultrasound_I2C_FF__CLK_DIV1)
    #define Ultrasound_CLKDIV1_PTR    ( (reg8 *) Ultrasound_I2C_FF__CLK_DIV1)

    #define Ultrasound_CLKDIV2_REG    (*(reg8 *) Ultrasound_I2C_FF__CLK_DIV2)
    #define Ultrasound_CLKDIV2_PTR    ( (reg8 *) Ultrasound_I2C_FF__CLK_DIV2)

#else
    /* UDB implementation registers */
    #define Ultrasound_CFG_REG \
            (*(reg8 *) Ultrasound_bI2C_UDB_SyncCtl_CtrlReg__CONTROL_REG)
    #define Ultrasound_CFG_PTR \
            ( (reg8 *) Ultrasound_bI2C_UDB_SyncCtl_CtrlReg__CONTROL_REG)

    #define Ultrasound_CSR_REG        (*(reg8 *) Ultrasound_bI2C_UDB_StsReg__STATUS_REG)
    #define Ultrasound_CSR_PTR        ( (reg8 *) Ultrasound_bI2C_UDB_StsReg__STATUS_REG)

    #define Ultrasound_INT_MASK_REG   (*(reg8 *) Ultrasound_bI2C_UDB_StsReg__MASK_REG)
    #define Ultrasound_INT_MASK_PTR   ( (reg8 *) Ultrasound_bI2C_UDB_StsReg__MASK_REG)

    #define Ultrasound_INT_ENABLE_REG (*(reg8 *) Ultrasound_bI2C_UDB_StsReg__STATUS_AUX_CTL_REG)
    #define Ultrasound_INT_ENABLE_PTR ( (reg8 *) Ultrasound_bI2C_UDB_StsReg__STATUS_AUX_CTL_REG)

    #define Ultrasound_DATA_REG       (*(reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__A0_REG)
    #define Ultrasound_DATA_PTR       ( (reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__A0_REG)

    #define Ultrasound_GO_REG         (*(reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__F1_REG)
    #define Ultrasound_GO_PTR         ( (reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__F1_REG)

    #define Ultrasound_GO_DONE_REG    (*(reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__A1_REG)
    #define Ultrasound_GO_DONE_PTR    ( (reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__A1_REG)

    #define Ultrasound_MCLK_PRD_REG   (*(reg8 *) Ultrasound_bI2C_UDB_Master_ClkGen_u0__D0_REG)
    #define Ultrasound_MCLK_PRD_PTR   ( (reg8 *) Ultrasound_bI2C_UDB_Master_ClkGen_u0__D0_REG)

    #define Ultrasound_MCLK_CMP_REG   (*(reg8 *) Ultrasound_bI2C_UDB_Master_ClkGen_u0__D1_REG)
    #define Ultrasound_MCLK_CMP_PTR   ( (reg8 *) Ultrasound_bI2C_UDB_Master_ClkGen_u0__D1_REG)

    #if (Ultrasound_MODE_SLAVE_ENABLED)
        #define Ultrasound_ADDR_REG       (*(reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__D0_REG)
        #define Ultrasound_ADDR_PTR       ( (reg8 *) Ultrasound_bI2C_UDB_Shifter_u0__D0_REG)

        #define Ultrasound_PERIOD_REG     (*(reg8 *) Ultrasound_bI2C_UDB_Slave_BitCounter__PERIOD_REG)
        #define Ultrasound_PERIOD_PTR     ( (reg8 *) Ultrasound_bI2C_UDB_Slave_BitCounter__PERIOD_REG)

        #define Ultrasound_COUNTER_REG    (*(reg8 *) Ultrasound_bI2C_UDB_Slave_BitCounter__COUNT_REG)
        #define Ultrasound_COUNTER_PTR    ( (reg8 *) Ultrasound_bI2C_UDB_Slave_BitCounter__COUNT_REG)

        #define Ultrasound_COUNTER_AUX_CTL_REG \
                                    (*(reg8 *) Ultrasound_bI2C_UDB_Slave_BitCounter__CONTROL_AUX_CTL_REG)
        #define Ultrasound_COUNTER_AUX_CTL_PTR \
                                    ( (reg8 *) Ultrasound_bI2C_UDB_Slave_BitCounter__CONTROL_AUX_CTL_REG)

    #endif /* (Ultrasound_MODE_SLAVE_ENABLED) */

#endif /* (Ultrasound_FF_IMPLEMENTED) */


/***************************************
*        Registers Constants
***************************************/

/* Ultrasound_I2C_IRQ */
#define Ultrasound_ISR_NUMBER     ((uint8) Ultrasound_I2C_IRQ__INTC_NUMBER)
#define Ultrasound_ISR_PRIORITY   ((uint8) Ultrasound_I2C_IRQ__INTC_PRIOR_NUM)

/* I2C Slave Data Register */
#define Ultrasound_SLAVE_ADDR_MASK    (0x7Fu)
#define Ultrasound_SLAVE_ADDR_SHIFT   (0x01u)
#define Ultrasound_DATA_MASK          (0xFFu)
#define Ultrasound_READ_FLAG          (0x01u)

/* Block reset constants */
#define Ultrasound_CLEAR_REG          (0x00u)
#define Ultrasound_BLOCK_RESET_DELAY  (2u)
#define Ultrasound_FF_RESET_DELAY     (Ultrasound_BLOCK_RESET_DELAY)
#define Ultrasound_RESTORE_TIMEOUT    (255u)

#if (Ultrasound_FF_IMPLEMENTED)
    /* XCFG I2C Extended Configuration Register */
    #define Ultrasound_XCFG_CLK_EN        (0x80u) /* Enable gated clock to block */
    #define Ultrasound_XCFG_I2C_ON        (0x40u) /* Enable I2C as wake up source*/
    #define Ultrasound_XCFG_RDY_TO_SLEEP  (0x20u) /* I2C ready go to sleep */
    #define Ultrasound_XCFG_FORCE_NACK    (0x10u) /* Force NACK all incoming transactions */
    #define Ultrasound_XCFG_NO_BC_INT     (0x08u) /* No interrupt on byte complete */
    #define Ultrasound_XCFG_BUF_MODE      (0x02u) /* Enable buffer mode */
    #define Ultrasound_XCFG_HDWR_ADDR_EN  (0x01u) /* Enable Hardware address match */

    /* CFG I2C Configuration Register */
    #define Ultrasound_CFG_SIO_SELECT     (0x80u) /* Pin Select for SCL/SDA lines */
    #define Ultrasound_CFG_PSELECT        (0x40u) /* Pin Select */
    #define Ultrasound_CFG_BUS_ERR_IE     (0x20u) /* Bus Error Interrupt Enable */
    #define Ultrasound_CFG_STOP_IE        (0x10u) /* Enable Interrupt on STOP condition */
    #define Ultrasound_CFG_CLK_RATE_MSK   (0x0Cu) /* Clock rate select */
    #define Ultrasound_CFG_CLK_RATE_100   (0x00u) /* Clock rate select 100K */
    #define Ultrasound_CFG_CLK_RATE_400   (0x04u) /* Clock rate select 400K */
    #define Ultrasound_CFG_CLK_RATE_050   (0x08u) /* Clock rate select 50K  */
    #define Ultrasound_CFG_CLK_RATE_RSVD  (0x0Cu) /* Clock rate select Invalid */
    #define Ultrasound_CFG_EN_MSTR        (0x02u) /* Enable Master operation */
    #define Ultrasound_CFG_EN_SLAVE       (0x01u) /* Enable Slave operation */

    #define Ultrasound_CFG_CLK_RATE_LESS_EQUAL_50 (0x04u) /* Clock rate select <= 50kHz */
    #define Ultrasound_CFG_CLK_RATE_GRATER_50     (0x00u) /* Clock rate select > 50kHz */

    /* CSR I2C Control and Status Register */
    #define Ultrasound_CSR_BUS_ERROR      (0x80u) /* Active high when bus error has occurred */
    #define Ultrasound_CSR_LOST_ARB       (0x40u) /* Set to 1 if lost arbitration in host mode */
    #define Ultrasound_CSR_STOP_STATUS    (0x20u) /* Set if Stop has been detected */
    #define Ultrasound_CSR_ACK            (0x10u) /* ACK response */
    #define Ultrasound_CSR_NAK            (0x00u) /* NAK response */
    #define Ultrasound_CSR_ADDRESS        (0x08u) /* Set in firmware 0 = status bit, 1 Address is slave */
    #define Ultrasound_CSR_TRANSMIT       (0x04u) /* Set in firmware 1 = transmit, 0 = receive */
    #define Ultrasound_CSR_LRB            (0x02u) /* Last received bit */
    #define Ultrasound_CSR_LRB_ACK        (0x00u) /* Last received bit was an ACK */
    #define Ultrasound_CSR_LRB_NAK        (0x02u) /* Last received bit was an NAK */
    #define Ultrasound_CSR_BYTE_COMPLETE  (0x01u) /* Informs that last byte has been sent */
    #define Ultrasound_CSR_STOP_GEN       (0x00u) /* Generate a stop condition */
    #define Ultrasound_CSR_RDY_TO_RD      (0x00u) /* Set to receive mode */

    /* MCSR I2C Master Control and Status Register */
    #define Ultrasound_MCSR_STOP_GEN      (0x10u) /* Firmware sets this bit to initiate a Stop condition */
    #define Ultrasound_MCSR_BUS_BUSY      (0x08u) /* Status bit, Set at Start and cleared at Stop condition */
    #define Ultrasound_MCSR_MSTR_MODE     (0x04u) /* Status bit, Set at Start and cleared at Stop condition */
    #define Ultrasound_MCSR_RESTART_GEN   (0x02u) /* Firmware sets this bit to initiate a ReStart condition */
    #define Ultrasound_MCSR_START_GEN     (0x01u) /* Firmware sets this bit to initiate a Start condition */

    /* PWRSYS_CR1 to handle Sleep */
    #define Ultrasound_PWRSYS_CR1_I2C_REG_BACKUP  (0x04u) /* Enables, power to I2C regs while sleep */

#else
    /* CONTROL REG bits location */
    #define Ultrasound_CTRL_START_SHIFT           (7u)
    #define Ultrasound_CTRL_STOP_SHIFT            (6u)
    #define Ultrasound_CTRL_RESTART_SHIFT         (5u)
    #define Ultrasound_CTRL_NACK_SHIFT            (4u)
    #define Ultrasound_CTRL_ANY_ADDRESS_SHIFT     (3u)
    #define Ultrasound_CTRL_TRANSMIT_SHIFT        (2u)
    #define Ultrasound_CTRL_ENABLE_MASTER_SHIFT   (1u)
    #define Ultrasound_CTRL_ENABLE_SLAVE_SHIFT    (0u)
    #define Ultrasound_CTRL_START_MASK            ((uint8) (0x01u << Ultrasound_CTRL_START_SHIFT))
    #define Ultrasound_CTRL_STOP_MASK             ((uint8) (0x01u << Ultrasound_CTRL_STOP_SHIFT))
    #define Ultrasound_CTRL_RESTART_MASK          ((uint8) (0x01u << Ultrasound_CTRL_RESTART_SHIFT))
    #define Ultrasound_CTRL_NACK_MASK             ((uint8) (0x01u << Ultrasound_CTRL_NACK_SHIFT))
    #define Ultrasound_CTRL_ANY_ADDRESS_MASK      ((uint8) (0x01u << Ultrasound_CTRL_ANY_ADDRESS_SHIFT))
    #define Ultrasound_CTRL_TRANSMIT_MASK         ((uint8) (0x01u << Ultrasound_CTRL_TRANSMIT_SHIFT))
    #define Ultrasound_CTRL_ENABLE_MASTER_MASK    ((uint8) (0x01u << Ultrasound_CTRL_ENABLE_MASTER_SHIFT))
    #define Ultrasound_CTRL_ENABLE_SLAVE_MASK     ((uint8) (0x01u << Ultrasound_CTRL_ENABLE_SLAVE_SHIFT))

    /* STATUS REG bits location */
    #define Ultrasound_STS_LOST_ARB_SHIFT         (6u)
    #define Ultrasound_STS_STOP_SHIFT             (5u)
    #define Ultrasound_STS_BUSY_SHIFT             (4u)
    #define Ultrasound_STS_ADDR_SHIFT             (3u)
    #define Ultrasound_STS_MASTER_MODE_SHIFT      (2u)
    #define Ultrasound_STS_LRB_SHIFT              (1u)
    #define Ultrasound_STS_BYTE_COMPLETE_SHIFT    (0u)
    #define Ultrasound_STS_LOST_ARB_MASK          ((uint8) (0x01u << Ultrasound_STS_LOST_ARB_SHIFT))
    #define Ultrasound_STS_STOP_MASK              ((uint8) (0x01u << Ultrasound_STS_STOP_SHIFT))
    #define Ultrasound_STS_BUSY_MASK              ((uint8) (0x01u << Ultrasound_STS_BUSY_SHIFT))
    #define Ultrasound_STS_ADDR_MASK              ((uint8) (0x01u << Ultrasound_STS_ADDR_SHIFT))
    #define Ultrasound_STS_MASTER_MODE_MASK       ((uint8) (0x01u << Ultrasound_STS_MASTER_MODE_SHIFT))
    #define Ultrasound_STS_LRB_MASK               ((uint8) (0x01u << Ultrasound_STS_LRB_SHIFT))
    #define Ultrasound_STS_BYTE_COMPLETE_MASK     ((uint8) (0x01u << Ultrasound_STS_BYTE_COMPLETE_SHIFT))

    /* AUX_CTL bits definition */
    #define Ultrasound_COUNTER_ENABLE_MASK        (0x20u) /* Enable 7-bit counter */
    #define Ultrasound_INT_ENABLE_MASK            (0x10u) /* Enable interrupt from status register */
    #define Ultrasound_CNT7_ENABLE                (Ultrasound_COUNTER_ENABLE_MASK)
    #define Ultrasound_INTR_ENABLE                (Ultrasound_INT_ENABLE_MASK)

#endif /* (Ultrasound_FF_IMPLEMENTED) */


/***************************************
*        Marco
***************************************/

/* ACK and NACK for data and address checks */
#define Ultrasound_CHECK_ADDR_ACK(csr)    ((Ultrasound_CSR_LRB_ACK | Ultrasound_CSR_ADDRESS) == \
                                                 ((Ultrasound_CSR_LRB    | Ultrasound_CSR_ADDRESS) &  \
                                                  (csr)))


#define Ultrasound_CHECK_ADDR_NAK(csr)    ((Ultrasound_CSR_LRB_NAK | Ultrasound_CSR_ADDRESS) == \
                                                 ((Ultrasound_CSR_LRB    | Ultrasound_CSR_ADDRESS) &  \
                                                  (csr)))

#define Ultrasound_CHECK_DATA_ACK(csr)    (0u == ((csr) & Ultrasound_CSR_LRB_NAK))

/* MCSR conditions check */
#define Ultrasound_CHECK_BUS_FREE(mcsr)       (0u == ((mcsr) & Ultrasound_MCSR_BUS_BUSY))
#define Ultrasound_CHECK_MASTER_MODE(mcsr)    (0u != ((mcsr) & Ultrasound_MCSR_MSTR_MODE))

/* CSR conditions check */
#define Ultrasound_WAIT_BYTE_COMPLETE(csr)    (0u == ((csr) & Ultrasound_CSR_BYTE_COMPLETE))
#define Ultrasound_WAIT_STOP_COMPLETE(csr)    (0u == ((csr) & (Ultrasound_CSR_BYTE_COMPLETE | \
                                                                     Ultrasound_CSR_STOP_STATUS)))
#define Ultrasound_CHECK_BYTE_COMPLETE(csr)   (0u != ((csr) & Ultrasound_CSR_BYTE_COMPLETE))
#define Ultrasound_CHECK_STOP_STS(csr)        (0u != ((csr) & Ultrasound_CSR_STOP_STATUS))
#define Ultrasound_CHECK_LOST_ARB(csr)        (0u != ((csr) & Ultrasound_CSR_LOST_ARB))
#define Ultrasound_CHECK_ADDRESS_STS(csr)     (0u != ((csr) & Ultrasound_CSR_ADDRESS))

/* Software start and end of transaction check */
#define Ultrasound_CHECK_RESTART(mstrCtrl)    (0u != ((mstrCtrl) & Ultrasound_MODE_REPEAT_START))
#define Ultrasound_CHECK_NO_STOP(mstrCtrl)    (0u != ((mstrCtrl) & Ultrasound_MODE_NO_STOP))

/* Send read or write completion depends on state */
#define Ultrasound_GET_MSTAT_CMPLT ((0u != (Ultrasound_state & Ultrasound_SM_MSTR_RD)) ? \
                                                 (Ultrasound_MSTAT_RD_CMPLT) : (Ultrasound_MSTAT_WR_CMPLT))

/* Returns 7-bit slave address */
#define Ultrasound_GET_SLAVE_ADDR(dataReg)   (((dataReg) >> Ultrasound_SLAVE_ADDR_SHIFT) & \
                                                                  Ultrasound_SLAVE_ADDR_MASK)

#if (Ultrasound_FF_IMPLEMENTED)
    /* Check enable of module */
    #define Ultrasound_I2C_ENABLE_REG     (Ultrasound_ACT_PWRMGR_REG)
    #define Ultrasound_IS_I2C_ENABLE(reg) (0u != ((reg) & Ultrasound_ACT_PWR_EN))
    #define Ultrasound_IS_ENABLED         (0u != (Ultrasound_ACT_PWRMGR_REG & Ultrasound_ACT_PWR_EN))

    #define Ultrasound_CHECK_PWRSYS_I2C_BACKUP    (0u != (Ultrasound_PWRSYS_CR1_I2C_REG_BACKUP & \
                                                                Ultrasound_PWRSYS_CR1_REG))

    /* Check start condition generation */
    #define Ultrasound_CHECK_START_GEN(mcsr)  ((0u != ((mcsr) & Ultrasound_MCSR_START_GEN)) && \
                                                     (0u == ((mcsr) & Ultrasound_MCSR_MSTR_MODE)))

    #define Ultrasound_CLEAR_START_GEN        do{ \
                                                        Ultrasound_MCSR_REG &=                                   \
                                                                           ((uint8) ~Ultrasound_MCSR_START_GEN); \
                                                    }while(0)

    /* Stop interrupt */
    #define Ultrasound_ENABLE_INT_ON_STOP     do{ \
                                                        Ultrasound_CFG_REG |= Ultrasound_CFG_STOP_IE; \
                                                    }while(0)

    #define Ultrasound_DISABLE_INT_ON_STOP    do{ \
                                                        Ultrasound_CFG_REG &=                                 \
                                                                           ((uint8) ~Ultrasound_CFG_STOP_IE); \
                                                    }while(0)

    /* Transmit data */
    #define Ultrasound_TRANSMIT_DATA          do{ \
                                                        Ultrasound_CSR_REG = Ultrasound_CSR_TRANSMIT; \
                                                    }while(0)

    #define Ultrasound_ACK_AND_TRANSMIT       do{ \
                                                        Ultrasound_CSR_REG = (Ultrasound_CSR_ACK |      \
                                                                                    Ultrasound_CSR_TRANSMIT); \
                                                    }while(0)

    #define Ultrasound_NAK_AND_TRANSMIT       do{ \
                                                        Ultrasound_CSR_REG = Ultrasound_CSR_NAK; \
                                                    }while(0)

    /* Special case: udb needs to ack, ff needs to nak */
    #define Ultrasound_ACKNAK_AND_TRANSMIT    do{ \
                                                        Ultrasound_CSR_REG  = (Ultrasound_CSR_NAK |      \
                                                                                     Ultrasound_CSR_TRANSMIT); \
                                                    }while(0)
    /* Receive data */
    #define Ultrasound_ACK_AND_RECEIVE        do{ \
                                                        Ultrasound_CSR_REG = Ultrasound_CSR_ACK; \
                                                    }while(0)

    #define Ultrasound_NAK_AND_RECEIVE        do{ \
                                                        Ultrasound_CSR_REG = Ultrasound_CSR_NAK; \
                                                    }while(0)

    #define Ultrasound_READY_TO_READ          do{ \
                                                        Ultrasound_CSR_REG = Ultrasound_CSR_RDY_TO_RD; \
                                                    }while(0)

    /* Release bus after lost arbitration */
    #define Ultrasound_BUS_RELEASE    Ultrasound_READY_TO_READ

    /* Master Start/ReStart/Stop conditions generation */
    #define Ultrasound_GENERATE_START         do{ \
                                                        Ultrasound_MCSR_REG = Ultrasound_MCSR_START_GEN; \
                                                    }while(0)

    #define Ultrasound_GENERATE_RESTART \
                    do{                       \
                        Ultrasound_MCSR_REG = (Ultrasound_MCSR_RESTART_GEN | \
                                                     Ultrasound_MCSR_STOP_GEN);    \
                        Ultrasound_CSR_REG  = Ultrasound_CSR_TRANSMIT;       \
                    }while(0)

    #define Ultrasound_GENERATE_STOP \
                    do{                    \
                        Ultrasound_MCSR_REG = Ultrasound_MCSR_STOP_GEN; \
                        Ultrasound_CSR_REG  = Ultrasound_CSR_TRANSMIT;  \
                    }while(0)

    /* Master manual APIs compatible defines */
    #define Ultrasound_GENERATE_START_MANUAL      Ultrasound_GENERATE_START
    #define Ultrasound_GENERATE_RESTART_MANUAL    Ultrasound_GENERATE_RESTART
    #define Ultrasound_GENERATE_STOP_MANUAL       Ultrasound_GENERATE_STOP
    #define Ultrasound_TRANSMIT_DATA_MANUAL       Ultrasound_TRANSMIT_DATA
    #define Ultrasound_READY_TO_READ_MANUAL       Ultrasound_READY_TO_READ
    #define Ultrasound_ACK_AND_RECEIVE_MANUAL     Ultrasound_ACK_AND_RECEIVE
    #define Ultrasound_BUS_RELEASE_MANUAL         Ultrasound_BUS_RELEASE

#else

    /* Masks to enable interrupts from Status register */
    #define Ultrasound_STOP_IE_MASK           (Ultrasound_STS_STOP_MASK)
    #define Ultrasound_BYTE_COMPLETE_IE_MASK  (Ultrasound_STS_BYTE_COMPLETE_MASK)

    /* FF compatibility: CSR register bit-fields */
    #define Ultrasound_CSR_LOST_ARB       (Ultrasound_STS_LOST_ARB_MASK)
    #define Ultrasound_CSR_STOP_STATUS    (Ultrasound_STS_STOP_MASK)
    #define Ultrasound_CSR_BUS_ERROR      (0x00u)
    #define Ultrasound_CSR_ADDRESS        (Ultrasound_STS_ADDR_MASK)
    #define Ultrasound_CSR_TRANSMIT       (Ultrasound_CTRL_TRANSMIT_MASK)
    #define Ultrasound_CSR_LRB            (Ultrasound_STS_LRB_MASK)
    #define Ultrasound_CSR_LRB_NAK        (Ultrasound_STS_LRB_MASK)
    #define Ultrasound_CSR_LRB_ACK        (0x00u)
    #define Ultrasound_CSR_BYTE_COMPLETE  (Ultrasound_STS_BYTE_COMPLETE_MASK)

    /* FF compatibility: MCSR registers bit-fields */
    #define Ultrasound_MCSR_REG           (Ultrasound_CSR_REG)  /* UDB incorporates master and slave regs */
    #define Ultrasound_MCSR_BUS_BUSY      (Ultrasound_STS_BUSY_MASK)      /* Is bus is busy               */
    #define Ultrasound_MCSR_START_GEN     (Ultrasound_CTRL_START_MASK)    /* Generate Start condition     */
    #define Ultrasound_MCSR_RESTART_GEN   (Ultrasound_CTRL_RESTART_MASK)  /* Generates RESTART condition  */
    #define Ultrasound_MCSR_MSTR_MODE     (Ultrasound_STS_MASTER_MODE_MASK)/* Define if active Master     */

    /* Data to write into TX FIFO to release FSM */
    #define Ultrasound_PREPARE_TO_RELEASE (0xFFu)
    #define Ultrasound_RELEASE_FSM        (0x00u)

    /* Define release command done: history of byte complete status is cleared */
    #define Ultrasound_WAIT_RELEASE_CMD_DONE  (Ultrasound_RELEASE_FSM != Ultrasound_GO_DONE_REG)

    /* Check enable of module */
    #define Ultrasound_I2C_ENABLE_REG     (Ultrasound_CFG_REG)
    #define Ultrasound_IS_I2C_ENABLE(reg) ((0u != ((reg) & Ultrasound_ENABLE_MASTER)) || \
                                                 (0u != ((reg) & Ultrasound_ENABLE_SLAVE)))

    #define Ultrasound_IS_ENABLED         (0u != (Ultrasound_CFG_REG & Ultrasound_ENABLE_MS))

    /* Check start condition generation */
    #define Ultrasound_CHECK_START_GEN(mcsr)  ((0u != (Ultrasound_CFG_REG &        \
                                                             Ultrasound_MCSR_START_GEN)) \
                                                    &&                                         \
                                                    (0u == ((mcsr) & Ultrasound_MCSR_MSTR_MODE)))

    #define Ultrasound_CLEAR_START_GEN        do{ \
                                                        Ultrasound_CFG_REG &=                 \
                                                        ((uint8) ~Ultrasound_MCSR_START_GEN); \
                                                    }while(0)

    /* Stop interrupt */
    #define Ultrasound_ENABLE_INT_ON_STOP     do{ \
                                                       Ultrasound_INT_MASK_REG |= Ultrasound_STOP_IE_MASK; \
                                                    }while(0)

    #define Ultrasound_DISABLE_INT_ON_STOP    do{ \
                                                        Ultrasound_INT_MASK_REG &=                               \
                                                                             ((uint8) ~Ultrasound_STOP_IE_MASK); \
                                                    }while(0)

    /* Transmit data */
    #define Ultrasound_TRANSMIT_DATA \
                                    do{    \
                                        Ultrasound_CFG_REG     = (Ultrasound_CTRL_TRANSMIT_MASK | \
                                                                       Ultrasound_CTRL_DEFAULT);        \
                                        Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE;   \
                                        Ultrasound_GO_REG      = Ultrasound_RELEASE_FSM;          \
                                    }while(0)

    #define Ultrasound_ACK_AND_TRANSMIT   Ultrasound_TRANSMIT_DATA

    #define Ultrasound_NAK_AND_TRANSMIT \
                                        do{   \
                                            Ultrasound_CFG_REG     = (Ultrasound_CTRL_NACK_MASK     | \
                                                                            Ultrasound_CTRL_TRANSMIT_MASK | \
                                                                            Ultrasound_CTRL_DEFAULT);       \
                                            Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE;   \
                                            Ultrasound_GO_REG      = Ultrasound_RELEASE_FSM;          \
                                        }while(0)

    /* Receive data */
    #define Ultrasound_READY_TO_READ  \
                                        do{ \
                                            Ultrasound_CFG_REG     = Ultrasound_CTRL_DEFAULT;       \
                                            Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE; \
                                            Ultrasound_GO_REG      = Ultrasound_RELEASE_FSM;       \
                                        }while(0)

    #define Ultrasound_ACK_AND_RECEIVE    Ultrasound_READY_TO_READ

    /* Release bus after arbitration is lost */
    #define Ultrasound_BUS_RELEASE    Ultrasound_READY_TO_READ

    #define Ultrasound_NAK_AND_RECEIVE \
                                        do{  \
                                            Ultrasound_CFG_REG     = (Ultrasound_CTRL_NACK_MASK |   \
                                                                            Ultrasound_CTRL_DEFAULT);     \
                                            Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE; \
                                            Ultrasound_GO_REG      = Ultrasound_RELEASE_FSM;       \
                                        }while(0)

    /* Master condition generation */
    #define Ultrasound_GENERATE_START \
                                        do{ \
                                            Ultrasound_CFG_REG     = (Ultrasound_CTRL_START_MASK |  \
                                                                            Ultrasound_CTRL_DEFAULT);     \
                                            Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE; \
                                            Ultrasound_GO_REG      = Ultrasound_RELEASE_FSM;       \
                                        }while(0)

    #define Ultrasound_GENERATE_RESTART \
                                        do{   \
                                            Ultrasound_CFG_REG     = (Ultrasound_CTRL_RESTART_MASK | \
                                                                            Ultrasound_CTRL_NACK_MASK    | \
                                                                            Ultrasound_CTRL_DEFAULT);      \
                                            Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE;  \
                                            Ultrasound_GO_REG  =     Ultrasound_RELEASE_FSM;         \
                                        }while(0)

    #define Ultrasound_GENERATE_STOP  \
                                        do{ \
                                            Ultrasound_CFG_REG    = (Ultrasound_CTRL_NACK_MASK |    \
                                                                           Ultrasound_CTRL_STOP_MASK |    \
                                                                           Ultrasound_CTRL_DEFAULT);      \
                                            Ultrasound_GO_DONE_REG = Ultrasound_PREPARE_TO_RELEASE; \
                                            Ultrasound_GO_REG      = Ultrasound_RELEASE_FSM;        \
                                        }while(0)

    /* Master manual APIs compatible macros */
    /* These macros wait until byte complete history is cleared after command issued */
    #define Ultrasound_GENERATE_START_MANUAL \
                                        do{ \
                                            Ultrasound_GENERATE_START;                  \
                                            /* Wait until byte complete history is cleared */ \
                                            while(Ultrasound_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)
                                        
    #define Ultrasound_GENERATE_RESTART_MANUAL \
                                        do{          \
                                            Ultrasound_GENERATE_RESTART;                \
                                            /* Wait until byte complete history is cleared */ \
                                            while(Ultrasound_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define Ultrasound_GENERATE_STOP_MANUAL \
                                        do{       \
                                            Ultrasound_GENERATE_STOP;                   \
                                            /* Wait until byte complete history is cleared */ \
                                            while(Ultrasound_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define Ultrasound_TRANSMIT_DATA_MANUAL \
                                        do{       \
                                            Ultrasound_TRANSMIT_DATA;                   \
                                            /* Wait until byte complete history is cleared */ \
                                            while(Ultrasound_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define Ultrasound_READY_TO_READ_MANUAL \
                                        do{       \
                                            Ultrasound_READY_TO_READ;                   \
                                            /* Wait until byte complete history is cleared */ \
                                            while(Ultrasound_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define Ultrasound_ACK_AND_RECEIVE_MANUAL \
                                        do{         \
                                            Ultrasound_ACK_AND_RECEIVE;                 \
                                            /* Wait until byte complete history is cleared */ \
                                            while(Ultrasound_WAIT_RELEASE_CMD_DONE)     \
                                            {                                                 \
                                            }                                                 \
                                        }while(0)

    #define Ultrasound_BUS_RELEASE_MANUAL Ultrasound_READY_TO_READ_MANUAL

#endif /* (Ultrasound_FF_IMPLEMENTED) */


/***************************************
*     Default register init constants
***************************************/

#define Ultrasound_DISABLE    (0u)
#define Ultrasound_ENABLE     (1u)

#if (Ultrasound_FF_IMPLEMENTED)
    /* Ultrasound_XCFG_REG: bits definition */
    #define Ultrasound_DEFAULT_XCFG_HW_ADDR_EN ((Ultrasound_HW_ADRR_DECODE) ? \
                                                        (Ultrasound_XCFG_HDWR_ADDR_EN) : (0u))

    #define Ultrasound_DEFAULT_XCFG_I2C_ON    ((Ultrasound_WAKEUP_ENABLED) ? \
                                                        (Ultrasound_XCFG_I2C_ON) : (0u))


    #define Ultrasound_DEFAULT_CFG_SIO_SELECT ((Ultrasound_I2C1_SIO_PAIR) ? \
                                                        (Ultrasound_CFG_SIO_SELECT) : (0u))


    /* Ultrasound_CFG_REG: bits definition */
    #define Ultrasound_DEFAULT_CFG_PSELECT    ((Ultrasound_WAKEUP_ENABLED) ? \
                                                        (Ultrasound_CFG_PSELECT) : (0u))

    #define Ultrasound_DEFAULT_CLK_RATE0  ((Ultrasound_DATA_RATE <= 50u) ?        \
                                                    (Ultrasound_CFG_CLK_RATE_050) :     \
                                                    ((Ultrasound_DATA_RATE <= 100u) ?   \
                                                        (Ultrasound_CFG_CLK_RATE_100) : \
                                                        (Ultrasound_CFG_CLK_RATE_400)))

    #define Ultrasound_DEFAULT_CLK_RATE1  ((Ultrasound_DATA_RATE <= 50u) ?           \
                                                 (Ultrasound_CFG_CLK_RATE_LESS_EQUAL_50) : \
                                                 (Ultrasound_CFG_CLK_RATE_GRATER_50))

    #define Ultrasound_DEFAULT_CLK_RATE   (Ultrasound_DEFAULT_CLK_RATE1)


    #define Ultrasound_ENABLE_MASTER      ((Ultrasound_MODE_MASTER_ENABLED) ? \
                                                 (Ultrasound_CFG_EN_MSTR) : (0u))

    #define Ultrasound_ENABLE_SLAVE       ((Ultrasound_MODE_SLAVE_ENABLED) ? \
                                                 (Ultrasound_CFG_EN_SLAVE) : (0u))

    #define Ultrasound_ENABLE_MS      (Ultrasound_ENABLE_MASTER | Ultrasound_ENABLE_SLAVE)


    /* Ultrasound_DEFAULT_XCFG_REG */
    #define Ultrasound_DEFAULT_XCFG   (Ultrasound_XCFG_CLK_EN         | \
                                             Ultrasound_DEFAULT_XCFG_I2C_ON | \
                                             Ultrasound_DEFAULT_XCFG_HW_ADDR_EN)

    /* Ultrasound_DEFAULT_CFG_REG */
    #define Ultrasound_DEFAULT_CFG    (Ultrasound_DEFAULT_CFG_SIO_SELECT | \
                                             Ultrasound_DEFAULT_CFG_PSELECT    | \
                                             Ultrasound_DEFAULT_CLK_RATE       | \
                                             Ultrasound_ENABLE_MASTER          | \
                                             Ultrasound_ENABLE_SLAVE)

    /*Ultrasound_DEFAULT_DIVIDE_FACTOR_REG */
    #define Ultrasound_DEFAULT_DIVIDE_FACTOR  ((uint16) 15u)

#else
    /* Ultrasound_CFG_REG: bits definition  */
    #define Ultrasound_ENABLE_MASTER  ((Ultrasound_MODE_MASTER_ENABLED) ? \
                                             (Ultrasound_CTRL_ENABLE_MASTER_MASK) : (0u))

    #define Ultrasound_ENABLE_SLAVE   ((Ultrasound_MODE_SLAVE_ENABLED) ? \
                                             (Ultrasound_CTRL_ENABLE_SLAVE_MASK) : (0u))

    #define Ultrasound_ENABLE_MS      (Ultrasound_ENABLE_MASTER | Ultrasound_ENABLE_SLAVE)


    #define Ultrasound_DEFAULT_CTRL_ANY_ADDR   ((Ultrasound_HW_ADRR_DECODE) ? \
                                                      (0u) : (Ultrasound_CTRL_ANY_ADDRESS_MASK))

    /* Ultrasound_DEFAULT_CFG_REG */
    #define Ultrasound_DEFAULT_CFG    (Ultrasound_DEFAULT_CTRL_ANY_ADDR)

    /* All CTRL default bits to be used in macro */
    #define Ultrasound_CTRL_DEFAULT   (Ultrasound_DEFAULT_CTRL_ANY_ADDR | Ultrasound_ENABLE_MS)

    /* Master clock generator: d0 and d1 */
    #define Ultrasound_MCLK_PERIOD_VALUE  (0x0Fu)
    #define Ultrasound_MCLK_COMPARE_VALUE (0x08u)

    /* Slave bit-counter: control period */
    #define Ultrasound_PERIOD_VALUE       (0x07u)

    /* Ultrasound_DEFAULT_INT_MASK */
    #define Ultrasound_DEFAULT_INT_MASK   (Ultrasound_BYTE_COMPLETE_IE_MASK)

    /* Ultrasound_DEFAULT_MCLK_PRD_REG */
    #define Ultrasound_DEFAULT_MCLK_PRD   (Ultrasound_MCLK_PERIOD_VALUE)

    /* Ultrasound_DEFAULT_MCLK_CMP_REG */
    #define Ultrasound_DEFAULT_MCLK_CMP   (Ultrasound_MCLK_COMPARE_VALUE)

    /* Ultrasound_DEFAULT_PERIOD_REG */
    #define Ultrasound_DEFAULT_PERIOD     (Ultrasound_PERIOD_VALUE)

#endif /* (Ultrasound_FF_IMPLEMENTED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

#define Ultrasound_SSTAT_RD_ERR       (0x08u)
#define Ultrasound_SSTAT_WR_ERR       (0x80u)
#define Ultrasound_MSTR_SLAVE_BUSY    (Ultrasound_MSTR_NOT_READY)
#define Ultrasound_MSTAT_ERR_BUF_OVFL (0x80u)
#define Ultrasound_SSTAT_RD_CMPT      (Ultrasound_SSTAT_RD_CMPLT)
#define Ultrasound_SSTAT_WR_CMPT      (Ultrasound_SSTAT_WR_CMPLT)
#define Ultrasound_MODE_MULTI_MASTER_ENABLE    (Ultrasound_MODE_MULTI_MASTER_MASK)
#define Ultrasound_DATA_RATE_50       (50u)
#define Ultrasound_DATA_RATE_100      (100u)
#define Ultrasound_DEV_MASK           (0xF0u)
#define Ultrasound_SM_SL_STOP         (0x14u)
#define Ultrasound_SM_MASTER_IDLE     (0x40u)
#define Ultrasound_HDWR_DECODE        (0x01u)

#endif /* CY_I2C_Ultrasound_H */


/* [] END OF FILE */

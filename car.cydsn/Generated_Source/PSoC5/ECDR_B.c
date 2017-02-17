/*******************************************************************************
* File Name: ECDR_B.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "ECDR_B.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 ECDR_B__PORT == 15 && ((ECDR_B__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: ECDR_B_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void ECDR_B_Write(uint8 value) 
{
    uint8 staticBits = (ECDR_B_DR & (uint8)(~ECDR_B_MASK));
    ECDR_B_DR = staticBits | ((uint8)(value << ECDR_B_SHIFT) & ECDR_B_MASK);
}


/*******************************************************************************
* Function Name: ECDR_B_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  ECDR_B_DM_STRONG     Strong Drive 
*  ECDR_B_DM_OD_HI      Open Drain, Drives High 
*  ECDR_B_DM_OD_LO      Open Drain, Drives Low 
*  ECDR_B_DM_RES_UP     Resistive Pull Up 
*  ECDR_B_DM_RES_DWN    Resistive Pull Down 
*  ECDR_B_DM_RES_UPDWN  Resistive Pull Up/Down 
*  ECDR_B_DM_DIG_HIZ    High Impedance Digital 
*  ECDR_B_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void ECDR_B_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(ECDR_B_0, mode);
}


/*******************************************************************************
* Function Name: ECDR_B_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro ECDR_B_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 ECDR_B_Read(void) 
{
    return (ECDR_B_PS & ECDR_B_MASK) >> ECDR_B_SHIFT;
}


/*******************************************************************************
* Function Name: ECDR_B_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 ECDR_B_ReadDataReg(void) 
{
    return (ECDR_B_DR & ECDR_B_MASK) >> ECDR_B_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(ECDR_B_INTSTAT) 

    /*******************************************************************************
    * Function Name: ECDR_B_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 ECDR_B_ClearInterrupt(void) 
    {
        return (ECDR_B_INTSTAT & ECDR_B_MASK) >> ECDR_B_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

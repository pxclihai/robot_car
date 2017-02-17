/*******************************************************************************
* File Name: M_UP.c  
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
#include "M_UP.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 M_UP__PORT == 15 && ((M_UP__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: M_UP_Write
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
void M_UP_Write(uint8 value) 
{
    uint8 staticBits = (M_UP_DR & (uint8)(~M_UP_MASK));
    M_UP_DR = staticBits | ((uint8)(value << M_UP_SHIFT) & M_UP_MASK);
}


/*******************************************************************************
* Function Name: M_UP_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  M_UP_DM_STRONG     Strong Drive 
*  M_UP_DM_OD_HI      Open Drain, Drives High 
*  M_UP_DM_OD_LO      Open Drain, Drives Low 
*  M_UP_DM_RES_UP     Resistive Pull Up 
*  M_UP_DM_RES_DWN    Resistive Pull Down 
*  M_UP_DM_RES_UPDWN  Resistive Pull Up/Down 
*  M_UP_DM_DIG_HIZ    High Impedance Digital 
*  M_UP_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void M_UP_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(M_UP_0, mode);
}


/*******************************************************************************
* Function Name: M_UP_Read
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
*  Macro M_UP_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 M_UP_Read(void) 
{
    return (M_UP_PS & M_UP_MASK) >> M_UP_SHIFT;
}


/*******************************************************************************
* Function Name: M_UP_ReadDataReg
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
uint8 M_UP_ReadDataReg(void) 
{
    return (M_UP_DR & M_UP_MASK) >> M_UP_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(M_UP_INTSTAT) 

    /*******************************************************************************
    * Function Name: M_UP_ClearInterrupt
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
    uint8 M_UP_ClearInterrupt(void) 
    {
        return (M_UP_INTSTAT & M_UP_MASK) >> M_UP_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

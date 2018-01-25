/*******************************************************************************
* File Name: IN1.c  
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
#include "IN1.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 IN1__PORT == 15 && ((IN1__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: IN1_Write
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
void IN1_Write(uint8 value) 
{
    uint8 staticBits = (IN1_DR & (uint8)(~IN1_MASK));
    IN1_DR = staticBits | ((uint8)(value << IN1_SHIFT) & IN1_MASK);
}


/*******************************************************************************
* Function Name: IN1_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  IN1_DM_STRONG     Strong Drive 
*  IN1_DM_OD_HI      Open Drain, Drives High 
*  IN1_DM_OD_LO      Open Drain, Drives Low 
*  IN1_DM_RES_UP     Resistive Pull Up 
*  IN1_DM_RES_DWN    Resistive Pull Down 
*  IN1_DM_RES_UPDWN  Resistive Pull Up/Down 
*  IN1_DM_DIG_HIZ    High Impedance Digital 
*  IN1_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void IN1_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(IN1_0, mode);
}


/*******************************************************************************
* Function Name: IN1_Read
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
*  Macro IN1_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 IN1_Read(void) 
{
    return (IN1_PS & IN1_MASK) >> IN1_SHIFT;
}


/*******************************************************************************
* Function Name: IN1_ReadDataReg
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
uint8 IN1_ReadDataReg(void) 
{
    return (IN1_DR & IN1_MASK) >> IN1_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(IN1_INTSTAT) 

    /*******************************************************************************
    * Function Name: IN1_ClearInterrupt
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
    uint8 IN1_ClearInterrupt(void) 
    {
        return (IN1_INTSTAT & IN1_MASK) >> IN1_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

/*******************************************************************************
* File Name: LED8.c  
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
#include "LED8.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 LED8__PORT == 15 && ((LED8__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: LED8_Write
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
void LED8_Write(uint8 value) 
{
    uint8 staticBits = (LED8_DR & (uint8)(~LED8_MASK));
    LED8_DR = staticBits | ((uint8)(value << LED8_SHIFT) & LED8_MASK);
}


/*******************************************************************************
* Function Name: LED8_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  LED8_DM_STRONG     Strong Drive 
*  LED8_DM_OD_HI      Open Drain, Drives High 
*  LED8_DM_OD_LO      Open Drain, Drives Low 
*  LED8_DM_RES_UP     Resistive Pull Up 
*  LED8_DM_RES_DWN    Resistive Pull Down 
*  LED8_DM_RES_UPDWN  Resistive Pull Up/Down 
*  LED8_DM_DIG_HIZ    High Impedance Digital 
*  LED8_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void LED8_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(LED8_0, mode);
}


/*******************************************************************************
* Function Name: LED8_Read
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
*  Macro LED8_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 LED8_Read(void) 
{
    return (LED8_PS & LED8_MASK) >> LED8_SHIFT;
}


/*******************************************************************************
* Function Name: LED8_ReadDataReg
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
uint8 LED8_ReadDataReg(void) 
{
    return (LED8_DR & LED8_MASK) >> LED8_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(LED8_INTSTAT) 

    /*******************************************************************************
    * Function Name: LED8_ClearInterrupt
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
    uint8 LED8_ClearInterrupt(void) 
    {
        return (LED8_INTSTAT & LED8_MASK) >> LED8_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
/*******************************************************************************
* File Name: debug_time.c  
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
#include "debug_time.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 debug_time__PORT == 15 && ((debug_time__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: debug_time_Write
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
void debug_time_Write(uint8 value) 
{
    uint8 staticBits = (debug_time_DR & (uint8)(~debug_time_MASK));
    debug_time_DR = staticBits | ((uint8)(value << debug_time_SHIFT) & debug_time_MASK);
}


/*******************************************************************************
* Function Name: debug_time_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  debug_time_DM_STRONG     Strong Drive 
*  debug_time_DM_OD_HI      Open Drain, Drives High 
*  debug_time_DM_OD_LO      Open Drain, Drives Low 
*  debug_time_DM_RES_UP     Resistive Pull Up 
*  debug_time_DM_RES_DWN    Resistive Pull Down 
*  debug_time_DM_RES_UPDWN  Resistive Pull Up/Down 
*  debug_time_DM_DIG_HIZ    High Impedance Digital 
*  debug_time_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void debug_time_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(debug_time_0, mode);
}


/*******************************************************************************
* Function Name: debug_time_Read
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
*  Macro debug_time_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 debug_time_Read(void) 
{
    return (debug_time_PS & debug_time_MASK) >> debug_time_SHIFT;
}


/*******************************************************************************
* Function Name: debug_time_ReadDataReg
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
uint8 debug_time_ReadDataReg(void) 
{
    return (debug_time_DR & debug_time_MASK) >> debug_time_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(debug_time_INTSTAT) 

    /*******************************************************************************
    * Function Name: debug_time_ClearInterrupt
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
    uint8 debug_time_ClearInterrupt(void) 
    {
        return (debug_time_INTSTAT & debug_time_MASK) >> debug_time_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

/*******************************************************************************
* File Name: STEERING_3.c  
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
#include "STEERING_3.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 STEERING_3__PORT == 15 && ((STEERING_3__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: STEERING_3_Write
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
void STEERING_3_Write(uint8 value) 
{
    uint8 staticBits = (STEERING_3_DR & (uint8)(~STEERING_3_MASK));
    STEERING_3_DR = staticBits | ((uint8)(value << STEERING_3_SHIFT) & STEERING_3_MASK);
}


/*******************************************************************************
* Function Name: STEERING_3_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  STEERING_3_DM_STRONG     Strong Drive 
*  STEERING_3_DM_OD_HI      Open Drain, Drives High 
*  STEERING_3_DM_OD_LO      Open Drain, Drives Low 
*  STEERING_3_DM_RES_UP     Resistive Pull Up 
*  STEERING_3_DM_RES_DWN    Resistive Pull Down 
*  STEERING_3_DM_RES_UPDWN  Resistive Pull Up/Down 
*  STEERING_3_DM_DIG_HIZ    High Impedance Digital 
*  STEERING_3_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void STEERING_3_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(STEERING_3_0, mode);
}


/*******************************************************************************
* Function Name: STEERING_3_Read
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
*  Macro STEERING_3_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 STEERING_3_Read(void) 
{
    return (STEERING_3_PS & STEERING_3_MASK) >> STEERING_3_SHIFT;
}


/*******************************************************************************
* Function Name: STEERING_3_ReadDataReg
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
uint8 STEERING_3_ReadDataReg(void) 
{
    return (STEERING_3_DR & STEERING_3_MASK) >> STEERING_3_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(STEERING_3_INTSTAT) 

    /*******************************************************************************
    * Function Name: STEERING_3_ClearInterrupt
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
    uint8 STEERING_3_ClearInterrupt(void) 
    {
        return (STEERING_3_INTSTAT & STEERING_3_MASK) >> STEERING_3_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

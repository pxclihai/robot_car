/*******************************************************************************
* File Name: CONTRAL_LIDAR.c  
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
#include "CONTRAL_LIDAR.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 CONTRAL_LIDAR__PORT == 15 && ((CONTRAL_LIDAR__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: CONTRAL_LIDAR_Write
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
void CONTRAL_LIDAR_Write(uint8 value) 
{
    uint8 staticBits = (CONTRAL_LIDAR_DR & (uint8)(~CONTRAL_LIDAR_MASK));
    CONTRAL_LIDAR_DR = staticBits | ((uint8)(value << CONTRAL_LIDAR_SHIFT) & CONTRAL_LIDAR_MASK);
}


/*******************************************************************************
* Function Name: CONTRAL_LIDAR_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  CONTRAL_LIDAR_DM_STRONG     Strong Drive 
*  CONTRAL_LIDAR_DM_OD_HI      Open Drain, Drives High 
*  CONTRAL_LIDAR_DM_OD_LO      Open Drain, Drives Low 
*  CONTRAL_LIDAR_DM_RES_UP     Resistive Pull Up 
*  CONTRAL_LIDAR_DM_RES_DWN    Resistive Pull Down 
*  CONTRAL_LIDAR_DM_RES_UPDWN  Resistive Pull Up/Down 
*  CONTRAL_LIDAR_DM_DIG_HIZ    High Impedance Digital 
*  CONTRAL_LIDAR_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void CONTRAL_LIDAR_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(CONTRAL_LIDAR_0, mode);
}


/*******************************************************************************
* Function Name: CONTRAL_LIDAR_Read
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
*  Macro CONTRAL_LIDAR_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 CONTRAL_LIDAR_Read(void) 
{
    return (CONTRAL_LIDAR_PS & CONTRAL_LIDAR_MASK) >> CONTRAL_LIDAR_SHIFT;
}


/*******************************************************************************
* Function Name: CONTRAL_LIDAR_ReadDataReg
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
uint8 CONTRAL_LIDAR_ReadDataReg(void) 
{
    return (CONTRAL_LIDAR_DR & CONTRAL_LIDAR_MASK) >> CONTRAL_LIDAR_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(CONTRAL_LIDAR_INTSTAT) 

    /*******************************************************************************
    * Function Name: CONTRAL_LIDAR_ClearInterrupt
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
    uint8 CONTRAL_LIDAR_ClearInterrupt(void) 
    {
        return (CONTRAL_LIDAR_INTSTAT & CONTRAL_LIDAR_MASK) >> CONTRAL_LIDAR_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */

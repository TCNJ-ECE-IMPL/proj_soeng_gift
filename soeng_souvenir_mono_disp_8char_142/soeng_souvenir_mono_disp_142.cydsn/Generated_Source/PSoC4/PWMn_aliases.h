/*******************************************************************************
* File Name: PWMn.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PWMn_ALIASES_H) /* Pins PWMn_ALIASES_H */
#define CY_PINS_PWMn_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define PWMn_0			(PWMn__0__PC)
#define PWMn_0_PS		(PWMn__0__PS)
#define PWMn_0_PC		(PWMn__0__PC)
#define PWMn_0_DR		(PWMn__0__DR)
#define PWMn_0_SHIFT	(PWMn__0__SHIFT)
#define PWMn_0_INTR	((uint16)((uint16)0x0003u << (PWMn__0__SHIFT*2u)))

#define PWMn_INTR_ALL	 ((uint16)(PWMn_0_INTR))


#endif /* End Pins PWMn_ALIASES_H */


/* [] END OF FILE */

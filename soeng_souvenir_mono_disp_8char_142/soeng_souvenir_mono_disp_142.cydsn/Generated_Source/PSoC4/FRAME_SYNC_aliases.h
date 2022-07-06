/*******************************************************************************
* File Name: FRAME_SYNC.h  
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

#if !defined(CY_PINS_FRAME_SYNC_ALIASES_H) /* Pins FRAME_SYNC_ALIASES_H */
#define CY_PINS_FRAME_SYNC_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define FRAME_SYNC_0			(FRAME_SYNC__0__PC)
#define FRAME_SYNC_0_PS		(FRAME_SYNC__0__PS)
#define FRAME_SYNC_0_PC		(FRAME_SYNC__0__PC)
#define FRAME_SYNC_0_DR		(FRAME_SYNC__0__DR)
#define FRAME_SYNC_0_SHIFT	(FRAME_SYNC__0__SHIFT)
#define FRAME_SYNC_0_INTR	((uint16)((uint16)0x0003u << (FRAME_SYNC__0__SHIFT*2u)))

#define FRAME_SYNC_INTR_ALL	 ((uint16)(FRAME_SYNC_0_INTR))


#endif /* End Pins FRAME_SYNC_ALIASES_H */


/* [] END OF FILE */

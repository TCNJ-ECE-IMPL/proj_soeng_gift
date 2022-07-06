/*******************************************************************************
* File Name: FRAME_SYNC.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "FRAME_SYNC.h"

static FRAME_SYNC_BACKUP_STRUCT  FRAME_SYNC_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: FRAME_SYNC_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet FRAME_SYNC_SUT.c usage_FRAME_SYNC_Sleep_Wakeup
*******************************************************************************/
void FRAME_SYNC_Sleep(void)
{
    #if defined(FRAME_SYNC__PC)
        FRAME_SYNC_backup.pcState = FRAME_SYNC_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            FRAME_SYNC_backup.usbState = FRAME_SYNC_CR1_REG;
            FRAME_SYNC_USB_POWER_REG |= FRAME_SYNC_USBIO_ENTER_SLEEP;
            FRAME_SYNC_CR1_REG &= FRAME_SYNC_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FRAME_SYNC__SIO)
        FRAME_SYNC_backup.sioState = FRAME_SYNC_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        FRAME_SYNC_SIO_REG &= (uint32)(~FRAME_SYNC_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: FRAME_SYNC_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to FRAME_SYNC_Sleep() for an example usage.
*******************************************************************************/
void FRAME_SYNC_Wakeup(void)
{
    #if defined(FRAME_SYNC__PC)
        FRAME_SYNC_PC = FRAME_SYNC_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            FRAME_SYNC_USB_POWER_REG &= FRAME_SYNC_USBIO_EXIT_SLEEP_PH1;
            FRAME_SYNC_CR1_REG = FRAME_SYNC_backup.usbState;
            FRAME_SYNC_USB_POWER_REG &= FRAME_SYNC_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FRAME_SYNC__SIO)
        FRAME_SYNC_SIO_REG = FRAME_SYNC_backup.sioState;
    #endif
}


/* [] END OF FILE */

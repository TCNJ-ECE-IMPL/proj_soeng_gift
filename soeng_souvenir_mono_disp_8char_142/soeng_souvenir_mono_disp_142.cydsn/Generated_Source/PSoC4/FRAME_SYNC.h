/*******************************************************************************
* File Name: FRAME_SYNC.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_FRAME_SYNC_H) /* Pins FRAME_SYNC_H */
#define CY_PINS_FRAME_SYNC_H

#include "cytypes.h"
#include "cyfitter.h"
#include "FRAME_SYNC_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} FRAME_SYNC_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   FRAME_SYNC_Read(void);
void    FRAME_SYNC_Write(uint8 value);
uint8   FRAME_SYNC_ReadDataReg(void);
#if defined(FRAME_SYNC__PC) || (CY_PSOC4_4200L) 
    void    FRAME_SYNC_SetDriveMode(uint8 mode);
#endif
void    FRAME_SYNC_SetInterruptMode(uint16 position, uint16 mode);
uint8   FRAME_SYNC_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void FRAME_SYNC_Sleep(void); 
void FRAME_SYNC_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(FRAME_SYNC__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define FRAME_SYNC_DRIVE_MODE_BITS        (3)
    #define FRAME_SYNC_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - FRAME_SYNC_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the FRAME_SYNC_SetDriveMode() function.
         *  @{
         */
        #define FRAME_SYNC_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define FRAME_SYNC_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define FRAME_SYNC_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define FRAME_SYNC_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define FRAME_SYNC_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define FRAME_SYNC_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define FRAME_SYNC_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define FRAME_SYNC_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define FRAME_SYNC_MASK               FRAME_SYNC__MASK
#define FRAME_SYNC_SHIFT              FRAME_SYNC__SHIFT
#define FRAME_SYNC_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FRAME_SYNC_SetInterruptMode() function.
     *  @{
     */
        #define FRAME_SYNC_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define FRAME_SYNC_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define FRAME_SYNC_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define FRAME_SYNC_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(FRAME_SYNC__SIO)
    #define FRAME_SYNC_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(FRAME_SYNC__PC) && (CY_PSOC4_4200L)
    #define FRAME_SYNC_USBIO_ENABLE               ((uint32)0x80000000u)
    #define FRAME_SYNC_USBIO_DISABLE              ((uint32)(~FRAME_SYNC_USBIO_ENABLE))
    #define FRAME_SYNC_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define FRAME_SYNC_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define FRAME_SYNC_USBIO_ENTER_SLEEP          ((uint32)((1u << FRAME_SYNC_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << FRAME_SYNC_USBIO_SUSPEND_DEL_SHIFT)))
    #define FRAME_SYNC_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << FRAME_SYNC_USBIO_SUSPEND_SHIFT)))
    #define FRAME_SYNC_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << FRAME_SYNC_USBIO_SUSPEND_DEL_SHIFT)))
    #define FRAME_SYNC_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(FRAME_SYNC__PC)
    /* Port Configuration */
    #define FRAME_SYNC_PC                 (* (reg32 *) FRAME_SYNC__PC)
#endif
/* Pin State */
#define FRAME_SYNC_PS                     (* (reg32 *) FRAME_SYNC__PS)
/* Data Register */
#define FRAME_SYNC_DR                     (* (reg32 *) FRAME_SYNC__DR)
/* Input Buffer Disable Override */
#define FRAME_SYNC_INP_DIS                (* (reg32 *) FRAME_SYNC__PC2)

/* Interrupt configuration Registers */
#define FRAME_SYNC_INTCFG                 (* (reg32 *) FRAME_SYNC__INTCFG)
#define FRAME_SYNC_INTSTAT                (* (reg32 *) FRAME_SYNC__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define FRAME_SYNC_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(FRAME_SYNC__SIO)
    #define FRAME_SYNC_SIO_REG            (* (reg32 *) FRAME_SYNC__SIO)
#endif /* (FRAME_SYNC__SIO_CFG) */

/* USBIO registers */
#if !defined(FRAME_SYNC__PC) && (CY_PSOC4_4200L)
    #define FRAME_SYNC_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define FRAME_SYNC_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define FRAME_SYNC_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define FRAME_SYNC_DRIVE_MODE_SHIFT       (0x00u)
#define FRAME_SYNC_DRIVE_MODE_MASK        (0x07u << FRAME_SYNC_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins FRAME_SYNC_H */


/* [] END OF FILE */

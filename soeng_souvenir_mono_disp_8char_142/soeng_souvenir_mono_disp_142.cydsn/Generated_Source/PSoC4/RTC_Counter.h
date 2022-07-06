/*******************************************************************************
* File Name: RTC_Counter.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the RTC_Counter
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_RTC_Counter_H)
#define CY_TCPWM_RTC_Counter_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} RTC_Counter_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  RTC_Counter_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define RTC_Counter_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define RTC_Counter_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define RTC_Counter_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define RTC_Counter_QUAD_ENCODING_MODES            (0lu)
#define RTC_Counter_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define RTC_Counter_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define RTC_Counter_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define RTC_Counter_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define RTC_Counter_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define RTC_Counter_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define RTC_Counter_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define RTC_Counter_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define RTC_Counter_TC_RUN_MODE                    (0lu)
#define RTC_Counter_TC_COUNTER_MODE                (0lu)
#define RTC_Counter_TC_COMP_CAP_MODE               (2lu)
#define RTC_Counter_TC_PRESCALER                   (0lu)

/* Signal modes */
#define RTC_Counter_TC_RELOAD_SIGNAL_MODE          (0lu)
#define RTC_Counter_TC_COUNT_SIGNAL_MODE           (3lu)
#define RTC_Counter_TC_START_SIGNAL_MODE           (0lu)
#define RTC_Counter_TC_STOP_SIGNAL_MODE            (0lu)
#define RTC_Counter_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define RTC_Counter_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define RTC_Counter_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define RTC_Counter_TC_START_SIGNAL_PRESENT        (0lu)
#define RTC_Counter_TC_STOP_SIGNAL_PRESENT         (0lu)
#define RTC_Counter_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define RTC_Counter_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define RTC_Counter_PWM_KILL_EVENT                 (0lu)
#define RTC_Counter_PWM_STOP_EVENT                 (0lu)
#define RTC_Counter_PWM_MODE                       (4lu)
#define RTC_Counter_PWM_OUT_N_INVERT               (0lu)
#define RTC_Counter_PWM_OUT_INVERT                 (0lu)
#define RTC_Counter_PWM_ALIGN                      (0lu)
#define RTC_Counter_PWM_RUN_MODE                   (0lu)
#define RTC_Counter_PWM_DEAD_TIME_CYCLE            (0lu)
#define RTC_Counter_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define RTC_Counter_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define RTC_Counter_PWM_COUNT_SIGNAL_MODE          (3lu)
#define RTC_Counter_PWM_START_SIGNAL_MODE          (0lu)
#define RTC_Counter_PWM_STOP_SIGNAL_MODE           (0lu)
#define RTC_Counter_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define RTC_Counter_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define RTC_Counter_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define RTC_Counter_PWM_START_SIGNAL_PRESENT       (0lu)
#define RTC_Counter_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define RTC_Counter_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define RTC_Counter_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define RTC_Counter_TC_PERIOD_VALUE                (999lu)
#define RTC_Counter_TC_COMPARE_VALUE               (65535lu)
#define RTC_Counter_TC_COMPARE_BUF_VALUE           (65535lu)
#define RTC_Counter_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define RTC_Counter_PWM_PERIOD_VALUE               (65535lu)
#define RTC_Counter_PWM_PERIOD_BUF_VALUE           (65535lu)
#define RTC_Counter_PWM_PERIOD_SWAP                (0lu)
#define RTC_Counter_PWM_COMPARE_VALUE              (65535lu)
#define RTC_Counter_PWM_COMPARE_BUF_VALUE          (65535lu)
#define RTC_Counter_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define RTC_Counter__LEFT 0
#define RTC_Counter__RIGHT 1
#define RTC_Counter__CENTER 2
#define RTC_Counter__ASYMMETRIC 3

#define RTC_Counter__X1 0
#define RTC_Counter__X2 1
#define RTC_Counter__X4 2

#define RTC_Counter__PWM 4
#define RTC_Counter__PWM_DT 5
#define RTC_Counter__PWM_PR 6

#define RTC_Counter__INVERSE 1
#define RTC_Counter__DIRECT 0

#define RTC_Counter__CAPTURE 2
#define RTC_Counter__COMPARE 0

#define RTC_Counter__TRIG_LEVEL 3
#define RTC_Counter__TRIG_RISING 0
#define RTC_Counter__TRIG_FALLING 1
#define RTC_Counter__TRIG_BOTH 2

#define RTC_Counter__INTR_MASK_TC 1
#define RTC_Counter__INTR_MASK_CC_MATCH 2
#define RTC_Counter__INTR_MASK_NONE 0
#define RTC_Counter__INTR_MASK_TC_CC 3

#define RTC_Counter__UNCONFIG 8
#define RTC_Counter__TIMER 1
#define RTC_Counter__QUAD 3
#define RTC_Counter__PWM_SEL 7

#define RTC_Counter__COUNT_UP 0
#define RTC_Counter__COUNT_DOWN 1
#define RTC_Counter__COUNT_UPDOWN0 2
#define RTC_Counter__COUNT_UPDOWN1 3


/* Prescaler */
#define RTC_Counter_PRESCALE_DIVBY1                ((uint32)(0u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY2                ((uint32)(1u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY4                ((uint32)(2u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY8                ((uint32)(3u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY16               ((uint32)(4u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY32               ((uint32)(5u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY64               ((uint32)(6u << RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_PRESCALE_DIVBY128              ((uint32)(7u << RTC_Counter_PRESCALER_SHIFT))

/* TCPWM set modes */
#define RTC_Counter_MODE_TIMER_COMPARE             ((uint32)(RTC_Counter__COMPARE         <<  \
                                                                  RTC_Counter_MODE_SHIFT))
#define RTC_Counter_MODE_TIMER_CAPTURE             ((uint32)(RTC_Counter__CAPTURE         <<  \
                                                                  RTC_Counter_MODE_SHIFT))
#define RTC_Counter_MODE_QUAD                      ((uint32)(RTC_Counter__QUAD            <<  \
                                                                  RTC_Counter_MODE_SHIFT))
#define RTC_Counter_MODE_PWM                       ((uint32)(RTC_Counter__PWM             <<  \
                                                                  RTC_Counter_MODE_SHIFT))
#define RTC_Counter_MODE_PWM_DT                    ((uint32)(RTC_Counter__PWM_DT          <<  \
                                                                  RTC_Counter_MODE_SHIFT))
#define RTC_Counter_MODE_PWM_PR                    ((uint32)(RTC_Counter__PWM_PR          <<  \
                                                                  RTC_Counter_MODE_SHIFT))

/* Quad Modes */
#define RTC_Counter_MODE_X1                        ((uint32)(RTC_Counter__X1              <<  \
                                                                  RTC_Counter_QUAD_MODE_SHIFT))
#define RTC_Counter_MODE_X2                        ((uint32)(RTC_Counter__X2              <<  \
                                                                  RTC_Counter_QUAD_MODE_SHIFT))
#define RTC_Counter_MODE_X4                        ((uint32)(RTC_Counter__X4              <<  \
                                                                  RTC_Counter_QUAD_MODE_SHIFT))

/* Counter modes */
#define RTC_Counter_COUNT_UP                       ((uint32)(RTC_Counter__COUNT_UP        <<  \
                                                                  RTC_Counter_UPDOWN_SHIFT))
#define RTC_Counter_COUNT_DOWN                     ((uint32)(RTC_Counter__COUNT_DOWN      <<  \
                                                                  RTC_Counter_UPDOWN_SHIFT))
#define RTC_Counter_COUNT_UPDOWN0                  ((uint32)(RTC_Counter__COUNT_UPDOWN0   <<  \
                                                                  RTC_Counter_UPDOWN_SHIFT))
#define RTC_Counter_COUNT_UPDOWN1                  ((uint32)(RTC_Counter__COUNT_UPDOWN1   <<  \
                                                                  RTC_Counter_UPDOWN_SHIFT))

/* PWM output invert */
#define RTC_Counter_INVERT_LINE                    ((uint32)(RTC_Counter__INVERSE         <<  \
                                                                  RTC_Counter_INV_OUT_SHIFT))
#define RTC_Counter_INVERT_LINE_N                  ((uint32)(RTC_Counter__INVERSE         <<  \
                                                                  RTC_Counter_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define RTC_Counter_TRIG_RISING                    ((uint32)RTC_Counter__TRIG_RISING)
#define RTC_Counter_TRIG_FALLING                   ((uint32)RTC_Counter__TRIG_FALLING)
#define RTC_Counter_TRIG_BOTH                      ((uint32)RTC_Counter__TRIG_BOTH)
#define RTC_Counter_TRIG_LEVEL                     ((uint32)RTC_Counter__TRIG_LEVEL)

/* Interrupt mask */
#define RTC_Counter_INTR_MASK_TC                   ((uint32)RTC_Counter__INTR_MASK_TC)
#define RTC_Counter_INTR_MASK_CC_MATCH             ((uint32)RTC_Counter__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define RTC_Counter_CC_MATCH_SET                   (0x00u)
#define RTC_Counter_CC_MATCH_CLEAR                 (0x01u)
#define RTC_Counter_CC_MATCH_INVERT                (0x02u)
#define RTC_Counter_CC_MATCH_NO_CHANGE             (0x03u)
#define RTC_Counter_OVERLOW_SET                    (0x00u)
#define RTC_Counter_OVERLOW_CLEAR                  (0x04u)
#define RTC_Counter_OVERLOW_INVERT                 (0x08u)
#define RTC_Counter_OVERLOW_NO_CHANGE              (0x0Cu)
#define RTC_Counter_UNDERFLOW_SET                  (0x00u)
#define RTC_Counter_UNDERFLOW_CLEAR                (0x10u)
#define RTC_Counter_UNDERFLOW_INVERT               (0x20u)
#define RTC_Counter_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define RTC_Counter_PWM_MODE_LEFT                  (RTC_Counter_CC_MATCH_CLEAR        |   \
                                                         RTC_Counter_OVERLOW_SET           |   \
                                                         RTC_Counter_UNDERFLOW_NO_CHANGE)
#define RTC_Counter_PWM_MODE_RIGHT                 (RTC_Counter_CC_MATCH_SET          |   \
                                                         RTC_Counter_OVERLOW_NO_CHANGE     |   \
                                                         RTC_Counter_UNDERFLOW_CLEAR)
#define RTC_Counter_PWM_MODE_ASYM                  (RTC_Counter_CC_MATCH_INVERT       |   \
                                                         RTC_Counter_OVERLOW_SET           |   \
                                                         RTC_Counter_UNDERFLOW_CLEAR)

#if (RTC_Counter_CY_TCPWM_V2)
    #if(RTC_Counter_CY_TCPWM_4000)
        #define RTC_Counter_PWM_MODE_CENTER                (RTC_Counter_CC_MATCH_INVERT       |   \
                                                                 RTC_Counter_OVERLOW_NO_CHANGE     |   \
                                                                 RTC_Counter_UNDERFLOW_CLEAR)
    #else
        #define RTC_Counter_PWM_MODE_CENTER                (RTC_Counter_CC_MATCH_INVERT       |   \
                                                                 RTC_Counter_OVERLOW_SET           |   \
                                                                 RTC_Counter_UNDERFLOW_CLEAR)
    #endif /* (RTC_Counter_CY_TCPWM_4000) */
#else
    #define RTC_Counter_PWM_MODE_CENTER                (RTC_Counter_CC_MATCH_INVERT       |   \
                                                             RTC_Counter_OVERLOW_NO_CHANGE     |   \
                                                             RTC_Counter_UNDERFLOW_CLEAR)
#endif /* (RTC_Counter_CY_TCPWM_NEW) */

/* Command operations without condition */
#define RTC_Counter_CMD_CAPTURE                    (0u)
#define RTC_Counter_CMD_RELOAD                     (8u)
#define RTC_Counter_CMD_STOP                       (16u)
#define RTC_Counter_CMD_START                      (24u)

/* Status */
#define RTC_Counter_STATUS_DOWN                    (1u)
#define RTC_Counter_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   RTC_Counter_Init(void);
void   RTC_Counter_Enable(void);
void   RTC_Counter_Start(void);
void   RTC_Counter_Stop(void);

void   RTC_Counter_SetMode(uint32 mode);
void   RTC_Counter_SetCounterMode(uint32 counterMode);
void   RTC_Counter_SetPWMMode(uint32 modeMask);
void   RTC_Counter_SetQDMode(uint32 qdMode);

void   RTC_Counter_SetPrescaler(uint32 prescaler);
void   RTC_Counter_TriggerCommand(uint32 mask, uint32 command);
void   RTC_Counter_SetOneShot(uint32 oneShotEnable);
uint32 RTC_Counter_ReadStatus(void);

void   RTC_Counter_SetPWMSyncKill(uint32 syncKillEnable);
void   RTC_Counter_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   RTC_Counter_SetPWMDeadTime(uint32 deadTime);
void   RTC_Counter_SetPWMInvert(uint32 mask);

void   RTC_Counter_SetInterruptMode(uint32 interruptMask);
uint32 RTC_Counter_GetInterruptSourceMasked(void);
uint32 RTC_Counter_GetInterruptSource(void);
void   RTC_Counter_ClearInterrupt(uint32 interruptMask);
void   RTC_Counter_SetInterrupt(uint32 interruptMask);

void   RTC_Counter_WriteCounter(uint32 count);
uint32 RTC_Counter_ReadCounter(void);

uint32 RTC_Counter_ReadCapture(void);
uint32 RTC_Counter_ReadCaptureBuf(void);

void   RTC_Counter_WritePeriod(uint32 period);
uint32 RTC_Counter_ReadPeriod(void);
void   RTC_Counter_WritePeriodBuf(uint32 periodBuf);
uint32 RTC_Counter_ReadPeriodBuf(void);

void   RTC_Counter_WriteCompare(uint32 compare);
uint32 RTC_Counter_ReadCompare(void);
void   RTC_Counter_WriteCompareBuf(uint32 compareBuf);
uint32 RTC_Counter_ReadCompareBuf(void);

void   RTC_Counter_SetPeriodSwap(uint32 swapEnable);
void   RTC_Counter_SetCompareSwap(uint32 swapEnable);

void   RTC_Counter_SetCaptureMode(uint32 triggerMode);
void   RTC_Counter_SetReloadMode(uint32 triggerMode);
void   RTC_Counter_SetStartMode(uint32 triggerMode);
void   RTC_Counter_SetStopMode(uint32 triggerMode);
void   RTC_Counter_SetCountMode(uint32 triggerMode);

void   RTC_Counter_SaveConfig(void);
void   RTC_Counter_RestoreConfig(void);
void   RTC_Counter_Sleep(void);
void   RTC_Counter_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define RTC_Counter_BLOCK_CONTROL_REG              (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define RTC_Counter_BLOCK_CONTROL_PTR              ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define RTC_Counter_COMMAND_REG                    (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define RTC_Counter_COMMAND_PTR                    ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define RTC_Counter_INTRRUPT_CAUSE_REG             (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define RTC_Counter_INTRRUPT_CAUSE_PTR             ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define RTC_Counter_CONTROL_REG                    (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__CTRL )
#define RTC_Counter_CONTROL_PTR                    ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__CTRL )
#define RTC_Counter_STATUS_REG                     (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__STATUS )
#define RTC_Counter_STATUS_PTR                     ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__STATUS )
#define RTC_Counter_COUNTER_REG                    (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__COUNTER )
#define RTC_Counter_COUNTER_PTR                    ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__COUNTER )
#define RTC_Counter_COMP_CAP_REG                   (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__CC )
#define RTC_Counter_COMP_CAP_PTR                   ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__CC )
#define RTC_Counter_COMP_CAP_BUF_REG               (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__CC_BUFF )
#define RTC_Counter_COMP_CAP_BUF_PTR               ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__CC_BUFF )
#define RTC_Counter_PERIOD_REG                     (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__PERIOD )
#define RTC_Counter_PERIOD_PTR                     ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__PERIOD )
#define RTC_Counter_PERIOD_BUF_REG                 (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define RTC_Counter_PERIOD_BUF_PTR                 ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define RTC_Counter_TRIG_CONTROL0_REG              (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define RTC_Counter_TRIG_CONTROL0_PTR              ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define RTC_Counter_TRIG_CONTROL1_REG              (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define RTC_Counter_TRIG_CONTROL1_PTR              ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define RTC_Counter_TRIG_CONTROL2_REG              (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define RTC_Counter_TRIG_CONTROL2_PTR              ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define RTC_Counter_INTERRUPT_REQ_REG              (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR )
#define RTC_Counter_INTERRUPT_REQ_PTR              ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR )
#define RTC_Counter_INTERRUPT_SET_REG              (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR_SET )
#define RTC_Counter_INTERRUPT_SET_PTR              ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR_SET )
#define RTC_Counter_INTERRUPT_MASK_REG             (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR_MASK )
#define RTC_Counter_INTERRUPT_MASK_PTR             ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR_MASK )
#define RTC_Counter_INTERRUPT_MASKED_REG           (*(reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR_MASKED )
#define RTC_Counter_INTERRUPT_MASKED_PTR           ( (reg32 *) RTC_Counter_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define RTC_Counter_MASK                           ((uint32)RTC_Counter_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define RTC_Counter_RELOAD_CC_SHIFT                (0u)
#define RTC_Counter_RELOAD_PERIOD_SHIFT            (1u)
#define RTC_Counter_PWM_SYNC_KILL_SHIFT            (2u)
#define RTC_Counter_PWM_STOP_KILL_SHIFT            (3u)
#define RTC_Counter_PRESCALER_SHIFT                (8u)
#define RTC_Counter_UPDOWN_SHIFT                   (16u)
#define RTC_Counter_ONESHOT_SHIFT                  (18u)
#define RTC_Counter_QUAD_MODE_SHIFT                (20u)
#define RTC_Counter_INV_OUT_SHIFT                  (20u)
#define RTC_Counter_INV_COMPL_OUT_SHIFT            (21u)
#define RTC_Counter_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define RTC_Counter_RELOAD_CC_MASK                 ((uint32)(RTC_Counter_1BIT_MASK        <<  \
                                                                            RTC_Counter_RELOAD_CC_SHIFT))
#define RTC_Counter_RELOAD_PERIOD_MASK             ((uint32)(RTC_Counter_1BIT_MASK        <<  \
                                                                            RTC_Counter_RELOAD_PERIOD_SHIFT))
#define RTC_Counter_PWM_SYNC_KILL_MASK             ((uint32)(RTC_Counter_1BIT_MASK        <<  \
                                                                            RTC_Counter_PWM_SYNC_KILL_SHIFT))
#define RTC_Counter_PWM_STOP_KILL_MASK             ((uint32)(RTC_Counter_1BIT_MASK        <<  \
                                                                            RTC_Counter_PWM_STOP_KILL_SHIFT))
#define RTC_Counter_PRESCALER_MASK                 ((uint32)(RTC_Counter_8BIT_MASK        <<  \
                                                                            RTC_Counter_PRESCALER_SHIFT))
#define RTC_Counter_UPDOWN_MASK                    ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                            RTC_Counter_UPDOWN_SHIFT))
#define RTC_Counter_ONESHOT_MASK                   ((uint32)(RTC_Counter_1BIT_MASK        <<  \
                                                                            RTC_Counter_ONESHOT_SHIFT))
#define RTC_Counter_QUAD_MODE_MASK                 ((uint32)(RTC_Counter_3BIT_MASK        <<  \
                                                                            RTC_Counter_QUAD_MODE_SHIFT))
#define RTC_Counter_INV_OUT_MASK                   ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                            RTC_Counter_INV_OUT_SHIFT))
#define RTC_Counter_MODE_MASK                      ((uint32)(RTC_Counter_3BIT_MASK        <<  \
                                                                            RTC_Counter_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define RTC_Counter_CAPTURE_SHIFT                  (0u)
#define RTC_Counter_COUNT_SHIFT                    (2u)
#define RTC_Counter_RELOAD_SHIFT                   (4u)
#define RTC_Counter_STOP_SHIFT                     (6u)
#define RTC_Counter_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define RTC_Counter_CAPTURE_MASK                   ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                  RTC_Counter_CAPTURE_SHIFT))
#define RTC_Counter_COUNT_MASK                     ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                  RTC_Counter_COUNT_SHIFT))
#define RTC_Counter_RELOAD_MASK                    ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                  RTC_Counter_RELOAD_SHIFT))
#define RTC_Counter_STOP_MASK                      ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                  RTC_Counter_STOP_SHIFT))
#define RTC_Counter_START_MASK                     ((uint32)(RTC_Counter_2BIT_MASK        <<  \
                                                                  RTC_Counter_START_SHIFT))

/* MASK */
#define RTC_Counter_1BIT_MASK                      ((uint32)0x01u)
#define RTC_Counter_2BIT_MASK                      ((uint32)0x03u)
#define RTC_Counter_3BIT_MASK                      ((uint32)0x07u)
#define RTC_Counter_6BIT_MASK                      ((uint32)0x3Fu)
#define RTC_Counter_8BIT_MASK                      ((uint32)0xFFu)
#define RTC_Counter_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define RTC_Counter_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define RTC_Counter_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(RTC_Counter_QUAD_ENCODING_MODES     << RTC_Counter_QUAD_MODE_SHIFT))       |\
         ((uint32)(RTC_Counter_CONFIG                  << RTC_Counter_MODE_SHIFT)))

#define RTC_Counter_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(RTC_Counter_PWM_STOP_EVENT          << RTC_Counter_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(RTC_Counter_PWM_OUT_INVERT          << RTC_Counter_INV_OUT_SHIFT))         |\
         ((uint32)(RTC_Counter_PWM_OUT_N_INVERT        << RTC_Counter_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(RTC_Counter_PWM_MODE                << RTC_Counter_MODE_SHIFT)))

#define RTC_Counter_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(RTC_Counter_PWM_RUN_MODE         << RTC_Counter_ONESHOT_SHIFT))
            
#define RTC_Counter_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(RTC_Counter_PWM_ALIGN            << RTC_Counter_UPDOWN_SHIFT))

#define RTC_Counter_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(RTC_Counter_PWM_KILL_EVENT      << RTC_Counter_PWM_SYNC_KILL_SHIFT))

#define RTC_Counter_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(RTC_Counter_PWM_DEAD_TIME_CYCLE  << RTC_Counter_PRESCALER_SHIFT))

#define RTC_Counter_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(RTC_Counter_PWM_PRESCALER        << RTC_Counter_PRESCALER_SHIFT))

#define RTC_Counter_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(RTC_Counter_TC_PRESCALER            << RTC_Counter_PRESCALER_SHIFT))       |\
         ((uint32)(RTC_Counter_TC_COUNTER_MODE         << RTC_Counter_UPDOWN_SHIFT))          |\
         ((uint32)(RTC_Counter_TC_RUN_MODE             << RTC_Counter_ONESHOT_SHIFT))         |\
         ((uint32)(RTC_Counter_TC_COMP_CAP_MODE        << RTC_Counter_MODE_SHIFT)))
        
#define RTC_Counter_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(RTC_Counter_QUAD_PHIA_SIGNAL_MODE   << RTC_Counter_COUNT_SHIFT))           |\
         ((uint32)(RTC_Counter_QUAD_INDEX_SIGNAL_MODE  << RTC_Counter_RELOAD_SHIFT))          |\
         ((uint32)(RTC_Counter_QUAD_STOP_SIGNAL_MODE   << RTC_Counter_STOP_SHIFT))            |\
         ((uint32)(RTC_Counter_QUAD_PHIB_SIGNAL_MODE   << RTC_Counter_START_SHIFT)))

#define RTC_Counter_PWM_SIGNALS_MODES                                                              \
        (((uint32)(RTC_Counter_PWM_SWITCH_SIGNAL_MODE  << RTC_Counter_CAPTURE_SHIFT))         |\
         ((uint32)(RTC_Counter_PWM_COUNT_SIGNAL_MODE   << RTC_Counter_COUNT_SHIFT))           |\
         ((uint32)(RTC_Counter_PWM_RELOAD_SIGNAL_MODE  << RTC_Counter_RELOAD_SHIFT))          |\
         ((uint32)(RTC_Counter_PWM_STOP_SIGNAL_MODE    << RTC_Counter_STOP_SHIFT))            |\
         ((uint32)(RTC_Counter_PWM_START_SIGNAL_MODE   << RTC_Counter_START_SHIFT)))

#define RTC_Counter_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(RTC_Counter_TC_CAPTURE_SIGNAL_MODE  << RTC_Counter_CAPTURE_SHIFT))         |\
         ((uint32)(RTC_Counter_TC_COUNT_SIGNAL_MODE    << RTC_Counter_COUNT_SHIFT))           |\
         ((uint32)(RTC_Counter_TC_RELOAD_SIGNAL_MODE   << RTC_Counter_RELOAD_SHIFT))          |\
         ((uint32)(RTC_Counter_TC_STOP_SIGNAL_MODE     << RTC_Counter_STOP_SHIFT))            |\
         ((uint32)(RTC_Counter_TC_START_SIGNAL_MODE    << RTC_Counter_START_SHIFT)))
        
#define RTC_Counter_TIMER_UPDOWN_CNT_USED                                                          \
                ((RTC_Counter__COUNT_UPDOWN0 == RTC_Counter_TC_COUNTER_MODE)                  ||\
                 (RTC_Counter__COUNT_UPDOWN1 == RTC_Counter_TC_COUNTER_MODE))

#define RTC_Counter_PWM_UPDOWN_CNT_USED                                                            \
                ((RTC_Counter__CENTER == RTC_Counter_PWM_ALIGN)                               ||\
                 (RTC_Counter__ASYMMETRIC == RTC_Counter_PWM_ALIGN))               
        
#define RTC_Counter_PWM_PR_INIT_VALUE              (1u)
#define RTC_Counter_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_RTC_Counter_H */

/* [] END OF FILE */

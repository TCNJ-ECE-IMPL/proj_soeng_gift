/*******************************************************************************
* File Name: RTC_Counter.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the RTC_Counter
*  component
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

#include "RTC_Counter.h"

uint8 RTC_Counter_initVar = 0u;


/*******************************************************************************
* Function Name: RTC_Counter_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default RTC_Counter configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (RTC_Counter__QUAD == RTC_Counter_CONFIG)
        RTC_Counter_CONTROL_REG = RTC_Counter_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        RTC_Counter_TRIG_CONTROL1_REG  = RTC_Counter_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        RTC_Counter_SetInterruptMode(RTC_Counter_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        RTC_Counter_SetCounterMode(RTC_Counter_COUNT_DOWN);
        RTC_Counter_WritePeriod(RTC_Counter_QUAD_PERIOD_INIT_VALUE);
        RTC_Counter_WriteCounter(RTC_Counter_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (RTC_Counter__QUAD == RTC_Counter_CONFIG) */

    #if (RTC_Counter__TIMER == RTC_Counter_CONFIG)
        RTC_Counter_CONTROL_REG = RTC_Counter_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        RTC_Counter_TRIG_CONTROL1_REG  = RTC_Counter_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        RTC_Counter_SetInterruptMode(RTC_Counter_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        RTC_Counter_WritePeriod(RTC_Counter_TC_PERIOD_VALUE );

        #if (RTC_Counter__COMPARE == RTC_Counter_TC_COMP_CAP_MODE)
            RTC_Counter_WriteCompare(RTC_Counter_TC_COMPARE_VALUE);

            #if (1u == RTC_Counter_TC_COMPARE_SWAP)
                RTC_Counter_SetCompareSwap(1u);
                RTC_Counter_WriteCompareBuf(RTC_Counter_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == RTC_Counter_TC_COMPARE_SWAP) */
        #endif  /* (RTC_Counter__COMPARE == RTC_Counter_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (RTC_Counter_CY_TCPWM_V2 && RTC_Counter_TIMER_UPDOWN_CNT_USED && !RTC_Counter_CY_TCPWM_4000)
            RTC_Counter_WriteCounter(1u);
        #elif(RTC_Counter__COUNT_DOWN == RTC_Counter_TC_COUNTER_MODE)
            RTC_Counter_WriteCounter(RTC_Counter_TC_PERIOD_VALUE);
        #else
            RTC_Counter_WriteCounter(0u);
        #endif /* (RTC_Counter_CY_TCPWM_V2 && RTC_Counter_TIMER_UPDOWN_CNT_USED && !RTC_Counter_CY_TCPWM_4000) */
    #endif  /* (RTC_Counter__TIMER == RTC_Counter_CONFIG) */

    #if (RTC_Counter__PWM_SEL == RTC_Counter_CONFIG)
        RTC_Counter_CONTROL_REG = RTC_Counter_CTRL_PWM_BASE_CONFIG;

        #if (RTC_Counter__PWM_PR == RTC_Counter_PWM_MODE)
            RTC_Counter_CONTROL_REG |= RTC_Counter_CTRL_PWM_RUN_MODE;
            RTC_Counter_WriteCounter(RTC_Counter_PWM_PR_INIT_VALUE);
        #else
            RTC_Counter_CONTROL_REG |= RTC_Counter_CTRL_PWM_ALIGN | RTC_Counter_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (RTC_Counter_CY_TCPWM_V2 && RTC_Counter_PWM_UPDOWN_CNT_USED && !RTC_Counter_CY_TCPWM_4000)
                RTC_Counter_WriteCounter(1u);
            #elif (RTC_Counter__RIGHT == RTC_Counter_PWM_ALIGN)
                RTC_Counter_WriteCounter(RTC_Counter_PWM_PERIOD_VALUE);
            #else 
                RTC_Counter_WriteCounter(0u);
            #endif  /* (RTC_Counter_CY_TCPWM_V2 && RTC_Counter_PWM_UPDOWN_CNT_USED && !RTC_Counter_CY_TCPWM_4000) */
        #endif  /* (RTC_Counter__PWM_PR == RTC_Counter_PWM_MODE) */

        #if (RTC_Counter__PWM_DT == RTC_Counter_PWM_MODE)
            RTC_Counter_CONTROL_REG |= RTC_Counter_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (RTC_Counter__PWM_DT == RTC_Counter_PWM_MODE) */

        #if (RTC_Counter__PWM == RTC_Counter_PWM_MODE)
            RTC_Counter_CONTROL_REG |= RTC_Counter_CTRL_PWM_PRESCALER;
        #endif  /* (RTC_Counter__PWM == RTC_Counter_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        RTC_Counter_TRIG_CONTROL1_REG  = RTC_Counter_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        RTC_Counter_SetInterruptMode(RTC_Counter_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (RTC_Counter__PWM_PR == RTC_Counter_PWM_MODE)
            RTC_Counter_TRIG_CONTROL2_REG =
                    (RTC_Counter_CC_MATCH_NO_CHANGE    |
                    RTC_Counter_OVERLOW_NO_CHANGE      |
                    RTC_Counter_UNDERFLOW_NO_CHANGE);
        #else
            #if (RTC_Counter__LEFT == RTC_Counter_PWM_ALIGN)
                RTC_Counter_TRIG_CONTROL2_REG = RTC_Counter_PWM_MODE_LEFT;
            #endif  /* ( RTC_Counter_PWM_LEFT == RTC_Counter_PWM_ALIGN) */

            #if (RTC_Counter__RIGHT == RTC_Counter_PWM_ALIGN)
                RTC_Counter_TRIG_CONTROL2_REG = RTC_Counter_PWM_MODE_RIGHT;
            #endif  /* ( RTC_Counter_PWM_RIGHT == RTC_Counter_PWM_ALIGN) */

            #if (RTC_Counter__CENTER == RTC_Counter_PWM_ALIGN)
                RTC_Counter_TRIG_CONTROL2_REG = RTC_Counter_PWM_MODE_CENTER;
            #endif  /* ( RTC_Counter_PWM_CENTER == RTC_Counter_PWM_ALIGN) */

            #if (RTC_Counter__ASYMMETRIC == RTC_Counter_PWM_ALIGN)
                RTC_Counter_TRIG_CONTROL2_REG = RTC_Counter_PWM_MODE_ASYM;
            #endif  /* (RTC_Counter__ASYMMETRIC == RTC_Counter_PWM_ALIGN) */
        #endif  /* (RTC_Counter__PWM_PR == RTC_Counter_PWM_MODE) */

        /* Set other values from customizer */
        RTC_Counter_WritePeriod(RTC_Counter_PWM_PERIOD_VALUE );
        RTC_Counter_WriteCompare(RTC_Counter_PWM_COMPARE_VALUE);

        #if (1u == RTC_Counter_PWM_COMPARE_SWAP)
            RTC_Counter_SetCompareSwap(1u);
            RTC_Counter_WriteCompareBuf(RTC_Counter_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == RTC_Counter_PWM_COMPARE_SWAP) */

        #if (1u == RTC_Counter_PWM_PERIOD_SWAP)
            RTC_Counter_SetPeriodSwap(1u);
            RTC_Counter_WritePeriodBuf(RTC_Counter_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == RTC_Counter_PWM_PERIOD_SWAP) */
    #endif  /* (RTC_Counter__PWM_SEL == RTC_Counter_CONFIG) */
    
}


/*******************************************************************************
* Function Name: RTC_Counter_Enable
********************************************************************************
*
* Summary:
*  Enables the RTC_Counter.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    RTC_Counter_BLOCK_CONTROL_REG |= RTC_Counter_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (RTC_Counter__PWM_SEL == RTC_Counter_CONFIG)
        #if (0u == RTC_Counter_PWM_START_SIGNAL_PRESENT)
            RTC_Counter_TriggerCommand(RTC_Counter_MASK, RTC_Counter_CMD_START);
        #endif /* (0u == RTC_Counter_PWM_START_SIGNAL_PRESENT) */
    #endif /* (RTC_Counter__PWM_SEL == RTC_Counter_CONFIG) */

    #if (RTC_Counter__TIMER == RTC_Counter_CONFIG)
        #if (0u == RTC_Counter_TC_START_SIGNAL_PRESENT)
            RTC_Counter_TriggerCommand(RTC_Counter_MASK, RTC_Counter_CMD_START);
        #endif /* (0u == RTC_Counter_TC_START_SIGNAL_PRESENT) */
    #endif /* (RTC_Counter__TIMER == RTC_Counter_CONFIG) */
    
    #if (RTC_Counter__QUAD == RTC_Counter_CONFIG)
        #if (0u != RTC_Counter_QUAD_AUTO_START)
            RTC_Counter_TriggerCommand(RTC_Counter_MASK, RTC_Counter_CMD_RELOAD);
        #endif /* (0u != RTC_Counter_QUAD_AUTO_START) */
    #endif  /* (RTC_Counter__QUAD == RTC_Counter_CONFIG) */
}


/*******************************************************************************
* Function Name: RTC_Counter_Start
********************************************************************************
*
* Summary:
*  Initializes the RTC_Counter with default customizer
*  values when called the first time and enables the RTC_Counter.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  RTC_Counter_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time RTC_Counter_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the RTC_Counter_Start() routine.
*
*******************************************************************************/
void RTC_Counter_Start(void)
{
    if (0u == RTC_Counter_initVar)
    {
        RTC_Counter_Init();
        RTC_Counter_initVar = 1u;
    }

    RTC_Counter_Enable();
}


/*******************************************************************************
* Function Name: RTC_Counter_Stop
********************************************************************************
*
* Summary:
*  Disables the RTC_Counter.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_BLOCK_CONTROL_REG &= (uint32)~RTC_Counter_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the RTC_Counter. This function is used when
*  configured as a generic RTC_Counter and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the RTC_Counter to operate in
*   Values:
*   - RTC_Counter_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - RTC_Counter_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - RTC_Counter_MODE_QUAD - Quadrature decoder
*         - RTC_Counter_MODE_PWM - PWM
*         - RTC_Counter_MODE_PWM_DT - PWM with dead time
*         - RTC_Counter_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_MODE_MASK;
    RTC_Counter_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - RTC_Counter_MODE_X1 - Counts on phi 1 rising
*         - RTC_Counter_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - RTC_Counter_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_QUAD_MODE_MASK;
    RTC_Counter_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - RTC_Counter_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - RTC_Counter_PRESCALE_DIVBY2    - Divide by 2
*         - RTC_Counter_PRESCALE_DIVBY4    - Divide by 4
*         - RTC_Counter_PRESCALE_DIVBY8    - Divide by 8
*         - RTC_Counter_PRESCALE_DIVBY16   - Divide by 16
*         - RTC_Counter_PRESCALE_DIVBY32   - Divide by 32
*         - RTC_Counter_PRESCALE_DIVBY64   - Divide by 64
*         - RTC_Counter_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_PRESCALER_MASK;
    RTC_Counter_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the RTC_Counter runs
*  continuously or stops when terminal count is reached.  By default the
*  RTC_Counter operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_ONESHOT_MASK;
    RTC_Counter_CONTROL_REG |= ((uint32)((oneShotEnable & RTC_Counter_1BIT_MASK) <<
                                                               RTC_Counter_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPWMMode(uint32 modeMask)
{
    RTC_Counter_TRIG_CONTROL2_REG = (modeMask & RTC_Counter_6BIT_MASK);
}



/*******************************************************************************
* Function Name: RTC_Counter_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_PWM_SYNC_KILL_MASK;
    RTC_Counter_CONTROL_REG |= ((uint32)((syncKillEnable & RTC_Counter_1BIT_MASK)  <<
                                               RTC_Counter_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_PWM_STOP_KILL_MASK;
    RTC_Counter_CONTROL_REG |= ((uint32)((stopOnKillEnable & RTC_Counter_1BIT_MASK)  <<
                                                         RTC_Counter_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_PRESCALER_MASK;
    RTC_Counter_CONTROL_REG |= ((uint32)((deadTime & RTC_Counter_8BIT_MASK) <<
                                                          RTC_Counter_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - RTC_Counter_INVERT_LINE   - Inverts the line output
*         - RTC_Counter_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_INV_OUT_MASK;
    RTC_Counter_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: RTC_Counter_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_WriteCounter(uint32 count)
{
    RTC_Counter_COUNTER_REG = (count & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 RTC_Counter_ReadCounter(void)
{
    return (RTC_Counter_COUNTER_REG & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - RTC_Counter_COUNT_UP       - Counts up
*     - RTC_Counter_COUNT_DOWN     - Counts down
*     - RTC_Counter_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - RTC_Counter_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_UPDOWN_MASK;
    RTC_Counter_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_WritePeriod(uint32 period)
{
    RTC_Counter_PERIOD_REG = (period & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 RTC_Counter_ReadPeriod(void)
{
    return (RTC_Counter_PERIOD_REG & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_RELOAD_CC_MASK;
    RTC_Counter_CONTROL_REG |= (swapEnable & RTC_Counter_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_WritePeriodBuf(uint32 periodBuf)
{
    RTC_Counter_PERIOD_BUF_REG = (periodBuf & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 RTC_Counter_ReadPeriodBuf(void)
{
    return (RTC_Counter_PERIOD_BUF_REG & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_CONTROL_REG &= (uint32)~RTC_Counter_RELOAD_PERIOD_MASK;
    RTC_Counter_CONTROL_REG |= ((uint32)((swapEnable & RTC_Counter_1BIT_MASK) <<
                                                            RTC_Counter_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void RTC_Counter_WriteCompare(uint32 compare)
{
    #if (RTC_Counter_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (RTC_Counter_CY_TCPWM_4000) */

    #if (RTC_Counter_CY_TCPWM_4000)
        currentMode = ((RTC_Counter_CONTROL_REG & RTC_Counter_UPDOWN_MASK) >> RTC_Counter_UPDOWN_SHIFT);

        if (((uint32)RTC_Counter__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)RTC_Counter__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (RTC_Counter_CY_TCPWM_4000) */
    
    RTC_Counter_COMP_CAP_REG = (compare & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 RTC_Counter_ReadCompare(void)
{
    #if (RTC_Counter_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (RTC_Counter_CY_TCPWM_4000) */

    #if (RTC_Counter_CY_TCPWM_4000)
        currentMode = ((RTC_Counter_CONTROL_REG & RTC_Counter_UPDOWN_MASK) >> RTC_Counter_UPDOWN_SHIFT);
        
        regVal = RTC_Counter_COMP_CAP_REG;
        
        if (((uint32)RTC_Counter__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)RTC_Counter__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & RTC_Counter_16BIT_MASK);
    #else
        return (RTC_Counter_COMP_CAP_REG & RTC_Counter_16BIT_MASK);
    #endif /* (RTC_Counter_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: RTC_Counter_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void RTC_Counter_WriteCompareBuf(uint32 compareBuf)
{
    #if (RTC_Counter_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (RTC_Counter_CY_TCPWM_4000) */

    #if (RTC_Counter_CY_TCPWM_4000)
        currentMode = ((RTC_Counter_CONTROL_REG & RTC_Counter_UPDOWN_MASK) >> RTC_Counter_UPDOWN_SHIFT);

        if (((uint32)RTC_Counter__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)RTC_Counter__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (RTC_Counter_CY_TCPWM_4000) */
    
    RTC_Counter_COMP_CAP_BUF_REG = (compareBuf & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 RTC_Counter_ReadCompareBuf(void)
{
    #if (RTC_Counter_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (RTC_Counter_CY_TCPWM_4000) */

    #if (RTC_Counter_CY_TCPWM_4000)
        currentMode = ((RTC_Counter_CONTROL_REG & RTC_Counter_UPDOWN_MASK) >> RTC_Counter_UPDOWN_SHIFT);

        regVal = RTC_Counter_COMP_CAP_BUF_REG;
        
        if (((uint32)RTC_Counter__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)RTC_Counter__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & RTC_Counter_16BIT_MASK);
    #else
        return (RTC_Counter_COMP_CAP_BUF_REG & RTC_Counter_16BIT_MASK);
    #endif /* (RTC_Counter_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 RTC_Counter_ReadCapture(void)
{
    return (RTC_Counter_COMP_CAP_REG & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 RTC_Counter_ReadCaptureBuf(void)
{
    return (RTC_Counter_COMP_CAP_BUF_REG & RTC_Counter_16BIT_MASK);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - RTC_Counter_TRIG_LEVEL     - Level
*     - RTC_Counter_TRIG_RISING    - Rising edge
*     - RTC_Counter_TRIG_FALLING   - Falling edge
*     - RTC_Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_TRIG_CONTROL1_REG &= (uint32)~RTC_Counter_CAPTURE_MASK;
    RTC_Counter_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - RTC_Counter_TRIG_LEVEL     - Level
*     - RTC_Counter_TRIG_RISING    - Rising edge
*     - RTC_Counter_TRIG_FALLING   - Falling edge
*     - RTC_Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_TRIG_CONTROL1_REG &= (uint32)~RTC_Counter_RELOAD_MASK;
    RTC_Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << RTC_Counter_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - RTC_Counter_TRIG_LEVEL     - Level
*     - RTC_Counter_TRIG_RISING    - Rising edge
*     - RTC_Counter_TRIG_FALLING   - Falling edge
*     - RTC_Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_TRIG_CONTROL1_REG &= (uint32)~RTC_Counter_START_MASK;
    RTC_Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << RTC_Counter_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - RTC_Counter_TRIG_LEVEL     - Level
*     - RTC_Counter_TRIG_RISING    - Rising edge
*     - RTC_Counter_TRIG_FALLING   - Falling edge
*     - RTC_Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_TRIG_CONTROL1_REG &= (uint32)~RTC_Counter_STOP_MASK;
    RTC_Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << RTC_Counter_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - RTC_Counter_TRIG_LEVEL     - Level
*     - RTC_Counter_TRIG_RISING    - Rising edge
*     - RTC_Counter_TRIG_FALLING   - Falling edge
*     - RTC_Counter_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_TRIG_CONTROL1_REG &= (uint32)~RTC_Counter_COUNT_MASK;
    RTC_Counter_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << RTC_Counter_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - RTC_Counter_CMD_CAPTURE    - Trigger Capture/Switch command
*     - RTC_Counter_CMD_RELOAD     - Trigger Reload/Index command
*     - RTC_Counter_CMD_STOP       - Trigger Stop/Kill command
*     - RTC_Counter_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    RTC_Counter_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RTC_Counter_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the RTC_Counter.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - RTC_Counter_STATUS_DOWN    - Set if counting down
*     - RTC_Counter_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 RTC_Counter_ReadStatus(void)
{
    return ((RTC_Counter_STATUS_REG >> RTC_Counter_RUNNING_STATUS_SHIFT) |
            (RTC_Counter_STATUS_REG & RTC_Counter_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: RTC_Counter_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - RTC_Counter_INTR_MASK_TC       - Terminal count mask
*     - RTC_Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetInterruptMode(uint32 interruptMask)
{
    RTC_Counter_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: RTC_Counter_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - RTC_Counter_INTR_MASK_TC       - Terminal count mask
*     - RTC_Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 RTC_Counter_GetInterruptSourceMasked(void)
{
    return (RTC_Counter_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: RTC_Counter_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - RTC_Counter_INTR_MASK_TC       - Terminal count mask
*     - RTC_Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 RTC_Counter_GetInterruptSource(void)
{
    return (RTC_Counter_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: RTC_Counter_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - RTC_Counter_INTR_MASK_TC       - Terminal count mask
*     - RTC_Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_ClearInterrupt(uint32 interruptMask)
{
    RTC_Counter_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: RTC_Counter_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - RTC_Counter_INTR_MASK_TC       - Terminal count mask
*     - RTC_Counter_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void RTC_Counter_SetInterrupt(uint32 interruptMask)
{
    RTC_Counter_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */

#include <stdint.h>
#include <stdio.h>
#include "project.h"
#include "led_mat_displ.h"
#include "lf_counter_callback.h"
#include "tcnj_config.h"

extern volatile int clk_hrs;
extern volatile int clk_min;
extern volatile int clk_sec;

extern volatile int count_ms;
extern volatile int lf_ticks;

extern volatile int cur_sound_idx;                          // set to zero to restart sound

#define LF_INT_FREQ (32)
//#define BANNER_TIME_IDX ( (int)(sizeof(BANNER_STR)-4-1) )   // -4 account for time chars, -1 to account for NULL

// Called exactly 32 times/sec
void lf_counter_callback(void)
{
    static char *banner_string[] = {
        "Purr  Happy Anniversary Dorie & Ron!! \03\03\03\03, I&L&E&J 7/15/22        ",
        "Purr  \03\03\03 At Last \03\03\03        ",
        "Purr  Love you every minute!        ",
        "Purr  Time for love \01        ",
        "Purr  All you need is love!        ",
        "Purr  I am my beloved's ...        ",
        "Purr  ... and my beloved is mine.        ",
        "Purr  \03\03\03 Always time for you \03\03\03        ",
        "Purr  Love gets stronger as time goes by        ",
        "Purr  \03\03\03 Side by side we see our future together \03\03\03        ",
        "Purr  Time together is precious!        ",
        "Purr  Hours feel like minutes when we have fun together \01\01        "
    };
//    static char *banner_string[] = {
//        "Purr  At Last        ",
//        "Purr  Just in time        ",
//        "Purr  Love you every minute!        ",
//        "Purr  Time for love         ",
//        "Purr  All you need is love!        ",
//        "Purr  I am my beloved's ...        ",
//        "Purr  ... and my beloved is mine.        ",
//        "Purr  Always time for you         ",
//        "Purr  Love gets stronger as time goes by        ",
//        "Purr  Side by side we see our future together         ",
//        "Purr  Time together is precious!        ",
//        "Purr  Hours feel like minutes when we have fun together         "
//    };
    char temp_str[128];
    
    // local state machines
    static int  lf_count            = 0;
    static int  splashing_flag      = 1;
    static int  animation_count     = 0;
    static int  banner_shift_idx    = 0; 
    static int  frac_shift_idx      = 0;

    // Static allocation of big arrays to avoid surprise stack over
    static uint8_t     str_pels[5][8];
    static uint16_t    disp_cmds[8][4];
    static char        time_str[5];
    
    int         hrs_button;
    int         min_button;
    int         eith_btn;
    int         both_btn;
    
    // Output pin for helping debug
    FRAME_SYNC_Write(~FRAME_SYNC_Read());

    hrs_button = !SW_HRS_Read();
    min_button = !SW_MIN_Read();
    eith_btn   = hrs_button || min_button;
    both_btn   = hrs_button && min_button;

    ++lf_ticks;
    count_ms += 31; // actually 31.25
    ++lf_count;
    if (lf_count == LF_INT_FREQ)
    {
        ++clk_sec;
        lf_count = 0;
    }
    
    int msg_idx;
    int banner_time_idx;
    
    msg_idx = clk_hrs % 12;
    strcpy(temp_str, banner_string[msg_idx]);
    banner_time_idx = strlen(temp_str)-4;
    // Not necessary to do every time, but could be long, so better see any issues all the time!
    sprintf(temp_str+banner_time_idx, "%2d%02d", clk_hrs, clk_min);

    if (splashing_flag)
    {
        if (animation_count < ROAR_FRAMES)
        {
            banner_shift_idx    = 0;    // reset for next banner
            frac_shift_idx      = 0;    // reset for next banner
            // First ROAR, with vibration
            get_pels_from_str( "Purr ", 0, 5, str_pels );
            get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, -(animation_count&1) );
            put_cmds_to_disp( disp_cmds );
            ++animation_count;
        }
        else if (animation_count == ROAR_FRAMES)
        // Then banner display
        {
            if (banner_shift_idx < banner_time_idx) // -4 to keep the end chars from scrolling off, -1 to ignore count for NULL
            {
                // Get pixels for the next 5 characters
                get_pels_from_str( temp_str, banner_shift_idx, 5, str_pels );
                get_disp_cmds_frac_shift( str_pels, frac_shift_idx, disp_cmds, 0 );
                put_cmds_to_disp( disp_cmds );
                
                if (eith_btn)
                    frac_shift_idx += 4;
                else
                    ++frac_shift_idx;
                
                if (frac_shift_idx >= 8)
                {
                    frac_shift_idx = 0;
                    ++banner_shift_idx;
                }
            }
            else
            {
                animation_count = 0;
                splashing_flag = 0;
            }
        }
    } // end if (splashing flag)
    else
    {
        sprintf( time_str, "%2d%02d", clk_hrs, clk_min );
        get_pels_from_str( time_str, 0, 5, str_pels );
        
        // uint8_t str_pels[5][8];
        // Want to right shift the words in [2][] and [3][] by 1, to make space for colon!
        for (int digit_idx = 2; digit_idx <= 3; ++digit_idx)
        {
            for (int row_idx = 0; row_idx < 8; ++row_idx)
            {
                str_pels[digit_idx][row_idx] = str_pels[digit_idx][row_idx] >> 2;
            }
        }
        
        // Blink/draw colon
//        if (clk_sec & 1)
        if (clk_sec & 0)
        {
            str_pels[1][1] &= 0xFE;
            str_pels[1][2] &= 0xFE;
            str_pels[1][4] &= 0xFE;
            str_pels[1][5] &= 0xFE;
            str_pels[2][1] &= 0x7F;
            str_pels[2][2] &= 0x7F;
            str_pels[2][4] &= 0x7F;
            str_pels[2][5] &= 0x7F;
        }
        else
        {
            str_pels[1][1] |= 0x01;
            str_pels[1][2] |= 0x01;
            str_pels[1][4] |= 0x01;
            str_pels[1][5] |= 0x01;
            str_pels[2][1] |= 0x80;
            str_pels[2][2] |= 0x80;
            str_pels[2][4] |= 0x80;
            str_pels[2][5] |= 0x80;
        }        

        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, 0 );
        put_cmds_to_disp( disp_cmds );
    }
    
    if (clk_sec == 60)
    {
        clk_sec = 0;
        ++clk_min;

        if (clk_min >= 60)
        {
            cur_sound_idx   = 0;    // On the hour <for testing, on the minute>, restart the ROAR sound
            animation_count = 0;    // On the hour <for testing, on the minute>, do the banner display
            splashing_flag  = 1;    // On the hour <for testing, on the minute>, do the banner display

            clk_min = 0;
            if (clk_hrs >= 12)
                clk_hrs = 1;
            else
                clk_hrs++;
        }
    }
    if (both_btn)
    {
        cur_sound_idx   = 0;    // both buttons: restart the ROAR sound
        animation_count = 0;    // both buttons: do the banner display
        splashing_flag  = 1;    // both buttons: do the banner display
    }
}

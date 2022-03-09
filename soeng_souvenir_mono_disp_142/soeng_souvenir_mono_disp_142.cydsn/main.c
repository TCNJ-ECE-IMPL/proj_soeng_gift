#include <stdio.h>
#include "project.h"
#include "led_mat_displ.h"
#include "acrylic_led_strip.h"

volatile uint8_t  str_pels[5][8];
volatile uint16_t disp_cmds[8][4];
volatile int      splashing_flag = 1;

volatile int clk_hrs = 12;
volatile int clk_min = 0;
volatile int clk_sec = 0;

static volatile int count_ms = 0;
static volatile int lf_ticks = 0;
#define LF_INT_FREQ (32)
// Called exactly 32 times/sec
void lf_counter_callback(void)
{
    static int lf_count = 0;
    
    char time_str[5];
    
    FRAME_SYNC_Write(~FRAME_SYNC_Read());

    ++lf_ticks;
    count_ms += 31; // actually 31.25
    ++lf_count;
    if (lf_count == LF_INT_FREQ)
    {
        ++clk_sec;
        lf_count = 0;
    }
    
    if (splashing_flag)
    {
        put_cmds_to_disp( disp_cmds );
    }
    else
    {
        sprintf( time_str, "%2d%02d", clk_hrs, clk_min );
        get_pels_from_str( time_str, 0, 5, str_pels );
        
        if (clk_sec & 1)
            str_pels[0][0] &= 0x7F;
        else
            str_pels[0][0] |= 0x80;
            
        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, 0 );
        put_cmds_to_disp( disp_cmds );
    }
    
    if (clk_sec == 60)
    {
        clk_sec = 0;
        ++clk_min;

        if (clk_min == 60)
        {
            clk_min = 0;
            if (clk_hrs == 12)
                clk_hrs = 1;
            else
                clk_hrs++;
        }
    }
}

int main(void)
{
    int  prev_lf_ticks = 0;
    
    char the_string[]  = "ROAR   TCNJ Engineering -- Designing The Future!!!!    ";
    char full_string[sizeof(the_string) + 4];
    
//    uint16_t buf[8][4] = { 
//        { 0x01CC, 0x0155, 0x01AA, 0x01F0 },
//        { 0x02CC, 0x0255, 0x02AA, 0x02F0 },
//        { 0x03CC, 0x0355, 0x03AA, 0x03F0 },
//        { 0x04CC, 0x0455, 0x04AA, 0x04F0 },
//        { 0x05CC, 0x0555, 0x05AA, 0x05F0 },
//        { 0x06CC, 0x0655, 0x06AA, 0x06F0 },
//        { 0x07CC, 0x0755, 0x07AA, 0x07F0 },
//        { 0x08CC, 0x0855, 0x08AA, 0x08F0 }
//    };
    
    CyGlobalIntEnable;

    isr_1_Start();
    SPIM_Start();
    SPI_1_Start();
    PrISM_1_Start();
    Timer_1_Start();
    configure_max7219();
    
    uint8_t color_vals[3];
    
    // Do vibrating ROAR
    for (int loop = 0; loop < 66; ++loop)
    {
        get_pels_from_str( "ROAR ", 0, 5, str_pels );
        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, -(loop&1) );
        put_cmds_to_disp( disp_cmds );
        
        CyDelay(33);
        ms_to_colors( count_ms % 8000, color_vals );
        update_acrylic_leds( color_vals );
    }

    CySysWdtSetInterruptCallback(0, lf_counter_callback);
    CySysWdtEnableCounterIsr(0);

    sprintf(full_string, "%s%2d%02d", the_string, clk_hrs, clk_min);
    for (int str_idx = 0; str_idx < (int)strlen(full_string) - 4; ++str_idx)
    {
        for (int frac_shift = 0; frac_shift < 8; ++frac_shift)
        {
            // Get pixels for the next 5 characters
            get_pels_from_str( full_string, str_idx, 5, str_pels );
            get_disp_cmds_frac_shift( str_pels, frac_shift, disp_cmds, 0 );
            // put_cmds_to_disp( disp_cmds );
            
            if (SW_HRS_Read() && SW_MIN_Read())
            {
                // If neither button down wait for tick
                while (lf_ticks == prev_lf_ticks) {}
                prev_lf_ticks = lf_ticks;
            }
            else
            {
                // otherwise fast-forward
                CyDelay(10);
            }
            
            ms_to_colors( count_ms % 8000, color_vals );
            update_acrylic_leds( color_vals );
        }
    }
    splashing_flag = 0;
    while (1)
    {
        static int sw_hrs_down = 0;
        static int sw_min_down = 0;
        
//        CyDelay(33);
        // Wait for tick
        while (lf_ticks == prev_lf_ticks) {}
        prev_lf_ticks = lf_ticks;

        ms_to_colors( count_ms % 8000, color_vals );
        update_acrylic_leds( color_vals );
        
//        CySysWdtDisableCounterIsr(0);
        //isr_1_hz_Disable();
        if (SW_HRS_Read())
        {
            sw_hrs_down = 0;    // button is up
        }
        else
        {
            if (sw_hrs_down == 0)
            {
                // Just went from up to down, increment
                if (clk_hrs == 12)
                    clk_hrs = 1;
                else
                    ++clk_hrs;
            }
            else if (sw_hrs_down > 30)
            {
                if ((sw_hrs_down % 8) == 0)
                {
                    if (clk_hrs == 12)
                        clk_hrs = 1;
                    else
                        ++clk_hrs;
                }
            }
            ++sw_hrs_down;
        }

        if (SW_MIN_Read())
        {
            sw_min_down = 0;    // button is up
        }
        else
        {
            if (sw_min_down == 0)
            {
                // Just went from up to down, increment
                if (clk_min == 59)
                    clk_min = 0;
                else
                    ++clk_min;
            }
            else if (sw_min_down > 30)
            {
                if ((sw_min_down % 4) == 0)
                {
                    if (clk_min == 59)
                        clk_min = 0;
                    else
                        ++clk_min;
                }
            }
            ++sw_min_down;
        }

//        char time_str[5];
//        sprintf( time_str, "%2d%02d", clk_hrs, clk_min );
//        get_pels_from_str( time_str, 0, 5, str_pels );
//        
//        if (clk_sec & 1)
//            str_pels[0][0] &= 0x7F;
//        else
//            str_pels[0][0] |= 0x80;
//            
//        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, 0 );
//        put_cmds_to_disp( disp_cmds );

//        CySysWdtEnableCounterIsr(0);
    } // while (1) for lights
}


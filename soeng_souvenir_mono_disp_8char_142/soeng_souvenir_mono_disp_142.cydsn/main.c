#include <stdio.h>
#include "project.h"
#include "led_mat_displ.h"
#include "acrylic_led_strip.h"
#include "lf_counter_callback.h"

volatile int clk_hrs = 12;
volatile int clk_min = 0;
volatile int clk_sec = 0;

volatile int count_ms = 0;
volatile int lf_ticks = 0;

int main(void)
{
    int  prev_lf_ticks = 0;
        
    CyGlobalIntEnable;

    //isr_1_SetPriority(2);   // Make audio interrupt higher priority
    isr_1_Start();
    SPIM_Start();
    SPI_1_Start();
    PrISM_1_Start();
    Timer_1_Start();
    configure_max7219();
    
    uint8_t color_vals[3];
    
    CySysWdtSetInterruptCallback(0, lf_counter_callback);
    CySysWdtEnableCounterIsr(0);

    while (1)
    {
        static int sw_hrs_down = 0;
        static int sw_min_down = 0;
        
        // Wait for tick
        while (lf_ticks == prev_lf_ticks) {}
        prev_lf_ticks = lf_ticks;

        ms_to_colors( count_ms % 8000, color_vals );
        update_acrylic_leds( color_vals );
        
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

    } // while (1) for lights and time set
}


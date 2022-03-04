#include <stdio.h>
#include "project.h"
#include "led_mat_displ.h"

#define C3 (61156)
#define CS3 (57724)
#define D3 (54484)
#define DS3 (51426)
#define E3 (48540)
#define F3 (45815)
#define FS3 (43244)
#define G3 (40817)
#define GS3 (38526)
#define A3 (36364)
#define AS3 (34323)
#define B3 (32396)
#define C4 (30578)
#define CS4 (28862)
#define D4 (27242)
#define DS4 (25713)
#define E4 (24270)
#define F4 (22908)
#define FS4 (21622)
#define G4 (20408)
#define GS4 (19263)
#define A4 (18182)
#define AS4 (17161)
#define B4 (16198)
#define Z   (2)

//High/Low Pattern
//================
//0: 220-380  ns / 580-1000 ns
//1: 580-1000 ns / 220-420  ns
//
//RESET > 280 us!

// 0 is represented by 1 for 0.4 us, then 0 for 0.85 us
// 1 is represented by 1 for 0.8 us, then 0 for 0.45 us
// 10 'chips', at 8 MHz, 125 ns/chip, total is 1250 ns, target 1250 ns (but within 600 ns is OK)
//
//       2 chips = 250 ns
//       3 chips = 375 ns
//       4 chips = 500 ns
//       5 chips = 625 ns
//       6 chips = 750 ns
//       7 chips = 875 ns
//       8 chips = 1000 ns

//
//  So do    3 +  7 = 11 1000 0000 = 0x380 
//  or       7 +  3 = 11 1111 1000 = 0x3F8

#define ZER_BIT (0x380)
#define ONE_BIT (0x3F8)

uint16_t chimes[] = { 
    E4, GS4, FS4, B3, B3,
    E4, FS4, GS4, E4, E4,
    GS4, E4, FS4, B3, B3,
    B3, FS4, GS4, E4, E4,
    Z,  Z,   Z,   Z,  E3, E3, E3, Z, E3, E3, E3, Z, E3, E3, E3, Z};

volatile uint8_t  str_pels[5][8];
volatile uint16_t disp_cmds[8][4];
volatile int      splashing_flag = 1;

volatile int clk_hrs = 12;
volatile int clk_min = 0;
volatile int clk_sec = 0;

// ms is a value from 0 to 7999 (8 second cycle)
void ms_to_colors( int ms, uint8_t *color_vals )
{
    int r;
    int g;
    int b;
    
    int fade;
    
    // cross from yellow to blue, then blue for 3 secs, the cross from blue to yellow, then yellow for 3 secs
    if (ms < 1000)
    {
        fade = 256 * ms / 1000;
        
        b = fade;
        r = g = 255-fade;
    }
    else if (ms < 4000)
    {
        b = 255;
        r = g = 0;
    }
    else if (ms < 5000)
    {
        fade = 256 * (ms-4000) / 1000;
        b = 255-fade;
        r = g = fade;
    }
    else
    {
        b = 0;
        r = g = 255;
    }
    // order is GRB!
    color_vals[0] = g;
    color_vals[1] = r;
    color_vals[2] = b;
}

#define LED_STR_LEN (5)
void update_acrylic_leds( uint8_t vals[3] )
{
    // First send reset ... at least 50 usec
    //FRAME_SYNC_Write(1);
    for (int rst_cnt = 0; rst_cnt < 300; ++rst_cnt)
        SPI_1_SpiUartWriteTxData(0);
    //FRAME_SYNC_Write(0);

    // 24-bit format
    //  G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
    for (int col = 0; col < LED_STR_LEN; ++col)
    {
        for (int clr = 0; clr < 3; ++clr)
        {
            int the_val = vals[clr];
            
            // each bit is a 2-byte pattern for PWM'ing
            for (int bit_idx = 7; bit_idx >= 0; --bit_idx)            
                if ((the_val >> bit_idx) & 1)
                    // send ONE_BIT
                    SPI_1_SpiUartWriteTxData(ONE_BIT);
                else
                    // send ZER_BIT
                    SPI_1_SpiUartWriteTxData(ZER_BIT);
        }
    }
}


void lf_counter_callback(void)
{
    char time_str[5];
    
    FRAME_SYNC_Write(~FRAME_SYNC_Read());
    
    ++clk_sec;
    
    if (!splashing_flag)
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

        sprintf( time_str, "%2d%02d", clk_hrs, clk_min );
        get_pels_from_str( time_str, 0, 5, str_pels );
        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, 0 );
        put_cmds_to_disp( disp_cmds );
    }
    CyDelay(1);
}

int main(void)
{
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
    

    int count_ms=0;
    
    CyGlobalIntEnable;

    isr_1_Start();
    // isr_1_hz_Start();    // Now using the internal low frequency counter!
    // RTC_Counter_Start();
    SPIM_Start();
    SPI_1_Start();
    PrISM_1_Start();
    Timer_1_Start();
    CySysWdtSetInterruptCallback(0, lf_counter_callback);
    CySysWdtEnableCounterIsr(0);
    configure_max7219();
    
    uint8_t color_vals[3];
    
    // Do vibrating ROAR
    for (int loop = 0; loop < 66; ++loop)
    {
        get_pels_from_str( "ROAR ", 0, 5, str_pels );
        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, -(loop&1) );
        put_cmds_to_disp( disp_cmds );
        
        CyDelay(33);
        count_ms += 33;
        ms_to_colors( count_ms % 8000, color_vals );
        update_acrylic_leds( color_vals );
    }

    sprintf(full_string, "%s%2d%02d", the_string, clk_hrs, clk_min);
    for (int str_idx = 0; str_idx < (int)strlen(full_string) - 4; ++str_idx)
    {
        for (int frac_shift = 0; frac_shift < 8; ++frac_shift)
        {
            // Get pixels for the next 5 characters
            get_pels_from_str( full_string, str_idx, 5, str_pels );
            
            get_disp_cmds_frac_shift( str_pels, frac_shift, disp_cmds, 0 );

            put_cmds_to_disp( disp_cmds );
            
            if (SW_HRS_Read() && SW_MIN_Read())
                // Fast forward if either button down
                CyDelay(33);
            else
                CyDelay(4);
                
            count_ms += 33;
            ms_to_colors( count_ms % 8000, color_vals );
            update_acrylic_leds( color_vals );
        }
    }
    splashing_flag = 0;
    while (1)
    {
        static int sw_hrs_down = 0;
        static int sw_min_down = 0;
        
        CyDelay(33);
        count_ms += 33;
        ms_to_colors( count_ms % 8000, color_vals );
        update_acrylic_leds( color_vals );
        
        CySysWdtDisableCounterIsr(0);
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

        char time_str[5];
        sprintf( time_str, "%2d%02d", clk_hrs, clk_min );
        get_pels_from_str( time_str, 0, 5, str_pels );
        
        if (clk_sec & 1)
            str_pels[0][0] &= 0x7F;
        else
            str_pels[0][0] |= 0x80;
            
        get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, 0 );
        put_cmds_to_disp( disp_cmds );

        CySysWdtEnableCounterIsr(0);
 //       isr_1_hz_Enable();
    } // while (1) for lights
}


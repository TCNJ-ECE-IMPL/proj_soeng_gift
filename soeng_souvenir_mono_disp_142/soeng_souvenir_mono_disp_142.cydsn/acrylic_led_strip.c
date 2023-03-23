#include "project.h"
#include "acrylic_led_strip.h"

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

// ms is a value from 0 to 8191 (~8 second cycle)
void ms_to_colors( int ms, uint8_t *color_vals )
{
    int r;
    int g;
    int b;
    
    int fade;
    
    // cross from yellow to blue, then blue for 3 secs, the cross from blue to yellow, then yellow for 3 secs
    if (ms < 512)
    {
        fade = 256 * ms / 512;
        
        b = fade;
        r = g = 255-fade;
    }
    else if (ms < 32768)
    {
        b = 255;
        r = g = 0;
    }
    else if (ms < 32768+512)
    {
        fade = 256 * (ms-32768) / 512;
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



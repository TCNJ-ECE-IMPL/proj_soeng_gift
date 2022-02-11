#include "project.h"
#include "fonts.h"

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

void replicate_cmd( uint16_t the_cmd )
{
    uint16_t repl_cmds[4];
    
    for (int i = 0; i < 4; ++i)
        repl_cmds[i] = the_cmd;
        
    while (!(SPIM_ReadTxStatus() & SPIM_STS_SPI_IDLE)) {}
    SPIM_PutArray( repl_cmds, 4 );    // send 4 bytes, in one transaction

}

void configure_max7219()
{
    replicate_cmd(0x0B07); // There are 8 rows, and each row is like a "digit",
                              // so set Scan Limit register to 7, for 8 digits

    replicate_cmd(0x0900); // For raw, set Decode Mode to No Decode (0)

    replicate_cmd(0x0F00); // Set Display Test register to Normal Operation(0)
    
    replicate_cmd(0x0C01); // Set Shutdown Mode register for Normal Operation (1)

    replicate_cmd(0x0A01); // Set light intensity to low value
}


void char_to_pels( uint8_t the_char, uint8_t the_pels[8] )
{
    uint8_t transpose[8];
    
    for (int i = 0; i < 8; ++i)
        transpose[i] = fonts[the_char][i];

    // Transpose to normal
    for (int out_row_idx = 0; out_row_idx < 8; ++out_row_idx)
    {
        int pel_val = 0;
        for (int out_bit_idx = 7; out_bit_idx >= 0; --out_bit_idx)
        {
            int in_row_idx = 7 - out_bit_idx;
            pel_val |= (((transpose[in_row_idx] >> out_row_idx) & 1)<<out_bit_idx);
        }
        the_pels[out_row_idx] = pel_val;
    }
}

void get_pels_from_str( char *the_str, int str_idx, int num_chars, uint8_t str_pels[5][8] )
{
    
    for (int i = 0; i < num_chars; ++i)
        char_to_pels(the_str[str_idx+i], &str_pels[i][0] );   
}

void get_disp_cmds_frac_shift( uint8_t str_pels[5][8], int frac_shift, uint16_t disp_cmds[8][4], int vshift )
{
    for (int row_cnt = 0; row_cnt < 8; ++row_cnt)
    {
        uint64_t long_word=0;
        
        int raw_row_idx = row_cnt + vshift;
        
        int row_idx = raw_row_idx;
        
        if (row_idx < 0) row_idx = 0;
        if (row_idx > 7) row_idx = 7;
        
        for (int char_idx = 0; char_idx < 5; ++char_idx)
        {
            uint64_t the_pels = str_pels[char_idx][row_idx];
            uint64_t shift_amt = 8*(4-char_idx);
            
            long_word |= (the_pels << shift_amt);
        }
        
        if (raw_row_idx < 0) long_word = 0;
        if (raw_row_idx > 7) long_word = 0;
        
        for (int char_idx = 0; char_idx < 4; ++char_idx)
        {
            uint64_t bits_lsb = (4-char_idx)*8-frac_shift;
            uint32_t char_pels = (long_word >> bits_lsb) & 0xFF;
            
            disp_cmds[row_cnt][char_idx] = ((row_cnt+1) << 8) + char_pels;
        }
    }
}

void put_cmds_to_disp( uint16_t disp_cmds[8][4] )
{
    for (int row = 0; row < 8; ++row)
    {
        while (!(SPIM_ReadTxStatus() & SPIM_STS_SPI_IDLE)) {}
        SPIM_PutArray( &disp_cmds[row][0], 4 );    // send 4 bytes, in one transaction
    }
}

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
    FRAME_SYNC_Write(1);
    for (int rst_cnt = 0; rst_cnt < 300; ++rst_cnt)
        SPI_1_SpiUartWriteTxData(0);
    FRAME_SYNC_Write(0);

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

int main(void)
{
    char the_string[] = "ROAR   TCNJ Engineering -- Designing The Future!!!!    12OO";
    
    uint8_t str_pels[5][8];
    
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
    
    uint16_t disp_cmds[8][4];

    int count_ms=0;
    
    CyGlobalIntEnable;

    isr_1_Start();
    SPIM_Start();
    SPI_1_Start();
    PWM_1_Start();

    configure_max7219();
    
    while (1)
    {
        uint8_t color_vals[3];
        
        // Do dot matrix display
        for (int loop = 0; loop < 100; ++loop)
        {
            get_pels_from_str( "ROAR ", 0, 5, str_pels );
            get_disp_cmds_frac_shift( str_pels, 0, disp_cmds, -(loop&1) );
            put_cmds_to_disp( disp_cmds );
            
            CyDelay(33);
            count_ms += 33;
            ms_to_colors( count_ms % 8000, color_vals );
            update_acrylic_leds( color_vals );
        }
        
        for (int str_idx = 0; str_idx < (int)strlen(the_string) - 4; ++str_idx)
        {
            for (int frac_shift = 0; frac_shift < 8; ++frac_shift)
            {
                // Get pixels for the next 5 characters
                get_pels_from_str( the_string, str_idx, 5, str_pels );
                
                get_disp_cmds_frac_shift( str_pels, frac_shift, disp_cmds, 0 );

                put_cmds_to_disp( disp_cmds );
                
                CyDelay(33);
                count_ms += 33;
                ms_to_colors( count_ms % 8000, color_vals );
                update_acrylic_leds( color_vals );
            }
        }
        while (1)
        {
            CyDelay(33);
            count_ms += 33;
            ms_to_colors( count_ms % 8000, color_vals );
            update_acrylic_leds( color_vals );
        }
    }
}


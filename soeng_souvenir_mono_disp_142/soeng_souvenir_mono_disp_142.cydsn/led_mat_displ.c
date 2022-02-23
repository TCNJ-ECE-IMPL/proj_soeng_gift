#include <stdint.h>
#include "project.h"
#include "fonts.h"
#include "led_mat_displ.h"

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


void char_to_pels( uint8_t the_char, volatile uint8_t the_pels[8] )
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

void get_pels_from_str( char *the_str, int str_idx, int num_chars, volatile uint8_t str_pels[5][8] )
{
    
    for (int i = 0; i < num_chars; ++i)
        char_to_pels(the_str[str_idx+i], &str_pels[i][0] );   
}

void get_disp_cmds_frac_shift( volatile uint8_t str_pels[5][8], int frac_shift, volatile uint16_t disp_cmds[8][4], int vshift )
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

void put_cmds_to_disp( volatile uint16_t disp_cmds[8][4] )
{
    for (int row = 0; row < 8; ++row)
    {
        while (!(SPIM_ReadTxStatus() & SPIM_STS_SPI_IDLE)) {}
        SPIM_PutArray( &disp_cmds[row][0], 4 );    // send 4 bytes, in one transaction
    }
}


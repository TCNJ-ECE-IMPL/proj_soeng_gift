#include <stdint.h>
#include "project.h"
#include "fonts.h"
#include "led_mat_displ.h"

void replicate_cmd( uint16_t the_cmd )
{
    uint16_t repl_cmds[NUM_MODULES_TOTAL];
    
    for (int i = 0; i < NUM_MODULES_TOTAL; ++i)
        repl_cmds[i] = the_cmd;
        
    while (!(SPIM_ReadTxStatus() & SPIM_STS_SPI_IDLE)) {}
    SPIM_PutArray( repl_cmds, NUM_MODULES_TOTAL );    // send NUM_CHAR_MODULES bytes, in one transaction

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

void get_pels_from_str( char *the_str, int str_idx, int num_chars, volatile uint8_t str_pels[NUM_CHAR_TO_RENDER][8] )
{
    
    for (int i = 0; i < num_chars; ++i)
        char_to_pels(the_str[str_idx+i], &str_pels[i][0] );   
}

void get_disp_cmds_frac_shift( volatile uint8_t str_pels[NUM_CHAR_TO_RENDER][8], int frac_shift, 
                                volatile uint16_t disp_cmds[8][NUM_MODULES_TOTAL], int vshift, int start_module, int num_modules_text )
{
    // Step through each row
    for (int row_cnt = 0; row_cnt < 8; ++row_cnt)
    {
        // long_word holds the pixel values for the current row
        // The array supports up to a total of 16 characters rendered
        uint64_t long_word[2]={0,0};
        
        int raw_row_idx = row_cnt + vshift;
        
        // row_idx gets clipped, to avoid writing out of the destination array
        // raw_row_idx does not get clipped, if this would clip, then we "blank" the row
        int row_idx = raw_row_idx;
        
        if (row_idx < 0) row_idx = 0;
        if (row_idx > 7) row_idx = 7;
        
        // Step through each character in the string -- may render one more than needed to support fractional horizontal shift
        for (int char_idx = 0; char_idx < NUM_CHAR_TO_RENDER; ++char_idx)
        {
            // Use 64-bit containers here to avoid needing to cast later
            uint64_t the_pels = str_pels[char_idx][row_idx];
            uint64_t shift_amt = 8*(num_modules_text-char_idx);     // For example, shift_amt can range from 64 to 0
            
            if (shift_amt >= 64)
            {
                shift_amt -= 64;
                long_word[1] |= (the_pels << shift_amt);
            }
            else
            {
                long_word[0] |= (the_pels << shift_amt);
            }
        }
        
        if (raw_row_idx < 0 || raw_row_idx > 7)
        {
            // Off screen, blank the row
            long_word[1] = long_word[0] = 0;
        }
        
        for (int char_idx = 0; char_idx < num_modules_text; ++char_idx)
        {
            uint32_t char_pels;
            uint64_t bits_lsb = (num_modules_text-char_idx)*8-frac_shift;   // bits_lsb goes from to 1 to 64
                                                                            // (max at char_idx = 0, frac_shift = 0)
            if (bits_lsb == 64)
            {
                char_pels = long_word[1] & 0xFF;
            }
            else if (bits_lsb > 56)
            {
                // For example, if bits_lsb = 59:
                //    char_pels = {long_word[1][bits 2:0], long_word[0][bits 63:59]}
                // General case:
                //    char_pels = {long_word[1][bits_lsb-57:0], long_word[0][bits 63:bits_lsb]}
                //    so, # of bits from long_word[1] are bits_lsb-56
                //        # of bits from long_word[0] are 64-bits_lsb
                uint64 low_shift_right  = bits_lsb;
                uint64 num_bits_low     = 64-bits_lsb;
                char_pels = ((long_word[1] << num_bits_low) | (long_word[0] >> low_shift_right)) & 0xFF;
            }
            else
            {
                char_pels = (long_word[0] >> bits_lsb) & 0xFF;
            }
            disp_cmds[row_cnt][char_idx + start_module] = ((row_cnt+1) << 8) + char_pels;  // The high 8-bits are command to disp. ctlr.
        }
    }
}

void put_cmds_to_disp( volatile uint16_t disp_cmds[8][NUM_MODULES_TOTAL] )
{
    for (int row = 0; row < 8; ++row)
    {
        while (!(SPIM_ReadTxStatus() & SPIM_STS_SPI_IDLE)) {}
        SPIM_PutArray( &disp_cmds[row][0], NUM_MODULES_TOTAL );    // send 4 bytes, in one transaction
    }
}


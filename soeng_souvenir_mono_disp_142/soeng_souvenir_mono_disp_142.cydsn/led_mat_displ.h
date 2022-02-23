#include <stdint.h>

void replicate_cmd( uint16_t the_cmd );
void configure_max7219();
void char_to_pels( uint8_t the_char, volatile uint8_t the_pels[8] );
void get_pels_from_str( char *the_str, int str_idx, int num_chars, volatile uint8_t str_pels[5][8] );
void get_disp_cmds_frac_shift( volatile uint8_t str_pels[5][8], int frac_shift, volatile uint16_t disp_cmds[8][4], int vshift );
void put_cmds_to_disp( volatile uint16_t disp_cmds[8][4] );


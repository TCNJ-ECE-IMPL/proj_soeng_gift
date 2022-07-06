#include <stdint.h>

#define NUM_MODULES_TOTAL   (12)
#define START_MODULE_TEXT   (0)
#define NUM_MODULES_TEXT    (8)
#define NUM_CHAR_TO_RENDER  (NUM_MODULES_TEXT + 1)
#define START_MODULE_TIME   (8)
#define NUM_MODULES_TIME    (4)

void replicate_cmd( uint16_t the_cmd );
void configure_max7219();
void char_to_pels( uint8_t the_char, volatile uint8_t the_pels[8] );
void get_pels_from_str( char *the_str, int str_idx, int num_chars, volatile uint8_t str_pels[NUM_CHAR_TO_RENDER][8] );
void get_disp_cmds_frac_shift( volatile uint8_t str_pels[NUM_CHAR_TO_RENDER][8], int frac_shift, volatile uint16_t disp_cmds[8][NUM_MODULES_TOTAL], int vshift, int start_module, int num_modules );
void put_cmds_to_disp( volatile uint16_t disp_cmds[8][NUM_MODULES_TOTAL] );


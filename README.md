# proj_soeng_gift
This is the PSoC 4 project, and code, for the TCNJ School of Engineering Roaring Clock.

Some notes:

1. The project is developed using the Cypress/Infineon PSoC Creator IDE.  It can be downloaded for free (only for Windows) [here, hit the big Download button](https://www.infineon.com/cms/en/design-support/tools/sdk/psoc-software/psoc-creator/?utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-training).
2. The code is written in pure 'C', not C++.  The main routine is in the file 'main.c'
3. You will need a physical device to reprogram your  microcontroller unit (MCU) module.  The cheapest approach would be to visit Dr. Pearlstein, and ask him to set you up in the lab to modify your board.  If you'd like to do this at home, you can buy the [PSoC 4 Dev Kit](https://www.digikey.com/en/products/detail/cypress-semiconductor-corp/CY8CKIT-042-BLE-A/6189340), which includes a carrier board for programming, and a spare MCU board.  You'll have to disassemble your enclosure to get to the MCU board.
4. The main routine handles things that are not real-time critical.  It initializes everything, and creates an infinite loop, which does:
    1. Updates the color of the LED strip that illuminates the acrylic lion's head.  The pattern transitions between Blue (RGB=0,0,255) and Gold (RGB=255,255,0).
    2. Updates minutes and seconds, based on buttons.
5. The banner string that is displayed on the hour, and when both buttons are pressed together, is in the file `tcnj_config.h`.

Most of the interesting work is done in Interrupt Service Routines.  These are in files:

- `lf_counter_callback.c` -- the procedure `lf_counter_callback()` is called exactly 32 times per second.  Actually, the crystal oscillator's accuracy is claimed to be about 150 parts per million (ppm).  This is pretty accurate, but your clock could gain or lose up to 1 minute per 5 days.  If you see a problem, consider adding/subtracting some number of seconds per day to address the issue.
    - On the hour (or when both buttons are pressed), we set:

`        cur_sound_idx   = 0;    // both buttons: restart the ROAR sound`

`        animation_count = 0;    // both buttons: do the banner display`

`        splashing_flag  = 1;    // both buttons: do the banner display`

which restarts the ROAR sound, turns on the "splash display" function, and resets the state of the splash sequence.

A key piece of code in here is:

`                get_pels_from_str( banner_string, banner_shift_idx, 5, str_pels );` - Gets 5 characters from the banner string to display (since characters can be shifted in units of 1/8 of a character, we need to be able to display 2 partial characters left and right, and 3 full characters in the middle), and converts these characters to a bitmap, which is stored in the array `static uint8_t str_pels[5][8]`.  Each uint8_t element represents a horizontal row of 8 on/off pixels.

`                get_disp_cmds_frac_shift( str_pels, frac_shift_idx, disp_cmds, 0 );` - extracts the fractionally shifted set of pixels for the 4-character display, and converts them to display commands. 

`                put_cmds_to_disp( disp_cmds );` - sends the display commands to the display device.


- `isr1.c` -- the procedure `isr_1_Interrupt` is called 22,059 times per second.  This is done to approximately match the 22,050 Hz sample rate of the sampled lion roar sound.  This sound is represented in the array `const uint8_t roar[]`, which is in the file `sound_dat.h`.  The index of the current sample is maintained in the variable `cur_sound_idx`, which cycles from 0 to `NUM_SAMPS + FADE_PER`.  Note that the sound fades up and down to avoid clicks.

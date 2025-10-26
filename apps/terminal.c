#include "apps.h"

#include "../drivers/keyboard.h"
#include "../drivers/display.h"



#define terminal_banner "fuckSH 1.0: truly a masterpiece\n by ByteDev"

// runs the terminal app
void terminal_init() {
    print_string(terminal_banner);
    print_string("\n>");
}

/*  Contains all the linkages to the low level stuff, such as
    display and keyboard i/o, 1ms timers, and interrupt servicers.
    This is also the place where the morse keyer software lives.
*/

#include <string.h>

#include "main.h"
#include "menu.h"
#include "io.h"



void io_init(void) {
    //initialize all buffers for io routines
    ;
}
int io_getkey(void) {
    //fetch character input from all available sources
    return 0;
}

void io_disp(char *s) {
    //send 16x2 character display to available display device
}












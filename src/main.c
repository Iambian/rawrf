/* Dummy main to be finangled into the source thinger */

#include <string.h>

#include "main.h"
#include "menu.h"
#include "io.h"

void main(void) {
    int k;

    io_init();
    mstate_init(minit_first);      //inits menu system. found in menu

    while (1) {
        mstate.curkey = io_getkey();   //function found in io. Is our key input
        mstate.callback();
        //Do timer fiddly bits to ensure that the display driver is attempted
        //only so many times a second. Something about framerates and keeping
        //the game on a steady timebase. Or sommat like that.

        //Time-sensitive components should probably be kept track of separately.

        //Also, perhaps add a way to break out of this
        //loop when compiled for hosted platforms.
        // Justathot.

    }
}











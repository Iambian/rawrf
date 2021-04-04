/*  Contains all the high level subroutines for menus 
    and the menus themselves
*/

#include <string.h>

#include "main.h"
#include "menu.h"
#include "io.h"
#include "cwinv.h"


/* ************************** STRING UTILITY FUNCTIONS ************************ */

/* Returns menu print cursor to top-left corner */
void sutil_homeup(void) {
    mstate.currow = 0;
    mstate.curcol = 0;
}

void sutil_newline(void) {
    mstate.curcol = 0;
    ++mstate.currow;
}

void sutil_setcol(int n) {
    mstate.curcol = n;
}

void sutil_setrow(int n) {
    mstate.currow = n;
}

void sutil_cr(void) {
    mstate.curcol = 0;
}

/* Prints character to menu buffer at curcol/currow */
void sutil_printc(char c) {
    if (mstate.currow > 1) return;  //do not display if below end of screen
    if (mstate.curcol > 15) {       //This exists to prevent crash via bad curcol
        mstate.curcol = mstate.curcol & 0x0F;   //cheap modulo 16
    }
    mstate.screenbuf[mstate.currow*16 + mstate.curcol] = c;
    ++mstate.curcol;
    if (mstate.curcol > 15) {
        mstate.curcol = mstate.curcol & 0x0F;   //also cheap modulo 16
        ++mstate.currow;
    }
}

/* Prints string to menu buffer */
void sutil_prints(char *s) {
    for (;*s,++s) sutil_printc(*s);
}
/* Prints string to menu buffer until zero-terminator or n chars printed */
void sutil_printsn(char *s, int n) {
    for (;(n>1 && *s);++n,++s) sutil_printc(*s);
}


/*  NOTES
    Stack only starts unwinding once last digit in number encounted.
    If length is longer than number, all leading zeroes will print, then
        the stack will start unwinding for all remaining digits.
    If length is too short but number is too long, the stack unwinds
        while printing all 9's for each available spot.
    Otherwise, the stack unwinds for each digit last encounted, left-to-right.
*/
/*Recursively prints digits of num with length len */
int sutil_putint(int num, int len) {
    int a;  //temp variable used to store reduced num for recursion.

	/* force num to positive, adding a negative sign in string. Guaranteed run-once */
	if (num < 0) {
		sutil_printc('-');
		num = 0 - num;
	}
	/* Recurse until 0 len or num is not more than 9 */
	if (num > 9) {
		int a = num / 10;
		num = num - 10 * a;
		if (len < 2 || game_putint(a, len - 1)) {
			sutil_printc('9');
			return 1;
		}
	} else if (len) {
		for (; len > 1; --len)
			sutil_printc('0');
	}
	sutil_printc('0' + num);
	return 0;
}



/* **************************** MAINTENANCE FUNCTIONS ************************* */

void mstate_setcallback(Callback callback) {
    mstate.callback = callback;
}

void mstate_resetbuffer(void) {
    memset(mstate.screenbuf,0,sizeof(mstate.screenbuf));
}

void mstate_setmenu(char** menu) {
    int i;
    mstate.menu = menu;
    mstate.curopt = 0;
    ++menu;
    for (i = 0; menu != NULL; ++i, ++menu);    //iterates over menu options until NULL hit.
    mstate.maxopt = i;
}

void mstate_init(minit_t init) {
    mstate_setcallback(init.callback);
    mstate_setmenu(init.menu);
}

int mstate_handlemenu(void) {
    int k;
    
    k = mstate.curkey;

    switch (mstate.curkey) {
        case KBD_ESC:
            return -2;
        case KBD_LEFT:
        case KBD_UP:
            return mstate.curopt;
        case KBD_DOWN:
        case KBD_RIGHT:
            ++mstate.curopt;
            if (mstate.curopt >= mstate.maxopt) {
                mstate.curopt = 0;
            }
            break;
        default:
            break;
    }
    ;
    ;
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void mstate_dispmenu(void) {
    mstate_resetbuffer();
    mstate_dispmenutitle();
    mstate_dispmenucontents();
}

void mstate_dispmenutitle(void) {
    char *screenbuf;
    char *menubuf;
    int slen;

    screenbuf = mstate.screenbuf;
    menubuf = mstate.menu[0];
    slen = strlen(menubuf);

    memset(screenbuf,' ', 16);                      //top clears line
    if (slen<16) memcpy(screenbuf,menubuf,slen);    //Fill in title
}

void mstate_dispmenucontents(void) {
    int i;
    int slen;
    char *screenbuf;
    char *menubuf;

    screenbuf = mstate.screenbuf+16;
    menubuf = mstate.menu[mstate.curopt];
    slen = strlen(menubuf);
    //"01234567879ABCDEF"
    //"<-         NEXT->"
    memcpy(screenbuf,"<-         NEXT->",16);       //presets bottom line
    if (slen<8) memcpy(screenbuf+2,menubuf,slen);   //fill in contents
}


/* ********************* FUNCTIONS THAT CONTAINS CONTENT ********************* */





/* ~~~~~~~~~~~~~ MAIN MENU ~~~~~~~~~~~~~~~~~ */
minit_t minit_first = { &mstate_first, {"Game Modes","CW_INV","TRAIN","OPTNS",NULL} };
minit_t minit_cwinvaders = { &mstate_cwinvaders, {"CW Invaders","Start","Options","Stats","Reset","Go Back", NULL}};
minit_t minit_trainer = { &mstate_trainer, {"Trainer","???","????","Go back"}};
minit_t minit_options = { &mstate_options, {"Main Options","Startup","Keyer","About","Go back"}};


void mstate_first(void) {
    mstate_dispmenu();

    switch (mstate_handlemenu()) {
        case 0:
            //cwinvaders
            mstate_init( minit_cwinvaders );
            break;
        case 1:
            //trainer
            break;
        case 2:
            //options
            break;
        default:
            break;
    }
}

/* ~~~~~~~~~~~~~ CW INVADERS SUBMENU ~~~~~~~~~~~~~~~~~ */
minit_t minit_cwgame = { &mstate_cwgame, {NULL}};
minit_t minit_cwoptions = { &mstate_cwoptions, {"CW Options","Level","Timings","Go back"}};
minit_t minit_cwreset = { &mstate_cwreset, {"CONFIRM RESET","No","Yes"}};
minit_t minit_cwstats = { &mstate_cwstats, {NULL}};

void mstate_cwinvaders(void) {
    mstate_dispmenu();
    
    switch (mstate_handlemenu()) {
        case 0:
            mstate_init( minit_cwgame );
            //game mode. This is a full custom setup.
            break;
        case 1:
            //options
            mstate_init( minit_cwoptions );
            break;
        case 2:
            //stats. what the hekk goes in here?
            //scores definitely. accuracy?
            break;
        case 3:
            //reset
            break;
        case 4:
            //go back
            mstate_init( minit_first );
            break;
        default:
            break;
    }
}

void mstate_cwgame(void) {
    //This is where the bulk of the program goes. This should NOT use
    //ANY of the predefined menu-y stuff. Keyboard input, however, will still
    //be passed through mstate.curkey.
    mstate_init( minit_cwinvaders );
}

void mstate_cwoptions(void) {
    /* You may want to inline the options so the data displays \
       in the title field. I don't want to have menu options too deep */
    mstate_dispmenu();
    
    switch (mstate_handlemenu()) {
        case 0:
            //level(difficulty) settings
            break;
        case 1:
            //timing/sensitivity settings
            break;
        case 2:
        case -2:
            mstate_init( minit_cwinvaders );
            break;
        default:
            break;
    }
}

void mstate_cwstats(void) {
    /* Stats should be custom/standalone. Maybe use all directionals?
       or idk. something. just not the default implementation */
    mstate_dispmenu();
    mstate_init( minit_cwinvaders );
    
}

void mstate_cwreset(void) {
    mstate_dispmenu();
    
    switch (mstate_handlemenu()) {
        case 0:
        case -2:
            //go back
            mstate_init( minit_cwinvaders );
            break;
        case 1:
            //reset now
            /*
                DO ALL THE THINGS THAT RESETS ALL THE SAVED SETTINGS
                IN THE CW INVADERS GAME
            */
            break;
        default:
            break;
    }
}


/* ~~~~~~~~~~~~~ TRAINER SUBMENU ~~~~~~~~~~~~~~~~~ */

void mstate_trainer(void) {
    mstate_dispmenu();

    switch (mstate_handlemenu()) {
        case 0:
            //??
            break;
        case 1:
            //????
            break;
        case 2:
        case -2:
            mstate_init( minit_first );
            break;
        default:
            break;
    }
}


/* ~~~~~~~~~~~~~ (MAIN) OPTIONS SUBMENU ~~~~~~~~~~~~~~~~~ */

minit_t minit_optstart = { &mstate_optstart, {NULL}};
minit_t minit_optkeyer = { &mstate_optkeyer, {NULL}};
minit_t minit_optabout = { &mstate_optabout, {NULL}};

void mstate_options(void) {
    mstate_dispmenu();

    switch (mstate_handlemenu()) {
        case 0:
            // Startup mode
            mstate_init( minit_optstart );
            break;
        case 1:
            // Keyer options
            mstate_init( minit_optkeyer );
            break;
        case 2:
            // About screen
            mstate_init( minit_optabout );
        case 3:
        case -2:
            mstate_init( minit_first );
            break;
        default:
            break;
    }
}



void mstate_optstart(void) {
    //Not sure what's going to go here. Startup options. What options???
    mstate_dispmenu();
    mstate_init( minit_options );
}
void mstate_optkeyer(void) {
    //Put in options wrt keyer, such as farnsworth on/off, timings, 
    mstate_dispmenu();
    mstate_init( minit_options );
}

void mstate_optabout(void) {
    //Full custom inputs and scrolling
    mstate_dispmenu();
    mstate_init( minit_options );
}











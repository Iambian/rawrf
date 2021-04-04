#ifndef __GUARD_MENUH__
#define __GUARD_MENUH__

#define KBD_ESC 0x1B
#define KBD_LEFT 'A'
#define KBD_RIGHT 'D'
#define KBD_UP 'W'
#define KBD_DOWN 'S'


typedef void (*Callback)(void);
//NOTE: menu is in form of NULL-terminated string array.
//e.g. {"Title","firstopt","secondopt",NULL};

typedef struct mstate_st {
    Callback callback;      //contains a menu rendering function that uses mstate global
    char** menu;            //Pointer to the menu being used
    int curkey;             //stores currently-presed key/keymacro
    int curopt;             //current selected option (0 to N-1)
    int maxopt;             //maximum options in menu (1 to N)
    unsigned int currow;             //0 or 1, current row in 16x2 char display
    unsigned int curcol;             //0 to 15, current column in 16x2 char display
    char* screenbuf[32];    //stores the current state of the screen
} mstate_t;
extern mstate_t mstate;     //defined in menu.c; contains current menu state, including buffers

typedef minit_st {
    Callback callback;
    char *menu[];
} minit_t;
extern minit_t minit_init;  //Very first menu. Used/called upon program initialize


//String utility functions
void sutil_homeup(void);
void sutil_newline(void);
void sutil_setcol(int n);
void sutil_setrow(int n);
void sutil_cr(void);
#define sutil_cls() mstate_resetbuffer()
#define sutil_clearlcdfull() mstate_resetbuffer()
void sutil_printc(char c);
void sutil_prints(char *s);
void stuil_printsn(char *s, int n);


//Maintenance: setup routines
void mstate_init(minit_t init);
void mstate_setcallback(Callback callback);
void mstate_resetbuffer(void);
void mstate_setmenu(char** menu);
int mstate_handlemenu(void);    //ret: -2 = cancel, -1 = not sel, curopt = sel.


//Maintenance: Display routines
void mstate_dispmenu(void);
void mstate_dispmenuttitle(void);
void mstate_dispmenucontents(void);


//Menu contents
void mstate_first(void);
//
void mstate_cwinvaders(void);
void mstate_cwgame(void);
void mstate_cwoptions(void);
void mstate_cwstats(void);
void mstate_cwreset(void);
//
void mstate_trainer(void);
//
void mstate_options(void);
void mstate_optstart(void);
void mstate_optkeyer(void);
void mstate_optabout(void);


#endif
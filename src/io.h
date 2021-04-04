#ifndef __GUARD_IOH__
#define __GUARD_IOH__

void io_init(void);     //Initialize all i/o peripherals and buffers
int io_getkey(void);    //Fetch character input from all available sources
void io_disp(char *s);  //send 16x2 character display to available display device


#endif
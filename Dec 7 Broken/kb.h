#ifndef KB_H
#define KB_H

#include "library.h"



unsigned char key_lookup(int index);
void keyboard_handler(registers_t);
void typing(registers_t);
void init_typing();
void init_keyboard();

#endif

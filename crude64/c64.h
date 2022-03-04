#ifndef _C64_H
#define _C64_H

#include "pico/stdlib.h"

#define RAM_KB 32
#define RAM_SIZE RAM_KB * 1024 + 256 + 128 + 64

extern void exec6502(int32_t tickcount);
extern void reset6502();

char waitForKey();

void bootC64();

void escapeMenu();

void pokeKey(uint8_t key);
void readKeyboard();

void writePetscii(uint8_t petscii, uint8_t row, uint8_t col);
void drawBackAndBorder();
void redrawScreen();

void run64();
void splashScreen();

#endif

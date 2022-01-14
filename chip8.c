#include "chip8.h"
#include <stdio.h>


void init(struct chip8 *chip) {
    chip->opcode = 0;
    chip->pc = 0x200;
    chip->sp = 0;
    chip->I = 0;
    chip->DT = 0;
    chip->ST = 0;

    int i = 0;
    for (i = 0; i < 4096; i++) {
        chip->memory[i] = 0;
    }

    for (i = 0; i < 16; i++) {
        chip->V[i] = 0;
        chip->stack[i] = 0;
        chip->keypad[i] = 0;
    }
}


void execute(struct chip8* chip, unsigned short opcode) {
    
}

void reset(struct chip8 *chip) {

}






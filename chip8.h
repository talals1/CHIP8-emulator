#ifndef CHIP8_H
#define CHIP8_H

struct chip8 {
    
    unsigned short opcode; // current instruction (opcode) being run

    unsigned short pc; // program counter
    unsigned char sp; // stack counter
    unsigned short I;

    unsigned char DT; // delay timer
    unsigned char ST; // sound timer

    unsigned short stack[16];
    unsigned char memory[4096];
    unsigned char V[16]; // V registers, V0 - VF
    unsigned char keypad[16];


};

void init(struct chip8 *chip);
void execute(struct chip8 *chip, unsigned short opcode);
void reset(struct chip8 *chip);


#endif



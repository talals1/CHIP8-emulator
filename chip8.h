#ifndef CHIP8_H
#define CHIP8_H

struct chip8 {
    
    unsigned short opcode; // current instruction (opcode) being run

    unsigned short pc; // program counter
    unsigned char sp; // stack counter
    unsigned short I;

    unsigned int rom_size; // size of the ROM currently loaded

    unsigned char DT; // delay timer
    unsigned char ST; // sound timer

    unsigned short stack[16];
    unsigned char memory[4096];
    unsigned char V[16]; // V registers, V0 - VF
    unsigned char keypad[16];
    unsigned char display[32][64]; // The CHIP-8 has a 64x32 display


};

void init(struct chip8 *chip);
void load_rom(struct chip8 *chip, unsigned char rom[], int size);
void run(struct chip8 *chip);
void execute(struct chip8 *chip, unsigned short opcode);
void reset(struct chip8 *chip);

void inst_JP(struct chip8 *chip, unsigned short opcode);
void inst_CALL(struct chip8 *chip, unsigned short opcode);
void inst_SE_Vx_byte(struct chip8 *chip, unsigned short opcode);
void inst_SNE_Vx_byte(struct chip8 *chip, unsigned short opcode);
void inst_SE_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_LD_Vx_byte(struct chip8 *chip, unsigned short opcode);
void inst_ADD_Vx_byte(struct chip8 *chip, unsigned short opcode);
void inst_LD_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_OR_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_AND_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_XOR_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_ADD_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_SUB_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_SHR_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_SUBN_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_SHL_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_SNE_Vx_Vy(struct chip8 *chip, unsigned short opcode);
void inst_LD_I_addr(struct chip8 *chip, unsigned short opcode);
void inst_JP_V0_addr(struct chip8 *chip, unsigned short opcode);
void inst_RND_Vx_byte(struct chip8 *chip, unsigned short opcode);
void inst_DRW_Vx_Vy_nibble(struct chip8 *chip, unsigned short opcode);
void inst_SKP_Vx(struct chip8 *chip, unsigned short opcode);
void inst_SKNP_Vx(struct chip8 *chip, unsigned short opcode);
void inst_LD_Vx_DT(struct chip8 *chip, unsigned short opcode);
void inst_LD_Vx_K(struct chip8 *chip, unsigned short opcode);
void inst_LD_DT_Vx(struct chip8 *chip, unsigned short opcode);
void inst_LD_ST_Vx(struct chip8 *chip, unsigned short opcode);
void inst_ADD_I_Vx(struct chip8 *chip, unsigned short opcode);
void inst_LD_F_Vx(struct chip8 *chip, unsigned short opcode);
void inst_LD_B_Vx(struct chip8 *chip, unsigned short opcode);
void inst_LD_I_idx_Vx(struct chip8 *chip, unsigned short opcode);
void inst_LD_Vx_I_idx(struct chip8 *chip, unsigned short opcode);


#endif



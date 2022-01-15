#include "chip8.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void init(struct chip8 *chip) {
    chip->opcode = 0;
    chip->pc = 0x200;
    chip->sp = -1;
    chip->I = 0;
    chip->rom_size = 0;
    chip->DT = 0;
    chip->ST = 0;

    int i = 0;
    // Initalize memory
    for (i = 0; i < 4096; i++) {
        chip->memory[i] = 0;
    }

    // Initialize registers, stack, and keypad
    for (i = 0; i < 16; i++) {
        chip->V[i] = 0;
        chip->stack[i] = 0;
        chip->keypad[i] = 0;
    }

    // Initialize display
    int j = 0;
    for (i = 0; i < 32; i++){
        for (j = 0; j < 64; j++) {
            chip->display[i][j] = 0;
        }
    }
}

void load_rom(struct chip8 *chip, unsigned char rom[], int size) {
    int i = 0;
    for(i = 0; i < size; i++) {
        // CHIP-8 programs mostly start at location 0x200 (512)
        chip->memory[512 + i] = rom[i];
    }
    chip->rom_size = size;
}

void run(struct chip8 *chip) {
    printf("Running the program...\n");

    // int i = 0;
    // for(i = 0; i < chip->rom_size; i = i + 2) {
    //     
    //     unsigned short opcode = (chip->memory[512 + i] << 8) | chip->memory[512 + i + 1];
    //     execute(chip, opcode);
    // }

    for(;;) {
        int i = chip->pc;

        // Since an opcode is comrpised of 2 bytes, we have to shift by first one over 1 byte
        // and then append the second byte
        unsigned short opcode = (chip->memory[i] << 8) | chip->memory[i + 1];
        printf("pc = %i , opcode = 0x%X\n", chip->pc, opcode);
        execute(chip, opcode);

        chip->pc += 2;

        if (chip->DT > 0) {
            chip->DT -= 1;
        }

        if (chip->ST > 0) {
            chip->ST -= 1;
        }

        sleep(1);
    }
}

// Executes the given opcode/instruction on the given chip
void execute(struct chip8 *chip, unsigned short opcode) {
    // We AND with 0xF000 so we can focus on only the left most bit using the swithc statement.
    // Each opcode has its own functiom, but some opcode (such as 0x8000's and 0xE000's) have
    // several variations, making it necessary to have a helper function for them.

    switch (opcode & 0xF000)
    {
    case 0x0000:
        handle_zero_inst(chip, opcode);
        break;
    
    case 0x1000:
        inst_JP(chip, opcode);
        break;
    
    case 0x2000:
        inst_CALL(chip, opcode);
        break;
    
    case 0x3000:
        inst_SE_Vx_byte(chip, opcode);
        break;
    
    case 0x4000:
        inst_SNE_Vx_byte(chip, opcode);
        break;

    case 0x5000:
        inst_SE_Vx_Vy(chip, opcode);
        break;
    
    case 0x6000:
        inst_LD_Vx_byte(chip, opcode);
        break;

    case 0x7000:
        inst_ADD_Vx_byte(chip, opcode);
        break;

    case 0x8000:
        handle_eight_inst(chip, opcode);
        break;

    case 0x9000:
        inst_SNE_Vx_Vy(chip, opcode);
        break;
    
    case 0xA000:
        inst_LD_I_addr(chip, opcode);
        break;
    
    case 0xB000:
        inst_JP_V0_addr(chip, opcode);
        break;

    case 0xC000:
        inst_RND_Vx_byte(chip, opcode);
        break;
    
    case 0xD000:
        inst_DRW_Vx_Vy_nibble(chip, opcode);
        break;
    
    case 0xE000:
        handle_E_inst(chip, opcode);
        break;
    
    case 0xF000:
        handle_F_inst(chip, opcode);
        break;

    default:
        printf("Got an unknown opcode: 0x%x", opcode);
        break;
    }
}

void handle_zero_inst(struct chip8 *chip, unsigned short opcode){
    int i = 0;
    switch (opcode)
    {
    case 0x00E0 :
        // Clear screen
        for (i = 0; i < 32; i++){
            int j = 0;
            for (j = 0; j < 64; j++) {
                chip->display[i][j] = 0;
            }
        }
        break;
    case 0x00EE :
        // Return from subroutine
        chip->pc = chip->stack[chip->sp];
        chip->sp -= 1;
        break;
    default:
        // Opcode 0nnn should be ignored as it was meant for older machines
        break;
    }
}

void handle_eight_inst(struct chip8 *chip, unsigned short opcode) {
    switch(opcode & 0xF00F) {
        case 0x8000:
            inst_LD_Vx_Vy(chip, opcode);
            break;
        
        case 0x8001:
            inst_OR_Vx_Vy(chip, opcode);
            break;

        case 0x8002:
            inst_AND_Vx_Vy(chip, opcode);
            break;

        case 0x8003:
            inst_XOR_Vx_Vy(chip, opcode);
            break;

        case 0x8004:
            inst_ADD_Vx_Vy(chip, opcode);
            break;

        case 0x8005:
            inst_SUB_Vx_Vy(chip, opcode);
            break;

        case 0x8006:
            inst_SHR_Vx_Vy(chip, opcode);
            break;

        case 0x8007:
            inst_SUBN_Vx_Vy(chip, opcode);
            break;

        case 0x800E:
            inst_SHL_Vx_Vy(chip, opcode);
            break;

        default:
            printf("Got an unknown opcode: 0x%x", opcode);
            break;
    }
}

void handle_E_inst(struct chip8 *chip, unsigned short opcode) {
    switch(opcode & 0xF0FF) {
        case 0xE09E:
            inst_SKP_Vx(chip, opcode);
            break;

        case 0xE0A1:
            inst_SKNP_Vx(chip, opcode);
            break;

        default:
            printf("Got an unknown opcode: 0x%x", opcode);
            break;
    }
}

void handle_F_inst(struct chip8 *chip, unsigned short opcode) {
    switch(opcode & 0xF0FF) {
        case 0xF007:
            inst_LD_Vx_DT(chip, opcode);
            break;

        case 0xF00A:
            inst_LD_Vx_K(chip, opcode);
            break;

        case 0xF015:
            inst_LD_DT_Vx(chip, opcode);
            break;

        case 0xF018:
            inst_LD_ST_Vx(chip, opcode);
            break;

        case 0xF01E:
            inst_ADD_I_Vx(chip, opcode);
            break;

        case 0xF029:
            inst_LD_F_Vx(chip, opcode);
            break;

        case 0xF033:
            inst_LD_B_Vx(chip, opcode);
            break;

        case 0xF055:
            inst_LD_I_idx_Vx(chip, opcode);
            break;
        
        case 0xF065:
            inst_LD_Vx_I_idx(chip, opcode);
            break;

        default:
           printf("Got an unknown opcode: 0x%x", opcode);
           break; 
    }
}

void inst_JP(struct chip8 *chip, unsigned short opcode) {
    chip->pc = opcode - 0x1000;
}

void inst_CALL(struct chip8 *chip, unsigned short opcode) {
    chip->sp += 1;
    chip->stack[chip->sp] = chip->pc;
    chip->pc = opcode - 0x2000;
}

void inst_SE_Vx_byte(struct chip8 *chip, unsigned short opcode) {
    unsigned char reg = (opcode & 0x0F00) >> 8;
    unsigned char value = (opcode & 0x00FF);
    if (chip->V[reg] == value) {
        chip->pc += 2;
    }
}

void inst_SNE_Vx_byte(struct chip8 *chip, unsigned short opcode) {
    unsigned char reg = (opcode & 0x0F00) >> 8;
    unsigned char value = (opcode & 0x00FF);
    if (chip->V[reg] != value) {
        chip->pc += 2;
    }
}

void inst_SE_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    if (chip->V[vx] == chip->V[vy]) {
        chip->pc += 2;
    }
}

void inst_LD_Vx_byte(struct chip8 *chip, unsigned short opcode) {
    unsigned char reg = (opcode & 0x0F00) >> 8;
    unsigned char value = (opcode & 0x00FF);
    chip->V[reg] = value;
}

void inst_ADD_Vx_byte(struct chip8 *chip, unsigned short opcode) {
    unsigned char reg = (opcode & 0x0F00) >> 8;
    unsigned char value = (opcode & 0x00FF);
    chip->V[reg] += value;
}

void inst_LD_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    chip->V[vx] = chip->V[vy];
}

void inst_OR_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    chip->V[vx] = chip->V[vx] | chip->V[vy];
}

void inst_AND_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    chip->V[vx] = chip->V[vx] & chip->V[vy];
}

void inst_XOR_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    chip->V[vx] = chip->V[vx] ^ chip->V[vy];
}

void inst_ADD_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    unsigned short sum = chip->V[vx] + chip->V[vy];
    if (sum > 255) {
        chip->V[15] = 1;
    } else {
        chip->V[15] = 0;
    }
    chip->V[vx] = (unsigned char) sum;
}

void inst_SUB_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    if (chip->V[vx] > chip->V[vy]) {
        chip->V[15] = 1;
    } else {
        chip->V[15] = 0;   
    }
    chip->V[vx] = chip->V[vx] - chip->V[vy];
}

void inst_SHR_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;

    if ((chip->V[vx] & 1) == 1) {
       chip->V[15] = 1;
    } else {
        chip->V[15] = 0;   
    }

    chip->V[vx] = chip->V[vx] / 2;
}

void inst_SUBN_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    if (chip->V[vy] > chip->V[vx]) {
        chip->V[15] = 1;
    } else {
        chip->V[15] = 0;   
    }
    chip->V[vx] = chip->V[vy] - chip->V[vx];
}

void inst_SHL_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;

    if ((chip->V[vx] & 1) == 1) {
       chip->V[15] = 1;
    } else {
        chip->V[15] = 0;   
    }

    chip->V[vx] = chip->V[vx] * 2;
}

void inst_SNE_Vx_Vy(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    if (chip->V[vx] != chip->V[vy]){
        chip->pc += 2;
    }
}

void inst_LD_I_addr(struct chip8 *chip, unsigned short opcode) {
    chip->I = opcode & 0x0FFF;
}

void inst_JP_V0_addr(struct chip8 *chip, unsigned short opcode) {
    unsigned short location = opcode & 0x0FFF;
    chip->pc = chip->V[0] + location;
}

void inst_RND_Vx_byte(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char value = opcode & 0x00FF;
    unsigned char rand_num = rand() & value;
    chip->V[vx] = rand_num;

}

void inst_DRW_Vx_Vy_nibble(struct chip8 *chip, unsigned short opcode) {
    // TODO
        printf("Unsupported opcode! opcode = 0x%X \n", opcode);

}

void inst_SKP_Vx(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    if (chip->keypad[chip->V[vx]] == 1) {
        chip->pc += 2;
    }
}

void inst_SKNP_Vx(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    if (chip->keypad[chip->V[vx]] == 0) {
        chip->pc += 2;
    }
}

void inst_LD_Vx_DT(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    chip->V[vx] = chip->DT;
}

void inst_LD_Vx_K(struct chip8 *chip, unsigned short opcode) {
    // Todo, may need to add a pause flag in the chip struct
        printf("Unsupported opcode! opcode = 0x%X \n", opcode);

}

void inst_LD_DT_Vx(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    chip->DT = chip->V[vx];
}

void inst_LD_ST_Vx(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    chip->ST = chip->V[vx];
}

void inst_ADD_I_Vx(struct chip8 *chip, unsigned short opcode) {
    unsigned char vx = (opcode & 0x0F00) >> 8;
    chip->I = chip->I + chip->V[vx];
}

void inst_LD_F_Vx(struct chip8 *chip, unsigned short opcode) {
    // todo
     printf("Unsupported opcode! opcode = 0x%X \n", opcode);

}

void inst_LD_B_Vx(struct chip8 *chip, unsigned short opcode) {
    // todo
    printf("Unsupported opcode! opcode = 0x%X \n", opcode);

}

void inst_LD_I_idx_Vx(struct chip8 *chip, unsigned short opcode) {
    // todo
     printf("Unsupported opcode! opcode = 0x%X \n", opcode);

}

void inst_LD_Vx_I_idx(struct chip8 *chip, unsigned short opcode) {
    // todo

    printf("Unsupported opcode! opcode = 0x%X \n", opcode);

}


void reset(struct chip8 *chip) {

}






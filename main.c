#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include "chip8.h"

struct chip8 chip;

int main(int argc, char *argv[]) {
    char* file_name = argv[1];
    FILE* gameFile = fopen(file_name, "rb");

    unsigned char buffer[100];

    if (!gameFile) {
        printf("Unable to read or find file \"%s\"\n", file_name);
        return 0;
    } else {
        printf("Loading \"%s\"...\n", file_name);
    }
    fseek(gameFile, 0, SEEK_SET);
    fread(buffer, sizeof(unsigned char), sizeof(buffer), gameFile);
    fclose(gameFile);
    int i = 0;
    for (i = 0; i < 100; i++) {
        printf("0x%02hhX ", buffer[i]);
    }

    init(&chip);

    return 0;
}

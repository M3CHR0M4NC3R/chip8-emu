struct chip8CPU {
    unsigned short opcode;
    unsigned char reg[16];
    unsigned short idx;
    unsigned short pc;
    
    unsigned char mem[4096];
    /* memory mapped as follows
    * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    * 0x200-0xFFF - Program ROM and work RAM
    */
    
    unsigned short stack[16];
    unsigned short sp;
    
    
    unsigned char pixels[64 * 32];
    
    unsigned char delay_timer;
    unsigned char sound_timer;
    
    //controller keys
    unsigned char key[16];
};

struct chip8CPU* initCPU();
int emulateCycle(struct chip8CPU *cpu);
int loadGame(struct chip8CPU *cpu, char *file);
void dumpCPU(struct chip8CPU *cpu);

//cpu spec
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

void initCPU(struct chip8CPU *cpu){
    cpu->pc = 0x200;
    cpu->opcode=0;
    cpu->idx=0;
    cpu->sp=0;
    
    // Load fontset
    /*
    for(int i = 0; i < 80; ++i)
        cpu->memory[i] = chip8_fontset[i]; TODO, where is the fontset?
    */
    
}
void emulateCycle(struct chip8CPU *cpu){
    int pc = cpu->pc;
    cpu->opcode = cpu->mem[pc] << 8 | cpu->mem[pc + 1];
    return;
}

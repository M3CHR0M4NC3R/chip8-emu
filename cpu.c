#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct chip8CPU* initCPU(){
    struct chip8CPU *cpu = calloc(1, sizeof(struct chip8CPU));
    cpu->pc = 0x200;
    cpu->opcode=0;
    cpu->idx=0;
    cpu->sp=0;
    
    // Load fontset
    /*
    for(int i = 0; i < 80; ++i)
        cpu->memory[i] = chip8_fontset[i]; TODO, where is the fontset?
    */
    
    return cpu;
}
int emulateCycle(struct chip8CPU *cpu){
    int pc = cpu->pc;
    int opcode = cpu->mem[pc] << 8 | cpu->mem[pc + 1];
    //cpu->opcode = opcode;
    //printf("%x %x\n",pc, opcode);
    if (pc==0x200)
        opcode = opcode>>8;

    switch(opcode & 0xF000){
        case 0x0000:
        {
            switch(opcode & 0x000F)
            {
              case 0x0000: //00E0 clear screen
                printf("clear screen instruction\n");
                break;
              default:
                printf("unhandled instruction 0x%x\n", opcode);
                return 0;
            }
        break;
        }
        case 0x6000:
            printf("%x: set v%X to %x\n",opcode, (opcode&0x0F00)>>8, opcode&0x00FF);
            cpu->reg[(opcode&0x0F00)>>8] = opcode&0x00FF;
            break;

        case 0xA000: // ANNN: Sets I to the address NNN
            printf("move %x to idx\n", opcode&0x0FFF);
            cpu->idx = opcode & 0x0FFF;
            break;
        case 0xD000: //DRAW SPRITE OMFG
            int regx = (opcode&0x0F00)>>8;
            int regy = (opcode&0x00F0)>4;
            printf("%x: draw sprite at V%x: %d, V%x: %d, 8 wide and %d tall\n",opcode, regx, cpu->reg[regx], regy, cpu->reg[regy], opcode&0x00F);
            break;
    break;
    default:
        printf("unhandled instruction 0x%x\n", opcode);
        return 0;
    }
    cpu->pc+=2;
    return 1;
}

int loadGame(struct chip8CPU *cpu, char *fileName){
    FILE *fptr;
    fptr = fopen(fileName, "rb");
    if(fptr==NULL){
        printf("game file %s not found\n", fileName);
        return 0;

    }
    fread(cpu->mem + 0x200, sizeof(cpu->mem[0]), 4096-0x200, fptr);
    //for now assume this is correct

    fclose(fptr);

    return 1;
}

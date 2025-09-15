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

void dumpCPU(struct chip8CPU *cpu){
    printf("pc: %x\n", cpu->pc);
    printf("idx: %x\n", cpu->idx);

    for(int i=0;i<0xF;i++)
        printf("register %x: %x\n",i,cpu->reg[i]);

    printf("graphics buffer:");
    for(int i=0;i<32*64;i++) {
        if(i%64==0)
            printf("\n");
        printf("%d",cpu->pixels[i]);
    }
}

int emulateCycle(struct chip8CPU *cpu){
    int pc = cpu->pc;
    int opcode = cpu->mem[pc] << 8 | cpu->mem[pc + 1];
    int regY,regX;
    //cpu->opcode = opcode;
    //printf("%x %x\n",pc, opcode);
    //if (pc==0x200)
    //    opcode = opcode>>8;

    switch(opcode & 0xF000){
        case 0x0000:
        {
            switch(opcode & 0x00FF)
            {
                case 0x00E0: //00E0 clear screen
                    printf("%x: clearning gfx\n", opcode);
                    for(int i=0;i<64*32;i++)
                        cpu->pixels[i]=0;
                    break;

                case 0x00EE:
                    cpu->sp--;
                    cpu->pc = cpu->stack[cpu->sp];
                    break;
                
              default:
                printf("unhandled 0000 instruction 0x%x\n", opcode);
                return 0;
            }
        break;
        }

        case 0x1000: //GOTO
            cpu->pc=opcode&0x0FFF;
            return 1;

        case 0x2000:
            cpu->stack[cpu->sp] = cpu->pc;
            cpu->sp++;
            cpu->pc = opcode & 0x0FFF;
            return 1;

        case 0x3000: //conditional skip next instruction (true)
            if (cpu->reg[(opcode&0x0F00)>>8]==(opcode&0x00FF))
                cpu->pc+=2;
            break;

        case 0x4000: //conditional skip next instruction (false)
            if (!(cpu->reg[(opcode&0x0F00)>>8]==(opcode&0x00FF)))
                cpu->pc+=2;
            break;

        case 0x5000:
            regX = (opcode&0x0F00)>>8;
            regY = (opcode&0x00F0)>>4;
            if(cpu->reg[regX]==cpu->reg[regY])
                cpu->pc+=2;
            break;

        case 0x9000:
            regX = (opcode&0x0F00)>>8;
            regY = (opcode&0x00F0)>>4;
            if(cpu->reg[regX]!=cpu->reg[regY])
                cpu->pc+=2;
            break;

        case 0x6000:
            cpu->reg[(opcode&0x0F00)>>8] = opcode&0x00FF;
            break;

        case 0x7000: //Addition, no cf
            regX = (opcode&0x0F00)>>8;
            cpu->reg[regX]+=opcode&0x0FF;
            break;

        case 0x8000:
            regX = (opcode&0x0F00)>>8;
            regY = (opcode&0x00F0)>>4;
            switch(opcode&0x000F)
            {
                case 0x0000: //copy reg y into reg x
                    cpu->reg[regX]=cpu->reg[regY];
                    break;

                case 0x0001:
                        cpu->reg[regX]|=cpu->reg[regY];
                        break;
                
                case 0x0002:
                        cpu->reg[regX]&=cpu->reg[regY];
                        break;

                case 0x0003:
                        cpu->reg[regX]^=cpu->reg[regY];
                        break;

                case 0x0004: //TODO VF flag
                        cpu->reg[regX]+=cpu->reg[regY];
                        break;

                case 0x0005: //TODO VF flag
                        cpu->reg[regX]-=cpu->reg[regY];
                        break;

                case 0x0006: //TODO VF flag
                        cpu->reg[regX]>>=1;
                        break;

                case 0x0007:
                        cpu->reg[regX]=(cpu->reg[regY]-cpu->reg[regX]);
                        break;

                case 0x000E:
                        cpu->reg[regX]<<=1;
                        break; 

                default:
                    printf("unhandled instruction 0x%x\n", opcode);
                    return 0;
            }
            break;

        case 0xA000: // ANNN: Sets I to the address NNN
            cpu->idx = opcode & 0x0FFF;
            break;

        //case 0xD000: //DRAW SPRITE OMFG
        //    int regx = (opcode&0x0F00)>>8;
        //    int regy = (opcode&0x00F0)>4;
        //    printf("%x: draw sprite at V%x: %d, V%x: %d, 8 wide and %d tall\n",opcode, regx, cpu->reg[regx], regy, cpu->reg[regy], opcode&0x000F);

        //    for(int i=0;i<(opcode&0x000F);i++){
        //        int n = cpu->mem[i+cpu->idx];
        //        while (n) {
        //            if (n & 1)
        //                printf("1");
        //            else
        //                printf("0");
        //        
        //            n >>= 1;
        //        }
        //        printf("\n");
        //    }
        //    break;
        case 0xD000:		   
            unsigned short x = cpu->reg[(opcode & 0x0F00) >> 8];
            unsigned short y = cpu->reg[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;
         
            cpu->reg[0xF] = 0;
            for (int yline = 0; yline < height; yline++) {
                pixel = cpu->mem[cpu->idx + yline];
                for(int xline = 0; xline < 8; xline++) {
                    if((pixel & (0x80 >> xline)) != 0) {
                        if(cpu->pixels[(x + xline + ((y + yline) * 64))] == 1)
                            cpu->reg[0xF] = 1;                                 
                        cpu->pixels[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }
         
            //drawFlag = true;
            break;

        case 0xF000:
            switch(opcode&0x00FF){
                case 0x0065:
                    regX = (opcode&0x0F00)>>8;
                    for(int i = 0;i<regX;i++)
                        cpu->reg[i]=cpu->mem[(cpu->idx)+i];
                    break;

            }
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

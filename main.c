#include "cpu.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

void drawGFX(unsigned char pixels[64*32]);

struct chip8CPU *cpu;
typedef enum GameScreen { INIT=0, GAMEPLAY } GameScreen;
const int scale = 10;
const int screenWidth = 64*scale;
const int screenHeight = 32*scale;

int main(int argc, char *argv[])
{



    GameScreen currentScreen = INIT;
    InitAudioDevice();
    Sound beep = LoadSound("untitled.ogg");
    InitWindow(screenWidth, screenHeight, "Chip8 Emulator");

    SetTargetFPS(60);
    int framesCounter = 0;
    int cpuInit = 0;
    int gameInit = 0;
    struct chip8CPU *cpu = NULL;
    if (argc<1){
        printf("You have to specify a file\n");
        return 1;
    }

    //init controller
    //
    //load game

    while (!WindowShouldClose()){
        //background tasks switch
        switch (currentScreen){
        case INIT:
            //init graphics
                if(cpu==NULL){
                    cpu = initCPU();
                }else{
            //init game file
                    if(!loadGame(cpu, argv[1]))
                        return 1;
                    currentScreen = GAMEPLAY;
                }
            break;

        case GAMEPLAY:
        {
            if(cpu->sound_timer>0){
                PlaySound(beep);
                cpu->sound_timer--;
            }else
                StopSound(beep);
            if(cpu->delay_timer>0)
                cpu->delay_timer--;
            if(emulateCycle(cpu))
                break;
            //dumpCPU(cpu);
            return 1;
        }
        }
        //drawing switch
        BeginDrawing();
        switch(currentScreen){
            case INIT:
                {
                DrawText("cpu initing", 0, 0, 20, WHITE);

                if(framesCounter<120){
                    DrawText(TextFormat("cpu stored at %d, %d byes", cpu, sizeof(struct chip8CPU)), 0, 20, 20, WHITE);
                }
                break;
                }
            case GAMEPLAY:
                {
                
                //if drawflag
                //TODO get correct bit from this register
                if(cpu->reg[15]){
                        //TODO actually draw

                }

                //3 draw to screen

                    //ClearBackground(BLACK);
                    drawGFX(cpu->pixels);


                break;

                }
        }
        EndDrawing();
    }
    CloseWindow();        // Close window and OpenGL context

    free(cpu);
    return 0;
}

/*this function written by llm*/
void drawGFX(unsigned char pixels[64*32]){
    ClearBackground(BLACK);
        
        // Draw each pixel
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 64; x++) {
                if (pixels[x+(y*64)] > 0) { // If pixel is "on"
                    DrawRectangle(
                        x * scale, 
                        y * scale, 
                        scale, 
                        scale, 
                        WHITE
                    );
                }
            }
        }
}

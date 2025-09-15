#include "cpu.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

struct chip8CPU *cpu;
typedef enum GameScreen { INIT=0, GAMEPLAY } GameScreen;

int main(int argc, char *argv[])
{

    const int scale = 10;
    const int screenWidth = 64*scale;
    const int screenHeight = 32*scale;


    GameScreen currentScreen = INIT;
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
                    if(!loadGame(cpu, argv[1]))
                        return 1;
                }
                framesCounter++;



                if(framesCounter>=120){
                    currentScreen = GAMEPLAY;
                }
            //init game file
            break;

        case GAMEPLAY:
        {
            if(emulateCycle(cpu))
                break;
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

                    ClearBackground(WHITE);
                    DrawText("no game loaded", 0, 0, 20, BLACK);


                break;

                }
        }
        EndDrawing();
    }
    CloseWindow();        // Close window and OpenGL context

    free(cpu);
    return 0;
}

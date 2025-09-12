#include "cpu.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

struct chip8CPU *cpu;
typedef enum GameScreen { INIT=0, GAMEPLAY } GameScreen;

int main(void)
{

    const int scale = 10;
    const int screenWidth = 64*scale;
    const int screenHeight = 32*scale;


    GameScreen currentScreen = INIT;
    InitWindow(screenWidth, screenHeight, "Chip8 Emulator");

    SetTargetFPS(60);
    int framesCounter = 0;

    //init controller
    //
    //load game

    while (!WindowShouldClose()){
        //background tasks switch
        switch (currentScreen){
        case INIT:
            //init graphics
                if(framesCounter==0){
                    cpu = calloc(1, sizeof(struct chip8CPU));
                }
                framesCounter++;


                if(framesCounter>=120){
                    currentScreen = GAMEPLAY;
                }
            break;

        case GAMEPLAY:
        {
            //put cpu logic here
            //1 get instruction
            //2 perform operation

            break;
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

#include <iostream>
#include <SDL.h>
#include <math.h>
#include "navier-stokes-solver.h"

bool init();
void kill();
bool loop();

SDL_Window* window;
SDL_Renderer* renderer;

NavierStokesSolver solver;
int drawSize = 3;
const int WIDTH=(solver.N+2)*drawSize, HEIGHT=WIDTH;//WIDTH*3/4;

double visc = 0.0001, dt = 0.01, diff = 0.0001, limitVelocity = 100, gasDensity = 10;


static int mouseX, mouseY, oldMouseX, oldMouseY;
static bool mouseDown, mouseDetected;

int main(int argc, char** args){
    if(!init()){
        return 1;
    }

    while(loop()){
        SDL_Delay(10);
    }
    kill();
    return 0;
}

int boxNum(int x, int y){
    return x + (solver.N+2) * y;
}

int sgn(double v) {
  if (v < 0) return -1;
  if (v > 0) return 1;
  return 0;
}

bool loop(){
    static const unsigned char* keys = SDL_GetKeyboardState(NULL);

    SDL_Event e;
    SDL_Rect r;

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    while(SDL_PollEvent(&e) != 0){
        switch(e.type){
            case SDL_QUIT:
                return false;
            case SDL_MOUSEBUTTONDOWN:
                mouseX = e.button.x;
                mouseY = e.button.y;
                mouseDown = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseX = e.button.x;
                mouseY = e.button.y;
                mouseDown = false;
            case SDL_MOUSEMOTION:
                mouseX = e.button.x;
                mouseY = e.button.y;
                break;
        }
    }

    double mouseDx = mouseX - oldMouseX;
    double mouseDy = mouseY - oldMouseY;
    int cellX = (int) floor(mouseX / drawSize);
    int cellY = (int) floor(mouseY / drawSize);
    if(cellX >= 0 && cellX < solver.N+2 && cellY >= 0 && cellY < solver.N+2){
        mouseDx = (abs((float) mouseDx) > limitVelocity) ? sgn(mouseDx) * limitVelocity : mouseDx;
        mouseDy = (abs((float) mouseDy) > limitVelocity) ? sgn(mouseDy) * limitVelocity : mouseDy;

        solver.applyForce(cellX, cellY, mouseDx, mouseDy);

        oldMouseX = mouseX;
        oldMouseY = mouseY;
    }

    if(mouseDown){
        int cellX = (int) floor(mouseX/drawSize);
        int cellY = (int) floor(mouseY/drawSize);
        if(cellX >= 0 && cellX < solver.N+2 && cellY >= 0 && cellY < solver.N+2){
            solver.dense[boxNum(cellX,cellY)] += gasDensity;
        }
    }
    solver.tick(dt, visc, diff);

    for(int y=0; y<solver.N+2; y++){
        for(int x=0; x<solver.N+2; x++){
            double density = (int)(255*solver.dense[boxNum(x,y)]);
            SDL_Rect rect;
            rect.x = x*drawSize;
            rect.y = y*drawSize;
            rect.w = drawSize;
            rect.h = drawSize;
            if(density > 255){
                density = 255;
            }
            SDL_SetRenderDrawColor(renderer,density,density,density,255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
    
    //std::cout<<"X: "<<mouseX<<"\tY: "<<mouseY<<std::endl;

    return true;
}

bool init(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout<<"Error initializing SDL: "<<SDL_GetError()<<std::endl;
        system("pause");
        return false;
    }
    window = SDL_CreateWindow("Fluid Dynamics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(!window){
        std::cout<<"Error creating window: "<<SDL_GetError()<<std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        std::cout<<"Error creating renderer: "<<SDL_GetError()<<std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    return true;
}

void kill(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

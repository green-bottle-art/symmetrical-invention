#include <SDL3/SDL.h>

int main()
{
    // bool succes {true};

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed!");
        return -1;
    }

    SDL_CreateWindow("1080", 320, 240, SDL_WINDOW_RESIZABLE);

    SDL_Delay(5000);

    SDL_Quit();
    return 0;
}
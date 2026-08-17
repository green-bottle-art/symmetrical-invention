/* dari : https://examples.libsdl.org/SDL3/renderer/01-clear/ */
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


//Ww
SDL_Window* window {nullptr};
SDL_Renderer* renderer {nullptr};

/* Run once at startup*/
SDL_AppResult SDL_AppInit(void** appstate , int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.com.com");

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Gagal inisialisasi SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(!SDL_CreateWindowAndRenderer("Example Renderer Clear", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Gagal membuat window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;
}

/* Runs when a new event (mouse input, key presses, etc) occurs*/
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if(event -> type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}


/* Function runs once per frame. bisa dibialang Main Loop*/
SDL_AppResult SDL_AppIterate(void* appstate)
{
    const double now  {((double)SDL_GetTicks()) / 1000.0}; // Convert milisecond to second
    // Choose the color for the frame we will draw
    const float red {(float)(0.5 + 0.5 * SDL_sin(now))};
    const float green {(float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3))};
    const float blue {(float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 /3))};

    SDL_SetRenderDrawColorFloat(renderer,red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;

}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}

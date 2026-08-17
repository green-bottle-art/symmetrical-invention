#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>
#include "class.h"

// Struktur AppState menyimpan objek RAII
struct AppState {
    std::unique_ptr<SDLContext> context;
    bool is_running{true};
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Gagal inisialisasi SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto state = std::make_unique<AppState>();

    // Inisialisasi Window & Renderer melalui wrapper RAII
    state->context = SDLContext::create("SDL3 C++ RAII Wrapper", 800, 600, 0);
    if (!state->context) {
        return SDL_APP_FAILURE;
    }

    *appstate = state.release(); // Pindahkan kepemilikan pointer ke SDL
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* state = static_cast<AppState*>(appstate);
    SDL_Renderer* renderer = state->context->getRenderer();

    /* Render Frame */
    SDL_SetRenderDrawColor(renderer, 24, 24, 32, 255);
    SDL_RenderClear(renderer);

    /* Render Objek Tambahan Di Sini */

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    // Re-wrap raw pointer ke std::unique_ptr
    std::unique_ptr<AppState> state(static_cast<AppState*>(appstate));

    /* 
       Destruktor `state` dan `SDLContext` dipanggil secara otomatis di sini.
       SDL_DestroyRenderer & SDL_DestroyWindow dieksekusi tanpa pemanggilan manual!
    */

    SDL_Quit();
}
#define SDL_MAIN_USE_CALLBACKS 1 // Harus ada sebelum #include
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>

struct AppState
{
    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};
    // bool isRunning {true};
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // 1. Alokasi dengan std::make_unique
    auto state = std::make_unique<AppState>();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 UniquePtr Example", 800, 600, 0, &state->window, &state->renderer)) {
        return SDL_APP_FAILURE;
    }

    // state->is_running = true;

    // 2. Lepaskan kepemilikan ke raw pointer untuk disimpan SDL
    *appstate = state.release();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    // 3. Ambil kembali raw pointer ke dalam unique_ptr
    std::unique_ptr<AppState> state(static_cast<AppState*>(appstate));

    if (state) {
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        // Memori `state` otomatis di-delete saat keluar scope fungsi ini
    }

    SDL_Quit();
}
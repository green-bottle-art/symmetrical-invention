#define SDL_MAIN_USE_CALLBACKS 1 // Harus ada sebelum #include
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


/* Struktur untuk menyimpan seluruh state/data aplikasi*/
struct AppState
{
    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};
    // bool isRunning {true};

    /* Data objek 2D player*/
    SDL_FRect player;
    float speed;

    /* Delta time*/
    Uint64 lastTick;
};

const int V_WIDTH = 800;
const int V_HEIGHT = 600;


/* Menginisialisasi pustaka SDL(SDL_Init) , membuat window, renderer, memuat aset, dan mengalokasikan memori  untuk state aplikasi. Dipanggil sekali saat program dijalankan*/
/* Tempat inisialisasi awal program*/
SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[])
{
    /* Alokasi memori untuk AppState*/
    AppState *state {(AppState *)SDL_calloc(1, sizeof(AppState))};
    if(!state)
    {
        return SDL_APP_FAILURE;
    }
    *appstate = state; // Simpan pointer agar dapat diakses fungsi callback lainya
    
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Gagal inisialisasi SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 Examples", V_WIDTH, V_HEIGHT, 0, &state ->window, &state->renderer))
    {
        SDL_Log("Gagal membuat window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Inisialisasi posisi dan kecepatan player*/
    state->player.w = 50.0f;
    state->player.h = 50.0f;
    state->player.x = (800.0f -state->player.w) / 2.0f;
    state->player.y = (600.0f -state->player.h) / 2.0f;
    state->speed = 300.0f; // Bergerak 300 px perdetik

    state->lastTick = SDL_GetTicks();

    return SDL_APP_CONTINUE;
}

/* Di panggil setiap kali terjadi event baru (key presses, mouse, gamepad, close window)*/
/* Menangani event yang masuk dari pengguna atau sistem */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *state {(AppState *)appstate};
    if (event ->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS; // Program berhasil dijalankan dan berhenti
    }

    return SDL_APP_CONTINUE;
}

/* Di panggil berulang kali setiap frame (Memperbarui posisi objek, collision detection, draw, dan presentRender ) */
/* Tempat logika utama atau perenderan*/
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state {(AppState *)appstate};
    
    /* Delta Time dalam detik */
    Uint64 curretTicks {SDL_GetTicks()};
    float delta {(curretTicks - state->lastTick) / 1000.0f};
    state->lastTick = curretTicks;

    /* Mengambil state keyboard terus menerus untuk pergerakan yang mulus */
    const bool *keys {SDL_GetKeyboardState(nullptr)};

    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
    {
        state->player.y -= state->speed * delta;
    }
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
    {
        state->player.y += state->speed * delta;
    }
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
    {
        state->player.x -= state->speed * delta;
    }
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
    {
        state->player.x += state->speed * delta;
    }

    // Batasi agar player tidak keluar dari layar
    if (state->player.x < 0) state->player.x = 0;
    if (state->player.y < 0) state->player.y = 0;
    if (state->player.x + state->player.w > 800) state->player.x = 800 - state->player.w;
    if (state->player.y + state->player.h > 600) state->player.y = 600 - state->player.h;

    /* Rendering */
    SDL_SetRenderDrawColor(state->renderer, 30, 30,  30, 255);
    SDL_RenderClear(state->renderer);
    /* Tempat render Objek tambahan*/
    /* Render Player */
    // Kotak biru
    SDL_SetRenderDrawColor(state->renderer, 50, 150, 250, 255);
    SDL_RenderFillRect(state->renderer, &state->player);


    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

/* Clean Up*/
/* Di panggil sekali saat akan keluar program*/
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *state {(AppState *)appstate};

    if (state)
    {
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
    SDL_Quit();
}

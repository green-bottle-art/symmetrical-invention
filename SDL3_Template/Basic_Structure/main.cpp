#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    // Inisialisasi SDL
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Gagal inisialisasi SDL error: %s\n", SDL_GetError());
        return -1;
    }


    // Window dan renderer
    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};

    int windowWidth {800};
    int windowHeight {600};

    if(!SDL_CreateWindowAndRenderer("Game 2d SDL3", windowWidth, windowHeight, 0, &window, &renderer))
    {
        SDL_Log("Gagal memuat window error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Game loops Variabel
    bool isRunning{true};
    SDL_Event event;

    // Contoh objek
    float rectX {100.0f};
    float rectY {100.0f};
    float speed {300.0f};

    uint64_t lastTime {SDL_GetTicks()};

    /* Game Loops */

    while(isRunning)
    {
        // calculate delta time
        uint64_t currentTime {SDL_GetTicks()};
        // Konversi ke detik
        float deltaTime {(currentTime - lastTime) / 1000.0f};
        lastTime = currentTime;

        // Input handling (event loop)
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT) isRunning = false;
        }

        // Keyboard state 
        const bool* keyState = SDL_GetKeyboardState(nullptr);
        if(keyState[SDL_SCANCODE_ESCAPE]) isRunning = false;
        if(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) rectX -= speed * deltaTime;
        if(keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) rectX += speed * deltaTime;
        if(keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) rectY -= speed * deltaTime;
        if(keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) rectY += speed * deltaTime;

        // Update logic
        // Logic, Collison detection dll


        // Render section
        // Bersihkan layar dgn vvarna background
        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 155);
        SDL_RenderClear(renderer);

        // Gambar object
        SDL_FRect playerRect {rectX, rectY, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer, 235, 60, 60, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        // tampil gabar
        SDL_RenderPresent(renderer);

    }
    // Clean exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;

}
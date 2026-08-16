#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Gagal inisialisasi SDL error:%s", SDL_GetError());
        return -1;
    }

    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};

    int windowWidth{800};
    int windowHeight{600};

    if (!SDL_CreateWindowAndRenderer("Point and Click", windowWidth, windowHeight, 0, &window, &renderer))
    {
        SDL_Log("Gagal memuat window/renderer error: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Area tombol
    SDL_FRect buttonRect{300.0f, 225.0f, 200.0f, 150.0f}; // X, Y, Lebar, Tinggi

    // Variabel tombol
    bool isPressed{false};
    bool isRunning{true};

    SDL_Event event;

    // Main evvent loop
    while (isRunning)
    {
        // Proses event
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                // Tombol mouse ditekan
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    float mouseX{event.button.x};
                    float mouseY{event.button.y};

                    // Memeriksa apakah cursor berada didalam kotak area tombol
                    if ((mouseX >= buttonRect.x && mouseX <= (buttonRect.x + buttonRect.w)) &&
                        (mouseY >= buttonRect.y && mouseY <= (buttonRect.y + buttonRect.h)))
                    {
                        isPressed = true;
                    }
                }
            }

            // Ketika Mouse dilepas
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isPressed = false;
                }
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // *renderer r, g, b, a
        SDL_RenderClear(renderer);

        if (isPressed)
        {
            // Hijau ketika tombol diklik
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255);
        }
        else
        {
            // Biru
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
        }

        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window);
    SDL_Quit();

    return 0;
}
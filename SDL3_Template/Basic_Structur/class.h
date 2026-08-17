#include <SDL3/SDL.h>
#include <memory>
#include <utility>

// Custom Deleter untuk SDL_Window
struct WindowDeleter {
    void operator()(SDL_Window* window) const {
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
};

// Custom Deleter untuk SDL_Renderer
struct RendererDeleter {
    void operator()(SDL_Renderer* renderer) const {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
    }
};

// Alias Smart Pointer RAII
using WindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;

// Class RAII Wrapper untuk menggabungkan Window dan Renderer
class SDLContext {
public:
    SDLContext() = default;

    // Factory method untuk inisialisasi aman
    static std::unique_ptr<SDLContext> create(const char* title, int width, int height, SDL_WindowFlags flags) {
        auto context = std::make_unique<SDLContext>();

        SDL_Window* raw_window = nullptr;
        SDL_Renderer* raw_renderer = nullptr;

        if (!SDL_CreateWindowAndRenderer(title, width, height, flags, &raw_window, &raw_renderer)) {
            SDL_Log("Gagal membuat window/renderer: %s", SDL_GetError());
            return nullptr;
        }

        context->window.reset(raw_window);
        context->renderer.reset(raw_renderer);

        return context;
    }

    // Hindari Copy (Pola RAII unik)
    SDLContext(const SDLContext&) = delete;
    SDLContext& operator=(const SDLContext&) = delete;

    // Izinkan Move semantics
    SDLContext(SDLContext&&) noexcept = default;
    SDLContext& operator=(SDLContext&&) noexcept = default;

    // Getter untuk mengakses pointer SDL bawaan
    SDL_Window* getWindow() const { return window.get(); }
    SDL_Renderer* getRenderer() const { return renderer.get(); }

private:
    WindowPtr window{nullptr};
    RendererPtr renderer{nullptr};
};
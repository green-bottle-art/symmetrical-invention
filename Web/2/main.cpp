/* cmake -B build && cmake --build build */


/* Headers */
// Using SDL, SDL_image, STL string
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

constexpr int kScreenWidth {640};
constexpr int kScreenHeight {480};

/* Class Prototype*/
class LTexture
{
    public:
        // Initialize texture variable
        LTexture();

        // Cleans up the exture variable
        ~LTexture();

        // Load texture from disks
        bool loadFromFile(std::string path);

        // Cleans up texture
        void destroy();

        //Draws texture
        void render(float x, float y);

        // Gets texture attrubutes
        int getWidth();
        int getHeight();
        bool isLoaded();

    private:
        // Contains texture data
        SDL_Texture* mTexture;

        // Texture Dimensions   
        int mWidth;
        int mHeight;

};

/* Global variable */
// The window we'll be rendering to
SDL_Window* gWindow {nullptr};

// The renderer used to draw the window
SDL_Renderer* gRenderer {nullptr};

// The png image we will render
LTexture gPngTexture;

// Class Implementations 
// LTexture Implementations

LTexture::LTexture():
    // Initialize texture variable
    mTexture {nullptr},
    mWidth {0},
    mHeight {0}
{
}

LTexture::~LTexture()
{
    // Clean up texture
    destroy();
}

bool LTexture::loadFromFile(std::string path)
{
    // Cleam up texture if it already exist
    destroy();

    // Load surface
    if(SDL_Surface* loadedSurface = IMG_Load(path.c_str()); loadedSurface == nullptr)
    {
        SDL_Log("Unable to load  image %s! SDL_image error: %s\n", SDL_GetError());
    }
    else
    {
        // Create texture from surface
        if(mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface); mTexture == nullptr)
        {
            SDL_Log("Unable to load texture from loaded pixels! SDL error: %s\n", SDL_GetError());
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface ->w;
            mHeight = loadedSurface ->h;
        }
        // Clean up loaded surface
        SDL_DestroySurface(loadedSurface);

    }
    // Return success if texture is loaded
    return mTexture != nullptr;
}

void LTexture::destroy()
{
    // Clean up texture
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::render(float x, float y)
{
    // Set texture position
    SDL_FRect dstRect {x, y, static_cast<float>(mWidth), static_cast<float>(mHeight)};

    // Render texture
    SDL_RenderTexture(gRenderer, mTexture, nullptr, &dstRect);
}

int LTexture:: getWidth()
{
    return mWidth;
}
int LTexture::getHeight()
{
    return mHeight;
}
bool LTexture::isLoaded()
{
    return mTexture != nullptr;
}

/* Function Implementations */
bool init()
{
    // Initialization flag
    bool succes {true};

    // Initialize SDl
    if(SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        succes = false;
    }
    else
    {
        // Create window renderer
        if(SDL_CreateWindowAndRenderer("SDL3 Tutorial and Extensions libraries", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer ) == false)
        {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            succes = false;
        }
    }
    return succes;
}   

bool loadMedia()
{
    // File loading flag
    bool succes {true};

    // Load splash image
    if(gPngTexture.loadFromFile("ikan.png") == false)
    {
        SDL_Log("Unable to load png image!\n");
        succes = false;
    }
    return succes;
}

void close()
{
    // Clean up texture
    gPngTexture.destroy();

    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[])
{
    // Final exit code
    int exitCode {0};

    // Initialize
    if(init() == false)
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    }
    else
    {
        // Load media
        if(loadMedia() == false)
        {
            SDL_Log("Unable to load media!\n");
            exitCode = 2;
        }
        else
        {
            // The quit flag
            bool quit {false};

            // The event data
            SDL_Event e;
            SDL_zero(e);

            // The main loop
            while (quit == false)
            {
                // Get event data
                while(SDL_PollEvent(&e) == true)
                {
                    // If event is quit type
                    if(e.type == SDL_EVENT_QUIT)
                    {
                        // End the main loop
                        quit = true;
                    }
                }
                
                // Fill the background white
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render image on screen
                gPngTexture.render(0.0f, 0.0f);
                SDL_RenderPresent(gRenderer);
            }
            
        }
    }

    // Clean up
    close();

    return exitCode;
}
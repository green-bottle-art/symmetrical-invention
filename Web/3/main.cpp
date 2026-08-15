#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

/* Global Variables */

constexpr int kScreenWidth {1000};
constexpr int kScreenHeight {1000};

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

// The window we'll be rendering to
SDL_Window* gWindow {nullptr};

// The renderer used to draw to the window
SDL_Renderer* gRenderer {nullptr};

// The directional images
LTexture gUpTexture, gDownTexture, gLeftTexture, gRightTexture;

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
        SDL_Log("Unable to load  image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError());
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

    // Load directional images
    if(gUpTexture.loadFromFile("img/up.png") == false)
    {
        SDL_Log("Unable to load up image!\n");
        succes = false;
    }
    if(gDownTexture.loadFromFile("img/down.png") == false)
    {
        SDL_Log("Unable to load down image!\n");
        succes = false;
    }
    if(gLeftTexture.loadFromFile("img/Left.png") == false)
    {
        SDL_Log("Unable to load left image!\n");
        succes = false;
    }
    if(gRightTexture.loadFromFile("img/Right.png") == false)
    {
        SDL_Log("Unable to load right image!\n");
        succes = false;
    }

    return succes;
}

void close()
{
    // Clean up texture
    gUpTexture.destroy();
    gDownTexture.destroy();
    gLeftTexture.destroy();
    gRightTexture.destroy();

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

    int exitCode {0};

    if(init() == false)
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    }
    else
    {

        if(loadMedia() == false)
        {
            SDL_Log("Unable to load media!\n");
            exitCode = 2;
        }
        else
        {
              // The quit flag
            bool quit {false};

            // Event data
            SDL_Event e;
            SDL_zero(e);

            // Currently rendered texture
            LTexture* currentTexture = &gUpTexture;

            // Background color defaults to white
            SDL_Color bgColor {0xFF, 0xFF, 0xFF, 0xFF};

            while(quit == false)
            {
                while(SDL_PollEvent (&e) == true)
                {
                    // If event is quit type
                    if(e.type == SDL_EVENT_QUIT)
                    {
                        // End the main loop
                        quit = true;
                    }

                    // on Key press
                    else if(e.type == SDL_EVENT_KEY_DOWN)
                    {
                        // Set texture
                        if(e.key.key == SDLK_UP)
                        {
                            currentTexture = &gUpTexture;
                        }
                        else if(e.key.key == SDLK_DOWN)
                        {
                            currentTexture = &gDownTexture;
                        }
                        else if(e.key.key == SDLK_LEFT)
                        {
                            currentTexture = &gLeftTexture;
                        }
                        else if(e.key.key == SDLK_RIGHT)
                        {
                            currentTexture = &gRightTexture;
                        }
                    }
                }

                // Reset background color to white
                bgColor.r = 0xFF;
                bgColor.g = 0xFF;
                bgColor.b = 0xFF;

                //Set background color based on key state
                const bool* keyStates = SDL_GetKeyboardState(nullptr);
                if(keyStates[SDL_SCANCODE_UP] == true)
                {
                    //Red
                    bgColor.r = 0xFF;
                    bgColor.g = 0x00;
                    bgColor.b = 0x00;
                }
                else if( keyStates[ SDL_SCANCODE_DOWN ] == true )
                {
                    //Green
                    bgColor.r = 0x00;
                    bgColor.g = 0xFF;
                    bgColor.b = 0x00;
                }
                else if( keyStates[ SDL_SCANCODE_LEFT ] == true )
                {
                    //Yellow
                    bgColor.r = 0xFF;
                    bgColor.g = 0xFF;
                    bgColor.b = 0x00;
                }
                else if( keyStates[ SDL_SCANCODE_RIGHT ] == true )
                {
                    //Blue
                    bgColor.r = 0x00;
                    bgColor.g = 0x00;  
                    bgColor.b = 0xFF;
                }

                // Fill background
                SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF );
                SDL_RenderClear(gRenderer);

                // Render image on screen
                currentTexture -> render((kScreenWidth - currentTexture -> getWidth()) / 2.f, (kScreenHeight - currentTexture -> getHeight()) / 2.f);

                // Update screeen
                SDL_RenderPresent(gRenderer);


            }
        }
       

          // Clean up
    close();

    return exitCode;

    }
}


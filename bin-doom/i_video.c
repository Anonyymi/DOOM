#include "i_video.h"
#include <SDL2/SDL.h>
#include "doomdef.h"
#include "i_system.h"
#include "v_video.h"
#include "d_main.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * texture;
uint32_t * framebuffer;
uint32_t palette[256];

void I_InitGraphics(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        I_Error("error initializing SDL2");
    }

    window = SDL_CreateWindow(
        "DOOM",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREENWIDTH,
        SCREENHEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (window == NULL)
    {
        I_Error("error creating SDL_Window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        I_Error("error creating SDL_Renderer");
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        SCREENWIDTH,
        SCREENHEIGHT
    );

    if (texture == NULL)
    {
        I_Error("error creating SDL_Texture");
    }

    framebuffer = malloc(SCREENWIDTH * SCREENHEIGHT * sizeof(uint32_t));

    if (framebuffer == NULL)
    {
        I_Error("error allocating framebuffer");
    }
}

void I_ShutdownGraphics(void)
{
    free(framebuffer);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void I_SetPalette(byte* data)
{
    for (unsigned int i = 0; i < 256; ++i)
    {
        uint32_t r = gammatable[usegamma][*data++];
        uint32_t g = gammatable[usegamma][*data++];
        uint32_t b = gammatable[usegamma][*data++];
        
        palette[i] = ((0 << 24) | (r << 16) | (g << 8) | b);
    }
}

void I_UpdateNoBlit(void)
{
    
}

void I_FinishUpdate(void)
{
    if (framebuffer == NULL)
    {
        return;
    }

    for (unsigned int y = 0; y < SCREENHEIGHT; ++y)
    {
        for (unsigned int x = 0; x < SCREENWIDTH; ++x)
        {
            framebuffer[x + y * SCREENWIDTH] = palette[screens[0][x + y * SCREENWIDTH]];
        }
    }


    SDL_UpdateTexture(
        texture,
        NULL,
        framebuffer,
        SCREENWIDTH * sizeof(uint32_t)
    );
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void I_ReadScreen(byte* scr)
{
    
}

int sdl_keycode_to_doom_keycode(SDL_KeyCode code)
{
    switch (code)
    {
        case SDLK_UP: return KEY_UPARROW;
        case SDLK_DOWN: return KEY_DOWNARROW;
        case SDLK_LEFT: return KEY_LEFTARROW;
        case SDLK_RIGHT: return KEY_RIGHTARROW;
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            return KEY_RCTRL;
        case SDLK_LALT:
        case SDLK_RALT:
            return KEY_RALT;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            return KEY_RSHIFT;
        case SDLK_BACKSPACE: return KEY_BACKSPACE;
        case SDLK_TAB: return KEY_TAB;
        case SDLK_F1: return KEY_F1;
        case SDLK_F2: return KEY_F2;
        case SDLK_F3: return KEY_F3;
        case SDLK_F4: return KEY_F4;
        case SDLK_F5: return KEY_F5;
        case SDLK_F6: return KEY_F6;
        case SDLK_F7: return KEY_F7;
        case SDLK_F8: return KEY_F8;
        case SDLK_F9: return KEY_F9;
        case SDLK_F10: return KEY_F10;
        case SDLK_F11: return KEY_F11;
        case SDLK_F12: return KEY_F12;
        default: return code;
    }
}

void I_GetEvent(void)
{
    event_t doom_event;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                I_Quit();
                break;
            case SDL_KEYDOWN:
                doom_event.type = ev_keydown;
                doom_event.data1 = sdl_keycode_to_doom_keycode(event.key.keysym.sym);
                D_PostEvent(&doom_event);
                break;
            case SDL_KEYUP:
                doom_event.type = ev_keyup;
                doom_event.data1 = sdl_keycode_to_doom_keycode(event.key.keysym.sym);
                D_PostEvent(&doom_event);
                break;
        }
    }
}

void I_StartFrame(void)
{
    I_GetEvent();
}

void I_StartTic(void)
{
    
}

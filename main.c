#include "asdp_define.h"    // Includes SDL, Nuklear, etc.
#include <SDL2/SDL_image.h> // For IMG_Load and PNG loading

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

/* Forward-declare any functions from main_menu.h, play_menu.h, etc. */
extern void render_main_menu(struct nk_context *ctx, MenuState *current_menu);
extern void render_play_menu(struct nk_context *ctx, PlayState *current_play);
extern void render_leaderboards_menu(struct nk_context *ctx, LeaderboardState *current_leaderboard);

/* Global font pointer for the title font */
struct nk_font *title_font = NULL;

/* Global or static textures for background/logo */
static SDL_Texture *g_background_texture = NULL;
static SDL_Texture *g_logo_texture       = NULL;

/* Simple helper to load textures */
static int load_textures(SDL_Renderer *renderer)
{
    /* Load background */
    SDL_Surface *bgSurface = IMG_Load("menu_background.png");
    if (!bgSurface)
    {
        fprintf(stderr, "Could not load menu_background.png: %s\n", IMG_GetError());
        return 0;
    }
    g_background_texture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!g_background_texture)
    {
        fprintf(stderr, "Could not create texture from menu_background.png: %s\n", SDL_GetError());
        return 0;
    }

    /* Load logo */
    SDL_Surface *logoSurface = IMG_Load("logo.png");
    if (!logoSurface)
    {
        fprintf(stderr, "Could not load logo.png: %s\n", IMG_GetError());
        return 0;
    }
    g_logo_texture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);
    if (!g_logo_texture)
    {
        fprintf(stderr, "Could not create texture from logo.png: %s\n", SDL_GetError());
        return 0;
    }

    return 1; /* success */
}

int
main(void)
{
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    /* Also initialize SDL_image for PNG loading */
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        fprintf(stderr, "SDL_image Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Academic Ascension",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        fprintf(stderr, "SDL CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /* Load our background and logo textures */
    if (!load_textures(renderer))
    {
        fprintf(stderr, "Failed to load textures.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    /* Initialize Nuklear with the SDL window and renderer. */
    struct nk_context *ctx = nk_sdl_init(window, renderer);

    /* Set up the font atlas. */
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    struct nk_font *default_font = nk_font_atlas_add_default(atlas, 13, 0);

    /* Load Felipa-Regular.ttf as a large font for the game title */
    struct nk_font_config cfg = nk_font_config(0);
    title_font = nk_font_atlas_add_from_file(atlas, "Felipa-Regular.ttf", 48.0f, &cfg);
    if (!title_font)
    {
        fprintf(stderr, "Failed to load Felipa-Regular.ttf!\n");
        /* handle error if needed */
    }


    nk_sdl_font_stash_end();

    if (!default_font)
    {
        fprintf(stderr, "Failed to load default font!\n");
        nk_sdl_shutdown();
        SDL_DestroyTexture(g_background_texture);
        SDL_DestroyTexture(g_logo_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    nk_style_set_font(ctx, &default_font->handle);

    /* Set initial states. */
    MenuState current_menu = MENU_MAIN;
    PlayState current_play = PLAY_MAIN;
    LeaderboardState current_leaderboard = LEADERBOARD_MAIN;
    int running = 1;
    SDL_Event event;

    /* Main loop */
    while (running)
    {
        /* Feed events to Nuklear */
        nk_input_begin(ctx);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            nk_sdl_handle_event(&event);
        }
        nk_input_end(ctx);

        /* Clear screen first */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Draw the background */
        if (g_background_texture)
        {
            /* Fill entire window with background image */
            SDL_RenderCopy(renderer, g_background_texture, NULL, NULL);
        }

        /* Render the appropriate menu state */
        switch (current_menu)
        {
            case MENU_MAIN:
                render_main_menu(ctx, &current_menu);
                break;

            case MENU_PLAY:
                render_play_menu(ctx, &current_play);
                if (current_play == PLAY_QUIT)
                {
                    current_menu = MENU_MAIN;
                    current_play = PLAY_MAIN; /* reset */
                }
                break;

            case MENU_OPTIONS:
                /* render_options_menu(ctx, &current_option); */
                /* For now, just return to main menu */
                current_menu = MENU_MAIN;
                break;

            case MENU_LEADERBOARDS:
                render_leaderboards_menu(ctx, &current_leaderboard);
                if (current_leaderboard == LEADERBOARD_QUIT)
                {
                    current_menu = MENU_MAIN;
                    current_leaderboard = LEADERBOARD_MAIN; /* reset */
                }
                break;

            case MENU_QUIT:
                running = 0;
                break;

            default:
                break;
        }

        /* Draw Nuklear UI on top of the background */
        nk_sdl_render(NK_ANTI_ALIASING_ON);

        /* Optionally, draw the logo in the bottom-right corner with SDL
           
           if (g_logo_texture)
           {
               int logo_size = 128;
               SDL_Rect dest = {
                   .x = WINDOW_WIDTH - logo_size - 20,
                   .y = WINDOW_HEIGHT - logo_size - 20,
                   .w = logo_size,
                   .h = logo_size
               };
               SDL_RenderCopy(renderer, g_logo_texture, NULL, &dest);
           }
               */

        /* Present everything */
        SDL_RenderPresent(renderer);
    }

    /* Cleanup */
    nk_sdl_shutdown();

    SDL_DestroyTexture(g_background_texture);
    SDL_DestroyTexture(g_logo_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

#ifndef PLAY_MENU_H
#define PLAY_MENU_H

#include "../nuklear_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "states.h"
#include "window.h"

// Pass a pointer to your PlayState so that state changes persist.
static inline void render_play_menu(struct nk_context *ctx, PlayState *current_state)
{
    if (nk_begin(ctx, "Play Menu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
    {
        nk_layout_row_dynamic(ctx, 60, 1);
        if (nk_button_label(ctx, "New Game"))
            *current_state = PLAY_NEWGAME;
        if (nk_button_label(ctx, "Continue"))
            *current_state = PLAY_CONTINUE;
        if (nk_button_label(ctx, "Return to Main Menu"))
            *current_state = PLAY_QUIT;
    }
    nk_end(ctx);
}

/* TODO:
static inline void draw_character_screen(struct nk_context *ctx, PlayState *current_state)
{
    if (*current_state != PLAY_CONTINUE)
    {
        printf("Current state is not correct!");
        exit(EXIT_FAILURE);
    }

    if (nk_begin(ctx, "Current Character", nk_rect(80, 80, 100, 200), 
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
    {
        // For example, define a static layout row:
        nk_layout_row_static(ctx, 30, 100, 1);
        nk_label(ctx, "Character Info", NK_TEXT_LEFT);
    }
    nk_end(ctx);
}
*/

#endif // PLAY_MENU_H

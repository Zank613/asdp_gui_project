#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../nuklear_main.h"
#include "states.h"
#include "window.h"

extern struct nk_font *title_font;

static inline void
render_main_menu(struct nk_context *ctx, MenuState *current_menu)
{

    nk_style_push_style_item(ctx,
        &ctx->style.window.fixed_background,
        nk_style_item_color(nk_rgba(0, 0, 0, 0)));
    
    /* Fullscreen window with no scrollbar/title. */
    if (nk_begin(ctx, "MainMenu", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
    {
        /* Use a space layout covering the full window height. */
        nk_layout_space_begin(ctx, NK_STATIC, WINDOW_HEIGHT, 10);

        /* 1) Title in top-left */
        {
            struct nk_rect title_pos = nk_rect(20, 20, 300, 40);
            nk_layout_space_push(ctx, title_pos);

            /* Push the Felipa font if it loaded successfully */
            if (title_font)
            {
                nk_style_push_font(ctx, &title_font->handle);
            }
 
            /* Pale yellow color (example: 240, 224, 160) */
            nk_label_colored(ctx, "Academic Ascension", NK_TEXT_LEFT, nk_rgb(240, 224, 160));
 
            /* Pop the font if we pushed it */
            if (title_font)
            {
                nk_style_pop_font(ctx);
            }
        }

        /* 2) Menu Buttons near bottom-left */
        {
            int button_width  = 150;
            int button_height = 40;
            int spacing       = 10;
            /* We'll have 4 buttons stacked vertically. */
            int total_buttons_h = 4 * (button_height + spacing);
            /* Position them a bit above the bottom. */
            int start_y = WINDOW_HEIGHT - total_buttons_h - 60;

            /* Play */
            nk_layout_space_push(ctx, nk_rect(20, start_y, button_width, button_height));
            if (nk_button_label(ctx, "Play"))
            {
                *current_menu = MENU_PLAY;
            }

            /* Options */
            nk_layout_space_push(ctx, nk_rect(20, start_y + (button_height + spacing),
                                              button_width, button_height));
            if (nk_button_label(ctx, "Options"))
            {
                *current_menu = MENU_OPTIONS;
            }

            /* Leaderboards */
            nk_layout_space_push(ctx, nk_rect(20, start_y + 2*(button_height + spacing),
                                              button_width, button_height));
            if (nk_button_label(ctx, "Leaderboards"))
            {
                *current_menu = MENU_LEADERBOARDS;
            }

            /* Quit */
            nk_layout_space_push(ctx, nk_rect(20, start_y + 3*(button_height + spacing),
                                              button_width, button_height));
            if (nk_button_label(ctx, "Quit"))
            {
                *current_menu = MENU_QUIT;
            }
        }

        /* 3) Version text in bottom-left corner */
        {
            struct nk_rect ver_pos = nk_rect(20, WINDOW_HEIGHT - 30, 200, 20);
            nk_layout_space_push(ctx, ver_pos);
            nk_label(ctx, "v1.0.1", NK_TEXT_LEFT);
        }

        /* 4) Logo in bottom-right (optional) */
        /* 
           If you have the logo as an nk_image, you can do:
           
           extern struct nk_image g_logo_nk; // declared somewhere
           
           int logo_size = 128;
           struct nk_rect logo_pos = nk_rect(WINDOW_WIDTH - logo_size - 20,
                                             WINDOW_HEIGHT - logo_size - 20,
                                             logo_size, logo_size);
           nk_layout_space_push(ctx, logo_pos);
           nk_image(ctx, g_logo_nk);
        */
        nk_layout_space_end(ctx);
    }
    nk_end(ctx);

    nk_style_pop_style_item(ctx);
}

#endif // MAIN_MENU_H

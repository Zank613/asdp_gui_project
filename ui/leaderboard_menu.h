#ifndef LEADERBOARDS_MENU_H
#define LEADERBOARDS_MENU_H

#include "../nuklear_main.h"
#include "states.h"
#include "window.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Example scoreboard entry structure */
typedef struct ScoreEntry
{
    int rank;              /* Will be recalculated after each sort */
    char name[32];
    char difficulty[16];
    char course[32];
    int score;
} ScoreEntry;

/* Example data: In a real app, fetch these dynamically */
static ScoreEntry g_leaderboard_data[] = {
    {1, "Gerry",           "Very Hard", "ASDP",     355080},
    {2, "Hughes_Callum",   "Easy",      "Games Dev",299999},
    {3, "Miles_Upshur",    "Medium",    "Games Dev",389765},
    {4, "SpadeJackman1",   "Very Hard", "Games Dev",456855},
    {5, "Your_character",  "Medium",    "Games Dev",120000}
};
static int g_leaderboard_count = sizeof(g_leaderboard_data) / sizeof(g_leaderboard_data[0]);

/* Filters (tabs): Daily, Weekly, Monthly, All-Time */
typedef enum LeaderboardFilter
{
    LB_DAILY,
    LB_WEEKLY,
    LB_MONTHLY,
    LB_ALLTIME
} LeaderboardFilter;
static LeaderboardFilter g_filter = LB_DAILY;  /* Current tab filter */

/* Sorting toggles for each column */
static int sort_rank_asc        = 1;
static int sort_name_asc        = 1;
static int sort_difficulty_asc  = 1;
static int sort_score_asc       = 1;

/* Currently selected course for display */
static char g_selected_course[32] = "All Courses";

/* Whether the course popup is open */
static int g_course_popup_open = 0;

/* Example courses for the popup (Arcade removed) */
static const char* g_course_list[] = {
    "ASDP",
    "Games Dev",
    "All Courses"
};
static int g_course_count = 3;

/* ----------------------------------------------------------------------
   Sorting Helpers
   After each qsort, we call recalc_ranks() to assign ranks based on
   the new order (top is rank=1, next is rank=2, etc.).
   ---------------------------------------------------------------------- */

/* Recalculates rank after sorting, so index 0 = rank 1, index 1 = rank 2, etc. */
static void recalc_ranks(void)
{
    for (int i = 0; i < g_leaderboard_count; i++)
    {
        g_leaderboard_data[i].rank = i + 1;
    }
}

static int compare_rank_asc(const void *a, const void *b)
{
    const ScoreEntry *ea = (const ScoreEntry*)a;
    const ScoreEntry *eb = (const ScoreEntry*)b;
    return ea->rank - eb->rank;
}
static int compare_rank_desc(const void *a, const void *b)
{
    return compare_rank_asc(b, a);
}

static int compare_name_asc(const void *a, const void *b)
{
    const ScoreEntry *ea = (const ScoreEntry*)a;
    const ScoreEntry *eb = (const ScoreEntry*)b;
    return strcmp(ea->name, eb->name);
}
static int compare_name_desc(const void *a, const void *b)
{
    return compare_name_asc(b, a);
}

/* Map difficulty strings to integers for sorting */
static int difficulty_to_int(const char *diff)
{
    if (strcmp(diff, "Easy") == 0)      return 1;
    if (strcmp(diff, "Medium") == 0)    return 2;
    if (strcmp(diff, "Very Hard") == 0) return 3;
    /* fallback if needed */
    return 999;
}

static int compare_difficulty_asc(const void *a, const void *b)
{
    const ScoreEntry *ea = (const ScoreEntry*)a;
    const ScoreEntry *eb = (const ScoreEntry*)b;
    return difficulty_to_int(ea->difficulty) - difficulty_to_int(eb->difficulty);
}
static int compare_difficulty_desc(const void *a, const void *b)
{
    return compare_difficulty_asc(b, a);
}

static int compare_score_asc(const void *a, const void *b)
{
    const ScoreEntry *ea = (const ScoreEntry*)a;
    const ScoreEntry *eb = (const ScoreEntry*)b;
    return ea->score - eb->score;
}
static int compare_score_desc(const void *a, const void *b)
{
    return compare_score_asc(b, a);
}

/* ----------------------------------------------------------------------
   Main Rendering Function
   ---------------------------------------------------------------------- */
static inline void
render_leaderboards_menu(struct nk_context *ctx, LeaderboardState *current_leaderboard)
{
    /* Full-screen overlay with no extra label at the top */
    if (nk_begin(ctx, "Leaderboards", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
    {
        /* 1) Filter Tabs: Daily, Weekly, Monthly, All-Time */
        nk_layout_row_dynamic(ctx, 40, 4);
        if (nk_option_label(ctx, "Daily",    g_filter == LB_DAILY))   g_filter = LB_DAILY;
        if (nk_option_label(ctx, "Weekly",   g_filter == LB_WEEKLY))  g_filter = LB_WEEKLY;
        if (nk_option_label(ctx, "Monthly",  g_filter == LB_MONTHLY)) g_filter = LB_MONTHLY;
        if (nk_option_label(ctx, "All-time", g_filter == LB_ALLTIME)) g_filter = LB_ALLTIME;

        /* Some spacing before the table */
        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* 2) Table Header (Rank, Name, Difficulty, Course, Score) */
        nk_layout_row_template_begin(ctx, 30);
        nk_layout_row_template_push_static(ctx, 70);  // Rank column
        nk_layout_row_template_push_static(ctx, 150); // Name column
        nk_layout_row_template_push_static(ctx, 120); // Difficulty column
        nk_layout_row_template_push_static(ctx, 150); // Course column
        nk_layout_row_template_push_static(ctx, 120); // Score column
        nk_layout_row_template_end(ctx);

        /* Header Row */
        /* RANK (Asc/Desc) */
        if (nk_button_label(ctx, sort_rank_asc ? "Rank (Asc)" : "Rank (Desc)"))
        {
            if (sort_rank_asc)
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_rank_desc);
            else
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_rank_asc);
            sort_rank_asc = !sort_rank_asc;
            recalc_ranks();
        }
        /* NAME (Asc/Desc) */
        if (nk_button_label(ctx, sort_name_asc ? "Name (Asc)" : "Name (Desc)"))
        {
            if (sort_name_asc)
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_name_desc);
            else
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_name_asc);
            sort_name_asc = !sort_name_asc;
            recalc_ranks();
        }
        /* DIFFICULTY (Asc/Desc) */
        if (nk_button_label(ctx, sort_difficulty_asc ? "Difficulty (Asc)" : "Difficulty (Desc)"))
        {
            if (sort_difficulty_asc)
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_difficulty_desc);
            else
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_difficulty_asc);
            sort_difficulty_asc = !sort_difficulty_asc;
            recalc_ranks();
        }

        /* COURSE (popup) */
        if (nk_button_label(ctx, g_selected_course))
        {
            g_course_popup_open = !g_course_popup_open;
        }

        /* SCORE (Asc/Desc) */
        if (nk_button_label(ctx, sort_score_asc ? "Score (Asc)" : "Score (Desc)"))
        {
            if (sort_score_asc)
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_score_desc);
            else
                qsort(g_leaderboard_data, g_leaderboard_count, sizeof(ScoreEntry), compare_score_asc);
            sort_score_asc = !sort_score_asc;
            recalc_ranks();
        }

        /* Popup for course selection */
        if (g_course_popup_open)
        {
            /* Position it beneath the last widget (the course button) */
            struct nk_rect bounds = nk_widget_bounds(ctx);
            struct nk_rect popup_rect = nk_rect(bounds.x, bounds.y + bounds.h + 5, 200, 120);

            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Course Select", 0, popup_rect))
            {
                nk_layout_row_dynamic(ctx, 30, 1);
                for (int i = 0; i < g_course_count; i++)
                {
                    if (nk_button_label(ctx, g_course_list[i]))
                    {
                        strncpy(g_selected_course, g_course_list[i], sizeof(g_selected_course));
                        g_course_popup_open = 0;
                        nk_popup_close(ctx);
                    }
                }
                nk_popup_end(ctx);
            }
            else
            {
                g_course_popup_open = 0;
            }
        }

        /* 3) Table Body */
        for (int i = 0; i < g_leaderboard_count; i++)
        {
            /* Filter by course unless it's "All Courses" */
            if (strcmp(g_selected_course, "All Courses") != 0 &&
                strcmp(g_leaderboard_data[i].course, g_selected_course) != 0)
            {
                continue;
            }

            nk_layout_row_template_begin(ctx, 28);
            nk_layout_row_template_push_static(ctx, 70);
            nk_layout_row_template_push_static(ctx, 150);
            nk_layout_row_template_push_static(ctx, 120);
            nk_layout_row_template_push_static(ctx, 150);
            nk_layout_row_template_push_static(ctx, 120);
            nk_layout_row_template_end(ctx);

            /* Highlight "Gerry" in yellow */
            int is_gerry = (strcmp(g_leaderboard_data[i].name, "Gerry") == 0);
            if (is_gerry)
            {
                nk_style_push_color(ctx, &ctx->style.text.color, nk_rgb(255, 255, 0));
            }

            /* RANK */
            char rank_buf[16];
            snprintf(rank_buf, sizeof(rank_buf), "%d", g_leaderboard_data[i].rank);
            nk_label(ctx, rank_buf, NK_TEXT_LEFT);

            /* NAME */
            nk_label(ctx, g_leaderboard_data[i].name, NK_TEXT_LEFT);

            /* DIFFICULTY */
            nk_label(ctx, g_leaderboard_data[i].difficulty, NK_TEXT_LEFT);

            /* COURSE */
            nk_label(ctx, g_leaderboard_data[i].course, NK_TEXT_LEFT);

            /* SCORE */
            char score_buf[32];
            snprintf(score_buf, sizeof(score_buf), "%d", g_leaderboard_data[i].score);
            nk_label(ctx, score_buf, NK_TEXT_LEFT);

            if (is_gerry)
            {
                nk_style_pop_color(ctx);
            }
        }

        /* Some spacing before the bottom button(s) */
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_spacing(ctx, 1);

        /* 4) Return Button */
        nk_layout_row_dynamic(ctx, 45, 1);
        if (nk_button_label(ctx, "Return to Main Menu"))
        {
            *current_leaderboard = LEADERBOARD_QUIT;
        }
    }
    nk_end(ctx);
}

#endif // LEADERBOARDS_MENU_H

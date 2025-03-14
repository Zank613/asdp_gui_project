#ifndef STATES_H
#define STATES_H

typedef enum MenuState
{
    MENU_MAIN,
    MENU_PLAY,
    MENU_OPTIONS,
    MENU_LEADERBOARDS,
    MENU_QUIT
} MenuState;

typedef enum OptionState
{
    OPTIONS_MAIN,
    OPTIONS_GRAPHICS,
    OPTIONS_SOUND,
    OPTIONS_ACCESSIBILITY,
    OPTIONS_HELP,
    OPTIONS_QUIT
} OptionState;

typedef enum PlayState
{
    PLAY_MAIN,
    PLAY_NEWGAME,
    PLAY_CONTINUE,
    PLAY_QUIT
} PlayState;

typedef enum LeaderboardState
{
    LEADERBOARD_MAIN,
    LEADERBOARD_QUIT
} LeaderboardState;

#endif //STATES_H
# Makefile for ASDP_GUI project with SDL2_image (cross-compile support)

CC            = gcc
CFLAGS        = -std=c99 -Wall -Wextra -pedantic -O2 -DSDL_DISABLE_IMMINTRIN_H `sdl2-config --cflags`
LIBS          = -lm -ldl `sdl2-config --libs` -lSDL2_image

WIN_CC        = x86_64-w64-mingw32-gcc
# Adjust include paths if needed
WIN_CFLAGS    = -std=c99 -Wall -Wextra -pedantic -O2 -DSDL_DISABLE_IMMINTRIN_H \
                -I/ataerky/SDL2_win32/SDL2-2.32.2/x86_64-w64-mingw32/include\
                -I/ataerky/SDL2_win32/SDL2-2.32.2/x86_64-w64-mingw32/include/SDL2
# Adjust library paths if needed
WIN_LIBS      = -L/ataerky/x86_64-w64-mingw32/lib \
                -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows -lm

SRC           = main.c
OBJ           = $(SRC:.c=.o)

LINUX_BIN_DIR = bin/linux
WIN_BIN_DIR   = bin/win32

BIN_LINUX     = $(LINUX_BIN_DIR)/demo
BIN_WIN32     = $(WIN_BIN_DIR)/demo.exe

.PHONY: all linux win32 clean

all: linux win32

linux: $(LINUX_BIN_DIR) $(BIN_LINUX)

$(LINUX_BIN_DIR):
	mkdir -p $(LINUX_BIN_DIR)

$(BIN_LINUX): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN_LINUX) $(LIBS)

win32: $(WIN_BIN_DIR) $(BIN_WIN32)

$(WIN_BIN_DIR):
	mkdir -p $(WIN_BIN_DIR)

$(BIN_WIN32): $(SRC)
	$(WIN_CC) $(WIN_CFLAGS) $(SRC) -o $(BIN_WIN32) $(WIN_LIBS)

clean:
	rm -rf $(LINUX_BIN_DIR) $(WIN_BIN_DIR) *.o

# Makefile for ASDP_GUI project with SDL2_image (cross-compile support)

CC            = gcc
CFLAGS        = -std=c99 -Wall -Wextra -pedantic -O2 -DSDL_DISABLE_IMMINTRIN_H `sdl2-config --cflags`
LIBS          = -lm -ldl `sdl2-config --libs` -lSDL2_image

SRC           = main.c
OBJ           = $(SRC:.c=.o)

LINUX_BIN_DIR = bin/linux

BIN_LINUX     = $(LINUX_BIN_DIR)/demo


.PHONY: all linux clean

all: linux

linux: $(LINUX_BIN_DIR) $(BIN_LINUX)

$(LINUX_BIN_DIR):
	mkdir -p $(LINUX_BIN_DIR)

$(BIN_LINUX): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN_LINUX) $(LIBS)


clean:
	rm -rf $(LINUX_BIN_DIR) $(WIN_BIN_DIR) *.o

# Variables
TARGET_LINUX = snake
TARGET_WINDOWS = snake.exe
SRC = snake.c  # Replace with your source file name
CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc

# Compiler Flags
CFLAGS = -Wall
SDL_LIBS_LINUX = `sdl2-config --cflags --libs` -lSDL2_ttf
SDL_LIBS_WINDOWS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

# Default Target
all: linux windows

# Build for Linux
linux:
	$(CC_LINUX) $(CFLAGS) $(SRC) -o $(TARGET_LINUX) $(SDL_LIBS_LINUX)
	@echo "Built $(TARGET_LINUX) for Linux."

# Build for Windows (Cross-compilation)
windows:
	$(CC_WINDOWS) $(CFLAGS) $(SRC) -o $(TARGET_WINDOWS) $(SDL_LIBS_WINDOWS)
	@echo "Built $(TARGET_WINDOWS) for Windows."

# Clean up build artifacts
clean:
	rm -f $(TARGET_LINUX) $(TARGET_WINDOWS)
	@echo "Cleaned up build files."

# Phony targets
.PHONY: all linux windows clean

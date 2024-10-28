# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs -lmingw32 -lSDL2main -lSDL2`

# Executable name and source files
TARGET = snake
SRC = snake.c

# Default target to build the executable
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

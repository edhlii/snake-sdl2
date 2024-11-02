#include "snake.h"
#include "include/SDL2/SDL_render.h"
#include <SDL2/SDL_events.h>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;
const int COLUMNS = 40;
const int ROWS = 30;

// Function prototypes
void spawnFood();
void printScore();
void printText(char[], SDL_Rect);
bool isLose();
void updateSnake();
bool init();
void showMenu();
void shopOptions();
void renderLose();
void closeSDL();
void handleEvents();
void update();
void render();

// SDL variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Game variables
GameObject snake[1000];
GameObject food;
Vec2 direction;
int score = 0;
SDL_Surface* textSurface;
SDL_Texture* textTexture;
SDL_Rect textRect;
TTF_Font* font;
SDL_Color White = {255, 255, 255, 255};
bool quit = false, startGame = false;
int delay = 80000;

// Menu variables
int numModes = 3;
int numOptions = 3;
int selectedMode = 0;
int selectedDiff = 0;

void spawnFood() {
  food.rect = (SDL_Rect) {
    (rand() % COLUMNS) * GRID_SIZE,
    (rand() % ROWS) * GRID_SIZE,
    GRID_SIZE, GRID_SIZE
  };
}

void printScore() {
  SDL_Color White = {255, 255, 255, 255};
  char scoreText[100];
  sprintf(scoreText, "Your score: %d", score);
  SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(font, scoreText, White);
  SDL_Texture* scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
  SDL_Rect scoreTextRect = {10, 10, 200, 50};
  SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);
}

void printText(char text[], SDL_Rect pos) {
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, White);
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_RenderCopy(renderer, textTexture, NULL, &pos);
}

void updateSnake() {
  for (int i = score; i >= 1; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0].rect.x += direction.x * GRID_SIZE;
  snake[0].rect.y += direction.y * GRID_SIZE;
}

bool isLose() {
  if (snake[0].rect.x >= SCREEN_WIDTH ||
      snake[0].rect.x < 0   ||
      snake[0].rect.y >= SCREEN_HEIGHT ||
      snake[0].rect.y < 0) {
    return true;
  }
  for (int i = 1; i <= score; i++) {
    if (snake[0].rect.x == snake[i].rect.x && snake[0].rect.y == snake[i].rect.y) return true;
  }
  return false;
}

bool init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return 1;
  }

  // Create window
  window = SDL_CreateWindow("Snek Game",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH,
                            SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Set render color (background color)
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // Load font
  font = TTF_OpenFont("./fonts/static/NotoSansMono-Bold.ttf", 24);
  if (font == NULL) {
    printf("Failed to load font: %s\n", TTF_GetError());
    return 1;
  }

  snake[0].color = (SDL_Color) {0, 255, 0, 255};
  snake[0].rect = (SDL_Rect) {100, 100, GRID_SIZE, GRID_SIZE};
  direction = (Vec2) {0, 0};
  food.color = (SDL_Color) {255, 0, 0, 255};
  spawnFood();

  return true;
}

void showMenu() {
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        return;
      }
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w: case SDLK_UP:
          selectedMode = (selectedMode - 1 + numModes) % numModes;
          break;
        case SDLK_s: case SDLK_DOWN:
          selectedMode = (selectedMode + 1) % numModes;
          break;
        case SDLK_RETURN:
          running = false;
          break;
        case SDLK_ESCAPE:
          running = false;
          quit = true;
          break;
        }
      }
    }
    SDL_Rect modePos;
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);

    if (selectedMode == 0) {
      modePos = (SDL_Rect) {320, 300, 160, 70};
    } else if (selectedMode == 1) {
      modePos = (SDL_Rect) {320, 380, 160, 70};
    } else if (selectedMode == 2) {
      modePos = (SDL_Rect) {320, 460, 160, 70};
    }
    SDL_RenderFillRect(renderer, &modePos);
    SDL_Rect newGameRect = {320, 300, 160, 70};
    SDL_Rect optionRect = {320, 380, 160, 70};
    SDL_Rect quitGameRect = {320, 460, 160, 70};
    printText("New game", newGameRect);
    printText("Options", optionRect);
    printText("Quit game", quitGameRect);

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }
}

void showOptions() {
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        return;
      }
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w: case SDLK_UP:
          selectedDiff = (selectedDiff - 1 + numOptions) % numOptions;
          break;
        case SDLK_s: case SDLK_DOWN:
          selectedDiff = (selectedDiff + 1) % numOptions;
          break;
        case SDLK_RETURN:
          running = false;
          break;
        case SDLK_ESCAPE:
          running = false;
          quit = true;
          break;
        }
      }
    }
    SDL_Rect modePos;
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);

    if (selectedDiff == 0) {
      modePos = (SDL_Rect) {320, 300, 160, 70};
    } else if (selectedDiff == 1) {
      modePos = (SDL_Rect) {320, 380, 160, 70};
    } else if (selectedDiff == 2) {
      modePos = (SDL_Rect) {320, 460, 160, 70};
    }
    SDL_RenderFillRect(renderer, &modePos);
    SDL_Rect easyRect = {320, 300, 160, 70};
    SDL_Rect normalRect = {320, 380, 160, 70};
    SDL_Rect hardRect = {320, 460, 160, 70};
    printText("Easy", easyRect);
    printText("Normal", normalRect);
    printText("Hard", hardRect);

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }
}

void renderLose() {
  printText("You Lose!", (SDL_Rect) {100, 170, 200, 50});
  printText("Press Enter to go to Menu!", (SDL_Rect) {100, 240, 500, 50});
  SDL_RenderPresent(renderer);
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        return;
      }
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_RETURN:
          running = false;
          quit = false;
          score = 0;
          startGame = false;
          break;
        case SDLK_ESCAPE:
          running = false;
          quit = true;
          break;
        }
      }
    }
  }
}

void closeSDL() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  SDL_Quit();
}

void handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      quit = true;
    }
    else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_w: case SDLK_UP:
        if (direction.y != 0) break;
        direction = (Vec2) {0, -1};
        break;
      case SDLK_s: case SDLK_DOWN:
        if (direction.y != 0) break;
        direction = (Vec2) {0, 1};
        break;
      case SDLK_a: case SDLK_LEFT:
        if (direction.x != 0) break;
        direction = (Vec2) { -1, 0};
        break;
      case SDLK_d: case SDLK_RIGHT:
        if (direction.x != 0) break;
        direction = (Vec2) {1, 0};
        break;
      case SDLK_ESCAPE:
        quit = true;
        break;
      default:
        break;
      }
    }
  }
}

void update() {
  updateSnake();
  if (isLose()) {
    quit = true;
  }
  if (snake[0].rect.x == food.rect.x && snake[0].rect.y == food.rect.y) {
    spawnFood();
    score++;
  }
  usleep(delay);
}

void render() {
  SDL_RenderClear(renderer);

  // Draw snake
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &snake[0].rect);
  for (int i = 1; i <= score; i++) {
    SDL_SetRenderDrawColor(renderer, snake[i].color.r, snake[i].color.g, snake[i].color.b, 255);
    SDL_RenderFillRect(renderer, &snake[i].rect);
  }

  // Draw food
  SDL_SetRenderDrawColor(renderer, food.color.r, food.color.g, food.color.b, 255);
  SDL_RenderFillRect(renderer, &food.rect);

  printScore();

  SDL_RenderPresent(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

int main(int argc, char *args[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
    return -1;
  }

  // Game menu loop
menu:
  while (!startGame && !quit) {
    showMenu();
    if (selectedMode == 2) quit = true;
    else if (selectedMode == 1) showOptions();
    else if (selectedMode == 0) startGame = true;
  }

  // Set difficulty
  if (selectedDiff == 0) delay = 100000;
  else if (selectedDiff == 1) delay = 80000;
  else if (selectedDiff == 2) delay = 50000;

  // Main game loops
  while (!quit) {
    handleEvents();
    update();
    render();
    if (isLose()) {
      renderLose();
      goto menu;
    }
  }

  closeSDL();

  return 0;
}
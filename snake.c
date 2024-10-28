#include "snake.h"

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;
const int COLUMNS = 40;
const int ROWS = 30;
const int DELAY = 80000;

bool quit = false;

// Function prototypes
void spawnFood();
void updateSnake();
bool init();
void closeSDL();
void handleEvents(bool *running);
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

void spawnFood() {
  food.rect = (SDL_Rect) {
    (rand() % COLUMNS) * GRID_SIZE,
    (rand() % ROWS) * GRID_SIZE,
    GRID_SIZE, GRID_SIZE
  };
}

void updateSnake() {
  for (int i = score; i >= 1; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0].rect.x += direction.x * GRID_SIZE;
  snake[0].rect.y += direction.y * GRID_SIZE;
  if (snake[0].rect.x >= SCREEN_WIDTH ||
      snake[0].rect.x < 0   ||
      snake[0].rect.y >= SCREEN_HEIGHT ||
      snake[0].rect.y < 0) {
    quit = true;
  }
  for (int i = 1; i <= score; i++) {
    if (snake[0].rect.x == snake[i].rect.x && snake[0].rect.y == snake[i].rect.y) quit = true;
  }
}

bool init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
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

  snake[0].color = (SDL_Color) {0, 255, 0, 255};
  snake[0].rect = (SDL_Rect) {100, 100, GRID_SIZE, GRID_SIZE};
  direction = (Vec2) {0, 0};
  food.color = (SDL_Color) {255, 0, 0, 255};
  spawnFood();

  return true;
}

void closeSDL() {
  // Destroy renderer and window
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  window = NULL;
  renderer = NULL;

  // Quit SDL
  SDL_Quit();
}

void handleEvents(bool *running) {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      *running = false;
    }
    else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_w:
        // snake[0].color = (SDL_Color) {255, 0, 0};
        if (direction.y != 0) break;
        direction = (Vec2) {0, -1};
        break;
      case SDLK_s:
        // snake[0].color = (SDL_Color) {0, 255, 0};
        if (direction.y != 0) break;
        direction = (Vec2) {0, 1};
        break;
      case SDLK_a:
        // snake[0].color = (SDL_Color) {255, 0, 255};
        if (direction.x != 0) break;
        direction = (Vec2) { -1, 0};
        break;
      case SDLK_d:
        // snake[0].color = (SDL_Color) {0, 0, 255};
        if (direction.x != 0) break;
        direction = (Vec2) {1, 0};
        break;
      case SDLK_ESCAPE:
        *running = false;
        break;
      default:
        break;
      }
    }
  }
}

void update() {
  // Update game logic
  updateSnake();
  if (snake[0].rect.x == food.rect.x && snake[0].rect.y == food.rect.y) {
    spawnFood();
    score++;
  }
  // printf("%d\n", score);
  usleep(DELAY);
}

void render() {
  // Clear the screen with the current draw color
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &snake[0].rect);

  for (int i = 1; i <= score; i++) {
    // Set the draw color for the rectangle (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, snake[i].color.r, snake[i].color.g, snake[i].color.b, 255);
    // Fill the rectangle
    SDL_RenderFillRect(renderer, &snake[i].rect);
  }

  // Draw food
  SDL_SetRenderDrawColor(renderer, food.color.r, food.color.g, food.color.b, 255);
  SDL_RenderFillRect(renderer, &food.rect);

  // Present the current buffer to the screen
  SDL_RenderPresent(renderer);

  // Reset draw color for next frame
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

int main(int argc, char *args[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
    return -1;
  }

  bool running = true;

  // Main game loop
  while (running && !quit) {
    handleEvents(&running); // Process input events
    update();               // Update game logic
    render();               // Render to the screen
  }

  // Clean up and close SDL
  closeSDL();

  return 0;
}
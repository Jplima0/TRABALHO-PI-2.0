#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "inimigo.h"

#define SQUARE_SIZE 20
#define GRID_WIDTH 20
#define GRID_HEIGHT 15


void AtualizarInimigo(Inimigo *inimigo, Vector2 posicaoCabecaJogador); 

typedef struct {
    Vector2 position;
    Color color;
} Food;

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} SnakeSegment;

typedef struct {
    SnakeSegment *segments;
    int length;
    int capacity;
    Color color;
} Snake;

void AddSegment(Snake *snake, Vector2 position) {
    if (snake->length >= snake->capacity) {
        snake->capacity *= 2;
        snake->segments = realloc(snake->segments, snake->capacity * sizeof(SnakeSegment));
    }
    
    snake->segments[snake->length].position = position;
    snake->segments[snake->length].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake->segments[snake->length].speed = (Vector2){SQUARE_SIZE, 0};
    snake->segments[snake->length].color = snake->color;
    snake->length++;
}

void MoveSnake(Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->segments[i].position = snake->segments[i-1].position;
    }
    snake->segments[0].position.x += snake->segments[0].speed.x;
    snake->segments[0].position.y += snake->segments[0].speed.y;
}

void GenerateFood(Food *food, Snake *snake, Inimigo *inimigo) {
    bool validPosition = false;
    
    while (!validPosition) {
        food->position.x = GetRandomValue(0, GRID_WIDTH - 1) * SQUARE_SIZE;
        food->position.y = GetRandomValue(0, GRID_HEIGHT - 1) * SQUARE_SIZE;
        
        validPosition = true;
        for (int i = 0; i < snake->length; i++) {
            if (food->position.x == snake->segments[i].position.x && 
                food->position.y == snake->segments[i].position.y) {
                validPosition = false;
                break;
            }
        }
        
        if (validPosition && VerificarColisaoInimigo(inimigo, food->position)) {
            validPosition = false;
        }
    }
    
    food->color = (Color){
        GetRandomValue(50, 255),
        GetRandomValue(50, 255),
        GetRandomValue(50, 255),
        255
    };
}

bool CheckCollisionWithSelf(Snake *snake) {
    for (int i = 1; i < snake->length; i++) {
        if (snake->segments[0].position.x == snake->segments[i].position.x &&
            snake->segments[0].position.y == snake->segments[i].position.y) {
            return true;
        }
    }
    return false;
}

bool CheckCollisionWithWalls(Snake *snake) {
    return (snake->segments[0].position.x < 0 || 
            snake->segments[0].position.x >= GRID_WIDTH * SQUARE_SIZE ||
            snake->segments[0].position.y < 0 || 
            snake->segments[0].position.y >= GRID_HEIGHT * SQUARE_SIZE);
}

int main(void) {
    const int screenWidth = GRID_WIDTH * SQUARE_SIZE;
    const int screenHeight = GRID_HEIGHT * SQUARE_SIZE;
    
    InitWindow(screenWidth, screenHeight, "Jogo da Cobrinha com Raylib");
    SetTargetFPS(10);
    
    Snake snake = {0};
    snake.capacity = 10;
    snake.segments = malloc(snake.capacity * sizeof(SnakeSegment));
    snake.color = GREEN;
    
    AddSegment(&snake, (Vector2){5 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    AddSegment(&snake, (Vector2){4 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    AddSegment(&snake, (Vector2){3 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    
    Inimigo inimigo;
    InicializarInimigo(&inimigo);
    
    Food food = {0};
    GenerateFood(&food, &snake, &inimigo);
    
    bool gameOver = false;
    int score = 0;
    
    while (!WindowShouldClose()) {
        if (!gameOver) {
            if (IsKeyPressed(KEY_RIGHT) && snake.segments[0].speed.x == 0) {
                snake.segments[0].speed = (Vector2){SQUARE_SIZE, 0};
            } else if (IsKeyPressed(KEY_LEFT) && snake.segments[0].speed.x == 0) {
                snake.segments[0].speed = (Vector2){-SQUARE_SIZE, 0};
            } else if (IsKeyPressed(KEY_UP) && snake.segments[0].speed.y == 0) {
                snake.segments[0].speed = (Vector2){0, -SQUARE_SIZE};
            } else if (IsKeyPressed(KEY_DOWN) && snake.segments[0].speed.y == 0) {
                snake.segments[0].speed = (Vector2){0, SQUARE_SIZE};
            }
            
            MoveSnake(&snake);
            AtualizarInimigo(&inimigo, snake.segments[0].position);
            
            if (snake.segments[0].position.x == food.position.x && 
                snake.segments[0].position.y == food.position.y) {
                AddSegment(&snake, snake.segments[snake.length-1].position);
                GenerateFood(&food, &snake, &inimigo);
                score += 10;
            }
            
            if (CheckCollisionWithSelf(&snake) || 
                CheckCollisionWithWalls(&snake) || 
                VerificarColisaoInimigo(&inimigo, snake.segments[0].position)) {
                gameOver = true;
            }
        } else if (IsKeyPressed(KEY_SPACE)) {
            free(snake.segments);
            snake.length = 0;
            snake.capacity = 10;
            snake.segments = malloc(snake.capacity * sizeof(SnakeSegment));
            
            AddSegment(&snake, (Vector2){5 * SQUARE_SIZE, 5 * SQUARE_SIZE});
            AddSegment(&snake, (Vector2){4 * SQUARE_SIZE, 5 * SQUARE_SIZE});
            AddSegment(&snake, (Vector2){3 * SQUARE_SIZE, 5 * SQUARE_SIZE});
            
            InicializarInimigo(&inimigo);
            GenerateFood(&food, &snake, &inimigo);
            gameOver = false;
            score = 0;
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            for (int i = 0; i < GRID_WIDTH; i++) {
                for (int j = 0; j < GRID_HEIGHT; j++) {
                    DrawRectangleLines(i * SQUARE_SIZE, j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
                }
            }
            
            DrawRectangle(food.position.x, food.position.y, SQUARE_SIZE, SQUARE_SIZE, food.color);
            DesenharInimigo(&inimigo);
            
            for (int i = 0; i < snake.length; i++) {
                Color segmentColor = (i == 0) ? DARKGREEN : snake.color;
                DrawRectangle(snake.segments[i].position.x, snake.segments[i].position.y, 
                             SQUARE_SIZE, SQUARE_SIZE, segmentColor);
            }
            
            DrawText(TextFormat("Pontos: %d", score), 10, 10, 20, DARKGRAY);
            
            if (gameOver) {
                DrawText("GAME OVER", screenWidth/2 - MeasureText("GAME OVER", 40)/2, screenHeight/2 - 40, 40, RED);
                DrawText("Pressione ESPACO para reiniciar", 
                         screenWidth/2 - MeasureText("Pressione ESPACO para reiniciar", 20)/2, 
                         screenHeight/2 + 10, 20, DARKGRAY);
            }
        EndDrawing();
    }
    
    free(snake.segments);
    CloseWindow();
    return 0;
}

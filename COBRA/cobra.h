#ifndef COBRA_H
#define COBRA_H

#include "raylib.h"
#include "inimigo.h"

#define SQUARE_SIZE 20
#define GRID_WIDTH 20
#define GRID_HEIGHT 15

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

// Funções da cobra
void AddSegment(Snake *snake, Vector2 position);
void MoveSnake(Snake *snake);
void GenerateFood(Food *food, Snake *snake, Inimigo inimigos[NUM_INIMIGOS]);
bool CheckCollisionWithSelf(Snake *snake);
bool CheckCollisionWithWalls(Snake *snake);

// Funções do jogo
void InicializarJogo(cobra *cobra, Inimigo *inimigo, Food *food);
void ReiniciarJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool *gameOver, int *score);
void ProcessarInput(cobra *cobra);
void AtualizarJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool *gameOver, int *score);
void DesenharJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool gameOver, int score);

#endif // COBRA_H

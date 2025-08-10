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
} cobraSegment;

typedef struct {
    cobraSegment *segmentos;
    int length;
    int capacity;
    Color color;
} cobra;

// Funções da cobra
void AddSegment(cobra *cobra, Vector2 position);
void Movecobra(cobra *cobra);
void GenerateFood(Food *food, cobra *cobra, Inimigo *inimigo);
bool CheckCollisionWithSelf(cobra *cobra);
bool CheckCollisionWithWalls(cobra *cobra);

// Funções do jogo
void InicializarJogo(cobra *cobra, Inimigo *inimigo, Food *food);
void ReiniciarJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool *gameOver, int *score);
void ProcessarInput(cobra *cobra);
void AtualizarJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool *gameOver, int *score);
void DesenharJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool gameOver, int score);

#endif // COBRA_H

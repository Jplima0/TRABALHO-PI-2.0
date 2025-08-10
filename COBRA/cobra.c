#include "cobra.h"
#include <stdlib.h>
#include <time.h>

void AddSegment(cobra *cobra, Vector2 position) {
    if (cobra->length >= cobra->capacity) {
        cobra->capacity *= 2;
        cobra->segmentos = realloc(cobra->segmentos, cobra->capacity * sizeof(cobraSegment));
    }
    
    cobra->segmentos[cobra->length].position = position;
    cobra->segmentos[cobra->length].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    cobra->segmentos[cobra->length].speed = (Vector2){SQUARE_SIZE, 0};
    cobra->segmentos[cobra->length].color = cobra->color;
    cobra->length++;
}

void Movecobra(cobra *cobra) {
    for (int i = cobra->length - 1; i > 0; i--) {
        cobra->segmentos[i].position = cobra->segmentos[i-1].position;
    }
    cobra->segmentos[0].position.x += cobra->segmentos[0].speed.x;
    cobra->segmentos[0].position.y += cobra->segmentos[0].speed.y;
}

void GenerateFood(Food *food, cobra *cobra, Inimigo *inimigo) {
    bool validPosition = false;
    
    while (!validPosition) {
        food->position.x = GetRandomValue(0, GRID_WIDTH - 1) * SQUARE_SIZE;
        food->position.y = GetRandomValue(0, GRID_HEIGHT - 1) * SQUARE_SIZE;
        
        validPosition = true;
        for (int i = 0; i < cobra->length; i++) {
            if (food->position.x == cobra->segmentos[i].position.x && 
                food->position.y == cobra->segmentos[i].position.y) {
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

bool CheckCollisionWithSelf(cobra *cobra) {
    for (int i = 1; i < cobra->length; i++) {
        if (cobra->segmentos[0].position.x == cobra->segmentos[i].position.x &&
            cobra->segmentos[0].position.y == cobra->segmentos[i].position.y) {
            return true;
        }
    }
    return false;
}

bool CheckCollisionWithWalls(cobra *cobra) {
    return (cobra->segmentos[0].position.x < 0 || 
            cobra->segmentos[0].position.x >= GRID_WIDTH * SQUARE_SIZE ||
            cobra->segmentos[0].position.y < 0 || 
            cobra->segmentos[0].position.y >= GRID_HEIGHT * SQUARE_SIZE);
}

void InicializarJogo(cobra *cobra, Inimigo *inimigo, Food *food) {
    cobra->capacity = 10;
    cobra->segmentos = malloc(cobra->capacity * sizeof(cobraSegment));
    cobra->length = 0;
    cobra->color = GREEN;
    
    AddSegment(cobra, (Vector2){5 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    AddSegment(cobra, (Vector2){4 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    AddSegment(cobra, (Vector2){3 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    
    InicializarInimigo(inimigo);
    GenerateFood(food, cobra, inimigo);
}

void ReiniciarJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool *gameOver, int *score) {
    free(cobra->segmentos);
    cobra->length = 0;
    cobra->capacity = 10;
    cobra->segmentos = malloc(cobra->capacity * sizeof(cobraSegment));
    
    AddSegment(cobra, (Vector2){5 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    AddSegment(cobra, (Vector2){4 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    AddSegment(cobra, (Vector2){3 * SQUARE_SIZE, 5 * SQUARE_SIZE});
    
    InicializarInimigo(inimigo);
    GenerateFood(food, cobra, inimigo);
    *gameOver = false;
    *score = 0;
}

void ProcessarInput(cobra *cobra) {
    if (IsKeyPressed(KEY_RIGHT) && cobra->segmentos[0].speed.x == 0) {
        cobra->segmentos[0].speed = (Vector2){SQUARE_SIZE, 0};
    } else if (IsKeyPressed(KEY_LEFT) && cobra->segmentos[0].speed.x == 0) {
        cobra->segmentos[0].speed = (Vector2){-SQUARE_SIZE, 0};
    } else if (IsKeyPressed(KEY_UP) && cobra->segmentos[0].speed.y == 0) {
        cobra->segmentos[0].speed = (Vector2){0, -SQUARE_SIZE};
    } else if (IsKeyPressed(KEY_DOWN) && cobra->segmentos[0].speed.y == 0) {
        cobra->segmentos[0].speed = (Vector2){0, SQUARE_SIZE};
    }
}

void AtualizarJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool *gameOver, int *score) {
    Movecobra(cobra);
    AtualizarInimigo(inimigo);
    
    if (cobra->segmentos[0].position.x == food->position.x && 
        cobra->segmentos[0].position.y == food->position.y) {
        AddSegment(cobra, cobra->segmentos[cobra->length-1].position);
        GenerateFood(food, cobra, inimigo);
        *score += 10;
    }
    
    if (CheckCollisionWithSelf(cobra) || 
        CheckCollisionWithWalls(cobra) || 
        VerificarColisaoInimigo(inimigo, cobra->segmentos[0].position)) {
        *gameOver = true;
    }
}

void DesenharJogo(cobra *cobra, Inimigo *inimigo, Food *food, bool gameOver, int score) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Grade
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                DrawRectangleLines(i * SQUARE_SIZE, j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
            }
        }
        
        // Comida
        DrawRectangle(food->position.x, food->position.y, SQUARE_SIZE, SQUARE_SIZE, food->color);
        
        // Inimigo
        DesenharInimigo(inimigo);
        
        // Cobra
        for (int i = 0; i < cobra->length; i++) {
            Color segmentColor = (i == 0) ? DARKGREEN : cobra->color;
            DrawRectangle(cobra->segmentos[i].position.x, cobra->segmentos[i].position.y, 
                         SQUARE_SIZE, SQUARE_SIZE, segmentColor);
        }
        
        // Pontuação
        DrawText(TextFormat("Pontos: %d", score), 10, 10, 20, DARKGRAY);
        
        // Game Over
        if (gameOver) {
            int screenWidth = GRID_WIDTH * SQUARE_SIZE;
            int screenHeight = GRID_HEIGHT * SQUARE_SIZE;
            
            DrawText("GAME OVER", screenWidth/2 - MeasureText("GAME OVER", 40)/2, screenHeight/2 - 40, 40, RED);
            DrawText("Pressione ESPACO para reiniciar", 
                     screenWidth/2 - MeasureText("Pressione ESPACO para reiniciar", 20)/2, 
                     screenHeight/2 + 10, 20, DARKGRAY);
        }
    EndDrawing();
}

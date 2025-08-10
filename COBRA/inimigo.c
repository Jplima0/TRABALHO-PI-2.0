#include "inimigo.h"
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define SQUARE_SIZE 20
#define GRID_WIDTH 20
#define GRID_HEIGHT 15

// Implementação da função auxiliar
static bool Vector2Equals(Vector2 a, Vector2 b) {
    return (a.x == b.x) && (a.y == b.y);
}

void InicializarInimigo(Inimigo *inimigo) {
    int startX = GetRandomValue(1, GRID_WIDTH - INIMIGO_SIZE - 1);
    int startY = GetRandomValue(1, GRID_HEIGHT - 2);
    
    int direcao = GetRandomValue(0, 3);
    Vector2 speed = {0};
    
    switch (direcao) {
        case 0: speed = (Vector2){SQUARE_SIZE, 0}; break;
        case 1: speed = (Vector2){-SQUARE_SIZE, 0}; break;
        case 2: speed = (Vector2){0, -SQUARE_SIZE}; break;
        case 3: speed = (Vector2){0, SQUARE_SIZE}; break;
    }
    
    for (int i = 0; i < INIMIGO_SIZE; i++) {
        inimigo->segmentos[i].position = (Vector2){
            (startX + i) * SQUARE_SIZE, 
            startY * SQUARE_SIZE
        };
        inimigo->segmentos[i].speed = speed;
        inimigo->segmentos[i].color = RED;
    }
    inimigo->direcaoMudancaTimer = GetRandomValue(5, 15);
}

void AtualizarInimigo(Inimigo *inimigo) {
    inimigo->direcaoMudancaTimer--;
    if (inimigo->direcaoMudancaTimer <= 0) {
        Vector2 novaSpeed = inimigo->segmentos[0].speed;
        
        while (Vector2Equals(novaSpeed, inimigo->segmentos[0].speed)) {
            switch (GetRandomValue(0, 3)) {
                case 0: novaSpeed = (Vector2){SQUARE_SIZE, 0}; break;
                case 1: novaSpeed = (Vector2){-SQUARE_SIZE, 0}; break;
                case 2: novaSpeed = (Vector2){0, -SQUARE_SIZE}; break;
                case 3: novaSpeed = (Vector2){0, SQUARE_SIZE}; break;
            }
        }
        
        for (int i = 0; i < INIMIGO_SIZE; i++) {
            inimigo->segmentos[i].speed = novaSpeed;
        }
        inimigo->direcaoMudancaTimer = GetRandomValue(10, 20);
    }
    
    for (int i = INIMIGO_SIZE - 1; i > 0; i--) {
        inimigo->segmentos[i].position = inimigo->segmentos[i-1].position;
    }
    inimigo->segmentos[0].position.x += inimigo->segmentos[0].speed.x;
    inimigo->segmentos[0].position.y += inimigo->segmentos[0].speed.y;
    
    if (inimigo->segmentos[0].position.x < 0 || 
        inimigo->segmentos[0].position.x >= (GRID_WIDTH - 1) * SQUARE_SIZE ||
        inimigo->segmentos[0].position.y < 0 || 
        inimigo->segmentos[0].position.y >= (GRID_HEIGHT - 1) * SQUARE_SIZE) {
        
        for (int i = 0; i < INIMIGO_SIZE; i++) {
            inimigo->segmentos[i].speed.x *= -1;
            inimigo->segmentos[i].speed.y *= -1;
        }
    }
}

void DesenharInimigo(Inimigo *inimigo) {
    for (int i = 0; i < INIMIGO_SIZE; i++) {
        DrawRectangle(
            inimigo->segmentos[i].position.x + 1,
            inimigo->segmentos[i].position.y + 1, 
            SQUARE_SIZE - 2, SQUARE_SIZE - 2, 
            (i == 0) ? MAROON : RED);
    }
}

bool VerificarColisaoInimigo(Inimigo *inimigo, Vector2 posicao) {
    for (int i = 0; i < INIMIGO_SIZE; i++) {
        if (Vector2Equals(inimigo->segmentos[i].position, posicao)) {
            return true;
        }
    }
    return false;
}

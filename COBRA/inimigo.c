// inimigo.c
#include "inimigo.h"
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include <math.h>

#define SQUARE_SIZE 20
#define GRID_WIDTH 20
#define GRID_HEIGHT 15

void InicializarInimigo(Inimigo *inimigo) {
    // Posição inicial aleatória, garantindo que não comece em cima do jogador
    int startX, startY;
    do {
        startX = GetRandomValue(0, GRID_WIDTH - 1);
        startY = GetRandomValue(0, GRID_HEIGHT - 1);
    } while ((startX >= 3 && startX <= 7) && (startY >= 3 && startY <= 7)); // Evita spawn perto do jogador
    
    inimigo->segmentos[0].position = (Vector2){
        startX * SQUARE_SIZE, 
        startY * SQUARE_SIZE
    };
    inimigo->segmentos[0].speed = (Vector2){0, 0};
    inimigo->segmentos[0].color = RED;
    inimigo->direcao = GetRandomValue(0, 3); // Direção aleatória inicial
}

void AtualizarInimigo(Inimigo *inimigo, Vector2 posicaoCabecaJogador) {
    // Calcula diferença de posição com o jogador
    float diffX = posicaoCabecaJogador.x - inimigo->segmentos[0].position.x;
    float diffY = posicaoCabecaJogador.y - inimigo->segmentos[0].position.y;

    // Só muda de direção quando estiver alinhado com a grade
    bool alinhadoX = ((int)inimigo->segmentos[0].position.x % SQUARE_SIZE) == 0;
    bool alinhadoY = ((int)inimigo->segmentos[0].position.y % SQUARE_SIZE) == 0;

    if (alinhadoX && alinhadoY) {
        // Decide a direção baseada na maior diferença
        if (fabsf(diffX) > fabsf(diffY)) {
            if (diffX > 0) {
                inimigo->direcao = DIRECAO_DIREITA;
            } else {
                inimigo->direcao = DIRECAO_ESQUERDA;
            }
        } else {
            if (diffY > 0) {
                inimigo->direcao = DIRECAO_BAIXO;
            } else {
                inimigo->direcao = DIRECAO_CIMA;
            }
        }
    }

    // Move o inimigo baseado na direção atual
    switch (inimigo->direcao) {
        case DIRECAO_CIMA:
            inimigo->segmentos[0].position.y -= VELOCIDADE_INIMIGO;
            break;
        case DIRECAO_BAIXO:
            inimigo->segmentos[0].position.y += VELOCIDADE_INIMIGO;
            break;
        case DIRECAO_ESQUERDA:
            inimigo->segmentos[0].position.x -= VELOCIDADE_INIMIGO;
            break;
        case DIRECAO_DIREITA:
            inimigo->segmentos[0].position.x += VELOCIDADE_INIMIGO;
            break;
    }

    // Verifica colisão com paredes e ajusta a posição
    if (inimigo->segmentos[0].position.x < 0) {
        inimigo->segmentos[0].position.x = 0;
        inimigo->direcao = DIRECAO_DIREITA;
    } 
    else if (inimigo->segmentos[0].position.x >= (GRID_WIDTH - 1) * SQUARE_SIZE) {
        inimigo->segmentos[0].position.x = (GRID_WIDTH - 1) * SQUARE_SIZE;
        inimigo->direcao = DIRECAO_ESQUERDA;
    } 
    else if (inimigo->segmentos[0].position.y < 0) {
        inimigo->segmentos[0].position.y = 0;
        inimigo->direcao = DIRECAO_BAIXO;
    } 
    else if (inimigo->segmentos[0].position.y >= (GRID_HEIGHT - 1) * SQUARE_SIZE) {
        inimigo->segmentos[0].position.y = (GRID_HEIGHT - 1) * SQUARE_SIZE;
        inimigo->direcao = DIRECAO_CIMA;
    }

    // Garante que o inimigo fique alinhado com a grade quando necessário
    if (alinhadoX) {
        inimigo->segmentos[0].position.x = roundf(inimigo->segmentos[0].position.x / SQUARE_SIZE) * SQUARE_SIZE;
    }
    if (alinhadoY) {
        inimigo->segmentos[0].position.y = roundf(inimigo->segmentos[0].position.y / SQUARE_SIZE) * SQUARE_SIZE;
    }
}

void DesenharInimigo(Inimigo *inimigo) {
    DrawRectangle(
        inimigo->segmentos[0].position.x,
        inimigo->segmentos[0].position.y,
        SQUARE_SIZE,
        SQUARE_SIZE,
        RED
    );
    
    // Desenha olhos para indicar a direção
    int eyeSize = SQUARE_SIZE / 5;
    int eyeOffset = SQUARE_SIZE / 4;
    
    switch (inimigo->direcao) {
        case DIRECAO_CIMA:
            DrawCircle(inimigo->segmentos[0].position.x + eyeOffset, inimigo->segmentos[0].position.y + eyeOffset, eyeSize, WHITE);
            DrawCircle(inimigo->segmentos[0].position.x + SQUARE_SIZE - eyeOffset, inimigo->segmentos[0].position.y + eyeOffset, eyeSize, WHITE);
            break;
        case DIRECAO_BAIXO:
            DrawCircle(inimigo->segmentos[0].position.x + eyeOffset, inimigo->segmentos[0].position.y + SQUARE_SIZE - eyeOffset, eyeSize, WHITE);
            DrawCircle(inimigo->segmentos[0].position.x + SQUARE_SIZE - eyeOffset, inimigo->segmentos[0].position.y + SQUARE_SIZE - eyeOffset, eyeSize, WHITE);
            break;
        case DIRECAO_ESQUERDA:
            DrawCircle(inimigo->segmentos[0].position.x + eyeOffset, inimigo->segmentos[0].position.y + eyeOffset, eyeSize, WHITE);
            DrawCircle(inimigo->segmentos[0].position.x + eyeOffset, inimigo->segmentos[0].position.y + SQUARE_SIZE - eyeOffset, eyeSize, WHITE);
            break;
        case DIRECAO_DIREITA:
            DrawCircle(inimigo->segmentos[0].position.x + SQUARE_SIZE - eyeOffset, inimigo->segmentos[0].position.y + eyeOffset, eyeSize, WHITE);
            DrawCircle(inimigo->segmentos[0].position.x + SQUARE_SIZE - eyeOffset, inimigo->segmentos[0].position.y + SQUARE_SIZE - eyeOffset, eyeSize, WHITE);
            break;
    }
}

bool VerificarColisaoInimigo(Inimigo *inimigo, Vector2 posicao) {
    return (inimigo->segmentos[0].position.x < posicao.x + SQUARE_SIZE &&
            inimigo->segmentos[0].position.x + SQUARE_SIZE > posicao.x &&
            inimigo->segmentos[0].position.y < posicao.y + SQUARE_SIZE &&
            inimigo->segmentos[0].position.y + SQUARE_SIZE > posicao.y);
}

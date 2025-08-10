#include "inimigo.h"
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include <math.h>

#define SQUARE_SIZE 20
#define GRID_WIDTH 30
#define GRID_HEIGHT 25

void InicializarInimigos(Inimigo inimigos[NUM_INIMIGOS]) {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        int startX, startY;
        do {
            startX = GetRandomValue(0, GRID_WIDTH - 1);
            startY = GetRandomValue(0, GRID_HEIGHT - 1);
        } while ((startX >= 3 && startX <= 7) && (startY >= 3 && startY <= 7));
        
        inimigos[i].segmentos[0].position = (Vector2){
            startX * SQUARE_SIZE, 
            startY * SQUARE_SIZE
        };
        inimigos[i].segmentos[0].speed = (Vector2){0, 0};
        inimigos[i].segmentos[0].color = RED;
        inimigos[i].direcao = GetRandomValue(0, 3);
    }
}

void AtualizarInimigos(Inimigo inimigos[NUM_INIMIGOS], Vector2 posicaoCabecaJogador) {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        float diffX = posicaoCabecaJogador.x - inimigos[i].segmentos[0].position.x;
        float diffY = posicaoCabecaJogador.y - inimigos[i].segmentos[0].position.y;

        bool alinhadoX = ((int)inimigos[i].segmentos[0].position.x % SQUARE_SIZE) == 0;
        bool alinhadoY = ((int)inimigos[i].segmentos[0].position.y % SQUARE_SIZE) == 0;

        if (alinhadoX && alinhadoY) {
            if (fabsf(diffX) > fabsf(diffY)) {
                if (diffX > 0) {
                    inimigos[i].direcao = DIRECAO_DIREITA;
                } else {
                    inimigos[i].direcao = DIRECAO_ESQUERDA;
                }
            } else {
                if (diffY > 0) {
                    inimigos[i].direcao = DIRECAO_BAIXO;
                } else {
                    inimigos[i].direcao = DIRECAO_CIMA;
                }
            }
        }

        switch (inimigos[i].direcao) {
            case DIRECAO_CIMA:
                inimigos[i].segmentos[0].position.y -= VELOCIDADE_INIMIGO;
                break;
            case DIRECAO_BAIXO:
                inimigos[i].segmentos[0].position.y += VELOCIDADE_INIMIGO;
                break;
            case DIRECAO_ESQUERDA:
                inimigos[i].segmentos[0].position.x -= VELOCIDADE_INIMIGO;
                break;
            case DIRECAO_DIREITA:
                inimigos[i].segmentos[0].position.x += VELOCIDADE_INIMIGO;
                break;
        }

        if (inimigos[i].segmentos[0].position.x < 0) {
            inimigos[i].segmentos[0].position.x = 0;
            inimigos[i].direcao = DIRECAO_DIREITA;
        } 
        else if (inimigos[i].segmentos[0].position.x >= (GRID_WIDTH - 1) * SQUARE_SIZE) {
            inimigos[i].segmentos[0].position.x = (GRID_WIDTH - 1) * SQUARE_SIZE;
            inimigos[i].direcao = DIRECAO_ESQUERDA;
        } 
        else if (inimigos[i].segmentos[0].position.y < 0) {
            inimigos[i].segmentos[0].position.y = 0;
            inimigos[i].direcao = DIRECAO_BAIXO;
        } 
        else if (inimigos[i].segmentos[0].position.y >= (GRID_HEIGHT - 1) * SQUARE_SIZE) {
            inimigos[i].segmentos[0].position.y = (GRID_HEIGHT - 1) * SQUARE_SIZE;
            inimigos[i].direcao = DIRECAO_CIMA;
        }

        if (alinhadoX) {
            inimigos[i].segmentos[0].position.x = roundf(inimigos[i].segmentos[0].position.x / SQUARE_SIZE) * SQUARE_SIZE;
        }
        if (alinhadoY) {
            inimigos[i].segmentos[0].position.y = roundf(inimigos[i].segmentos[0].position.y / SQUARE_SIZE) * SQUARE_SIZE;
        }
    }
}

void DesenharInimigos(Inimigo inimigos[NUM_INIMIGOS]) {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        DrawRectangle(
            inimigos[i].segmentos[0].position.x,
            inimigos[i].segmentos[0].position.y,
            SQUARE_SIZE,
            SQUARE_SIZE,
            RED
        );
    }
}

bool VerificarColisaoComInimigos(Inimigo inimigos[NUM_INIMIGOS], Vector2 posicao) {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        if (inimigos[i].segmentos[0].position.x < posicao.x + SQUARE_SIZE &&
            inimigos[i].segmentos[0].position.x + SQUARE_SIZE > posicao.x &&
            inimigos[i].segmentos[0].position.y < posicao.y + SQUARE_SIZE &&
            inimigos[i].segmentos[0].position.y + SQUARE_SIZE > posicao.y) {
            return true;
        }
    }
    return false;
}

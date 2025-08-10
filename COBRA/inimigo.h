#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"

#define NUM_INIMIGOS 30
#define INIMIGO_SIZE 1
#define VELOCIDADE_INIMIGO 0.1f

typedef enum {
    DIRECAO_CIMA,
    DIRECAO_BAIXO,
    DIRECAO_ESQUERDA,
    DIRECAO_DIREITA
} Direcao;

typedef struct {
    Vector2 position;
    Vector2 speed;
    Color color;
} InimigoSegmento;

typedef struct {
    InimigoSegmento segmentos[INIMIGO_SIZE];
    Direcao direcao;
} Inimigo;

void InicializarInimigos(Inimigo inimigos[NUM_INIMIGOS]);
void AtualizarInimigos(Inimigo inimigos[NUM_INIMIGOS], Vector2 posicaoCabecaJogador);
void DesenharInimigos(Inimigo inimigos[NUM_INIMIGOS]);
bool VerificarColisaoComInimigos(Inimigo inimigos[NUM_INIMIGOS], Vector2 posicao);

#endif

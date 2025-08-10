#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"

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

void InicializarInimigo(Inimigo *inimigo);
void AtualizarInimigo(Inimigo *inimigo, Vector2 posicaoCabecaJogador);
void DesenharInimigo(Inimigo *inimigo);
bool VerificarColisaoInimigo(Inimigo *inimigo, Vector2 posicao);

#endif

#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"

#define INIMIGO_SIZE 5

typedef struct {
    Vector2 position;
    Vector2 speed;
    Color color;
} InimigoSegmento;

typedef struct {
    InimigoSegmento segmentos[INIMIGO_SIZE];
    int direcaoMudancaTimer;
} Inimigo;

void InicializarInimigo(Inimigo *inimigo);
void AtualizarInimigo(Inimigo *inimigo);
void DesenharInimigo(Inimigo *inimigo);
bool VerificarColisaoInimigo(Inimigo *inimigo, Vector2 posicao);

#endif

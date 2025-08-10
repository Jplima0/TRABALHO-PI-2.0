#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "inimigo.h"
#include "cobra.h"
#include "menu.h"

typedef enum {
    JOGO_COBRA,
    JOGO_OUTRO,  // Adicione outros jogos aqui
    NUM_JOGOS
} TipoJogo;

void RodarJogoCobra() {
    const int screenWidth = GRID_WIDTH * SQUARE_SIZE;
    const int screenHeight = GRID_HEIGHT * SQUARE_SIZE;
    
    InitWindow(screenWidth, screenHeight, "Jogo da Cobrinha");
    SetTargetFPS(10);
    
    cobra cobra;
    Inimigo inimigo;
    Food food;
    bool gameOver = false;
    int score = 0;
    
    InicializarJogo(&cobra, &inimigo, &food);
    
    while (!WindowShouldClose()) {
        if (!gameOver) {
            ProcessarInput(&cobra);
            AtualizarJogo(&cobra, &inimigo, &food, &gameOver, &score);
        } else if (IsKeyPressed(KEY_SPACE)) {
            ReiniciarJogo(&cobra, &inimigo, &food, &gameOver, &score);
        }
        
        DesenharJogo(&cobra, &inimigo, &food, gameOver, score);
    }
    
    free(cobra.segmentos);
    CloseWindow();
}

int main(void) {
    // Loop principal do arcade
    while (1) {
        // Mostra o menu e obtém a seleção do usuário
        int opcao = IniciarMenu();
        
        // Verifica se o usuário quer sair
        if (opcao == 1) {
            break;
        }
        
        // Seleciona o jogo baseado na opção do menu
        switch (opcao) {
            case 0:  // Jogo da Cobra
                RodarJogoCobra();
                break;
                
            // Adicione outros casos para outros jogos
            // case 2: RodarJogoOutro();
            //   break;
                
            default:
                break;
        }
    }
    
    return 0;
}

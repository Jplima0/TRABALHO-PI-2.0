#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// Protótipo da função do menu de jogos
void ExibirMenuJogos();

int main(void) 
{
    // Inicializa o menu principal
    int resultado = IniciarMenu();
    
    // Se o usuário clicou em "Iniciar" (retorno 0)
    if (resultado == 0) {
        ExibirMenuJogos();
    }
    
    return 0;
}

// Implementação do menu de seleção de jogos
void ExibirMenuJogos() 
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Seleção de Jogos");
    SetTargetFPS(60);
    
    // Cores para os botões
    Color LIGHTGREEN = (Color){144, 238, 144, 255};
    Color LIGHTBLUE = (Color){173, 216, 230, 255};
    Color LIGHTPURPLE = (Color){221, 160, 221, 255};
    
    Color corJogo1 = GREEN;
    Color corJogo2 = BLUE;
    Color corJogo3 = PURPLE;
    
    // Retângulos para os botões
    Rectangle botaoJogo1 = {screenWidth/2 - 100, 200, 200, 50};
    Rectangle botaoJogo2 = {screenWidth/2 - 100, 300, 200, 50};
    Rectangle botaoJogo3 = {screenWidth/2 - 100, 400, 200, 50};
    Rectangle botaoVoltar = {screenWidth/2 - 50, 500, 100, 40};

    InitAudioDevice();
    Music musica = LoadMusicStream("menu/IntroLoop.wav");
    musica.looping = true;  
    PlayMusicStream(musica);
    
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        UpdateMusicStream(musica); 
        
        // Efeito hover nos botões
        corJogo1 = CheckCollisionPointRec(mouse, botaoJogo1) ? LIGHTGREEN : GREEN;
        corJogo2 = CheckCollisionPointRec(mouse, botaoJogo2) ? LIGHTBLUE : BLUE;
        corJogo3 = CheckCollisionPointRec(mouse, botaoJogo3) ? LIGHTPURPLE : PURPLE;
        
        // Verificar cliques
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, botaoJogo1)) {
                // Implementar a chamada para o Jogo 1
                printf("Iniciando Jogo 1...\n");
            }
            else if (CheckCollisionPointRec(mouse, botaoJogo2)) {
                // Implementar a chamada para o Jogo 2
                printf("Iniciando Jogo 2...\n");
            }
            else if (CheckCollisionPointRec(mouse, botaoJogo3)) {
                // Implementar a chamada para o Jogo 3
                printf("Iniciando Jogo 3...\n");
            }
            else if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                break; // Voltar para o menu principal
            }
        }
        
        // Desenhar
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Título
            DrawText("SELECIONE SEU JOGO", screenWidth/2 - MeasureText("SELECIONE SEU JOGO", 40)/2, 100, 40, DARKGRAY);
            
            // Botões
            DrawRectangleRec(botaoJogo1, corJogo1);
            DrawRectangleRec(botaoJogo2, corJogo2);
            DrawRectangleRec(botaoJogo3, corJogo3);
            DrawRectangleRec(botaoVoltar, LIGHTGRAY);
            
            // Texto dos botões
            int fontSize = 18; // Tamanho reduzido da fonte

        DrawText("cobrinha", botaoJogo1.x + (botaoJogo1.width - MeasureText("cobrinha", fontSize))/2, botaoJogo1.y + 15, fontSize, WHITE);
        DrawText("pula_catraca", botaoJogo2.x + (botaoJogo2.width - MeasureText("pula_catraca", fontSize))/2, botaoJogo2.y + 15, fontSize, WHITE);
        DrawText("flappy bird", botaoJogo3.x + (botaoJogo3.width - MeasureText("flappy bird", fontSize))/2, botaoJogo3.y + 15, fontSize, WHITE);
        DrawText("Sair", botaoVoltar.x + (botaoVoltar.width - MeasureText("Voltar", fontSize))/2, botaoVoltar.y + 10, fontSize, BLACK);
        EndDrawing();
    }
    UnloadMusicStream(musica);
    CloseAudioDevice();
    CloseWindow();
}

#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef enum {
    MENU_PRINCIPAL,
    CONFIGURACOES
} EstadoTela;


int IniciarMenu() {
    int largura = 800; 
    int altura = 600;
    int saiu = 0;
    int clicou_x = 0;

    InitWindow(largura, altura, "Menu");
    SetTargetFPS(60);
    InitAudioDevice();

    Texture2D background = LoadTexture("menu.png");
    Texture2D icon = LoadTexture("configuracao.png");
    Texture2D pergaminho = LoadTexture("pergaminho.png");
    

    Color LIGHTGREEN = (Color){144, 238, 144, 255};  
    Color LIGHTCORAL = (Color){240, 128, 128, 255}; 

    Color corBotaoIniciar = GREEN;
    Color corBotaoSair = RED;

    Rectangle botaoIniciar = {largura/2 - 100, 330, 200, 50};
    Rectangle botaoSair = {largura/2 - 50, 400, 100, 50};
    Rectangle botaoConfiguracao = {largura - 60, 20, 40, 40};
    Rectangle botaoVoltar = {largura/2 + 90, 450, 75, 40};
    Rectangle desligarSom = {largura/2 - 160, 180, 115, 40};
    Rectangle telacheia = {largura/2 - 160, 230, 190, 40};

    EstadoTela telaAtual = MENU_PRINCIPAL;

    PlayMusicStream(musica);

    bool somLigado = true;
    bool telaCheia = false;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (CheckCollisionPointRec(mouse, botaoIniciar)) {
            corBotaoIniciar = LIGHTGREEN;
        } else {
            corBotaoIniciar = GREEN;
        }

        if (CheckCollisionPointRec(mouse, botaoSair)) {
            corBotaoSair = LIGHTCORAL; 
        } else {
            corBotaoSair = RED;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (telaAtual == MENU_PRINCIPAL) {
                if (CheckCollisionPointRec(mouse, botaoIniciar)) {
                    clicou_x = 1;
                    break; //sair do menu
                }
                if (CheckCollisionPointRec(mouse, botaoSair)) {
                    saiu = 1;
                    clicou_x = 1;
                    break; 
                }
                if (CheckCollisionPointRec(mouse, botaoConfiguracao)) {
                    telaAtual = CONFIGURACOES;
                }
            } else if (telaAtual == CONFIGURACOES) {
                if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                    telaAtual = MENU_PRINCIPAL;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Rectangle sourceRecBg = { 0, 0, (float)background.width, (float)background.height };
        Rectangle destRecBg = { 0, 0, (float)largura, (float)altura };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(background, sourceRecBg, destRecBg, origin, 0.0f, WHITE);

        if(telaAtual != CONFIGURACOES) {
            Rectangle sourceRecIcon = { 0, 0, (float)icon.width, (float)icon.height };
            DrawTexturePro(icon, sourceRecIcon, botaoConfiguracao, origin, 0.0f, WHITE);
        }
        if (telaAtual == MENU_PRINCIPAL) {
            UpdateMusicStream(musica); 
            DrawRectangleRec(botaoIniciar, corBotaoIniciar);
            DrawRectangleRec(botaoSair, corBotaoSair);

            int fontSize = 50;
    
            // Calcula posições separadas para cada parte do texto
            const char* jailsonText = "Jailson's";
            const char* arcadeText = " Arcade";
    
            // Mede o comprimento de cada parte
            int jailsonWidth = MeasureText(jailsonText, fontSize);
            int arcadeWidth = MeasureText(arcadeText, fontSize);
    
            // Calcula posição X inicial para centralizar o texto completo
            int totalWidth = jailsonWidth + arcadeWidth;
            int x_start = largura/2 - totalWidth/2;
            int y_start = 80;

            // Cores personalizadas
            Color roxoJailson = (Color){128, 0, 128, 255};       // Roxo
            Color laranjaArcade = (Color){255, 165, 0, 255};     // Laranja

            // Desenha sombra primeiro
            DrawText(jailsonText, x_start + 3, y_start + 3, fontSize, BLACK);
            DrawText(arcadeText, x_start + jailsonWidth + 3, y_start + 3, fontSize, BLACK);
    
            // Desenha texto principal
            DrawText(jailsonText, x_start, y_start, fontSize, roxoJailson);
            DrawText(arcadeText, x_start + jailsonWidth, y_start, fontSize, laranjaArcade);

            DrawText("Iniciar", 355, 340, 30, BLACK);  
            DrawText("Sair", 370, 410, 30, BLACK);

  
              
        } else if (telaAtual == CONFIGURACOES) {
            UpdateMusicStream(musica);

            Rectangle sourceRecPergaminho = { 0, 0, (float)pergaminho.width, (float)pergaminho.height };
            Rectangle destRecPergaminho = { largura/2 - 350, altura/2 - 320, 700, 650 };
            DrawTexturePro(pergaminho, sourceRecPergaminho, destRecPergaminho, origin, 0.0f, WHITE);

            DrawText("CONFIGURAÇÕES", 310, 130, 20, BLACK);
            
            DrawRectangleRec(desligarSom, LIGHTGRAY);
            DrawRectangleRec(telacheia, LIGHTGRAY);
            DrawRectangleRec(botaoVoltar, LIGHTGRAY);
            DrawText("Voltar", largura/2 + 95, 460, 20, BLACK);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, desligarSom)) {
                    somLigado = !somLigado;
                    if (somLigado) {
                        PlayMusicStream(musica);
                    } else {
                        StopMusicStream(musica);
                    }
                }
            }

            if (somLigado) {
                DrawText("Som: [ON]", desligarSom.x + 5, desligarSom.y + 10, 20, BLACK);
            } else {
                DrawText("Som: [OFF]", desligarSom.x + 5, desligarSom.y + 10, 20, DARKGRAY);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, telacheia)) {
                    telaCheia = !telaCheia;
                    ToggleFullscreen();
                    if (!telaCheia && IsWindowFullscreen()) {
                        SetWindowSize(largura, altura);
                    }
                }
            }
            
            if (telaCheia) {
                DrawText("Tela Cheia: [ON]", telacheia.x + 5, telacheia.y + 10, 20, BLACK);
            } else {
                DrawText("Tela Cheia: [OFF]", telacheia.x + 5, telacheia.y + 10, 20, DARKGRAY);
            }
        }

        EndDrawing();
    }   

    UnloadTexture(background);
    UnloadTexture(icon);
    UnloadTexture(pergaminho);
    CloseWindow();
    if(clicou_x == 1){
        if(saiu){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

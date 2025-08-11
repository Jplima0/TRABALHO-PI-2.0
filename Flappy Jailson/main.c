#include <raylib.h>
#include <stdlib.h>
#include <time.h>

// Estrutura do personagem 
typedef struct {
    float x;
    float y;
    float velocidade;
    Rectangle bateuCano; // espaco delimitado para colissão do personagem com o cano 
    Texture2D texturaPular; // imagem de seu jailson pulando
    Texture2D texturaCair; // imagem de seu jailson caindo
} Personagem;

// Estrutura dos canos 
typedef struct {
    float x;
    float alturaTopo;
    float alturaBase;
    bool passou;
    Rectangle retanguloTopo;
    Rectangle retanguloBase;
    float velocidadeVertical;
    int direcaoVertical;
} Cano;

typedef enum {
    FLAPPY_NIVEL1,
    FLAPPY_NIVEL2,
    FIM_DE_JOGO
} EstadoTela;

int main(void) {
    // Dimensão da janela 
    int largura = 800;
    int altura = 600;

    // Física do personagem 
    float gravidade = 1500.0f;
    float pulo = -500.0f;

    // Informações do cano 
    int larguraCano = 80;
    int espacamentoCano = 160;
    int distanciaCano = 300;
    
    InitWindow(largura, altura, "Flappy Bird");

    SetTargetFPS(60);

    // Define a aleatoriedade baseado no tempo 
    srand(time(NULL));

    EstadoTela telaAtual = FLAPPY_NIVEL1;
    float velocidadeCano = 250.0f;
    int pontuacao = 0;
    int maiorPontuacao = 0;

    // Dimensão do personagem 
    float larguraPersonagem = 90.0f;
    float alturaPersonagem = 90.0f;
    
    // Dimensão do bloco de colisão
    float larguraHitbox = 30.0f;
    float alturaHitbox = 45.0f;

    // Inicialização do sprite 
    Personagem personagem;
    personagem.x = 100;
    personagem.y = altura / 2;
    personagem.velocidade = 0;
    personagem.bateuCano.width = larguraHitbox;
    personagem.bateuCano.height = alturaHitbox;
    
    personagem.texturaPular = LoadTexture("pular.png");
    personagem.texturaCair = LoadTexture("cair.png");
    Texture2D background = LoadTexture("ceuf.png");
    
    Cano canos[5];
    int quantidadeCanos = 5;

    
    void inicializarCanos() {
        for (int i = 0; i < quantidadeCanos; i++) {
            // Posiciona cada cano com um espaço fixo entre um e outro
            canos[i].x = largura + i * distanciaCano;
            
            // Calcula a posição do espaço entre eles
            float posicaoYdoEspaco = (float)(rand() % (altura - espacamentoCano - 100) + 50);
            
            canos[i].alturaTopo = posicaoYdoEspaco;
            canos[i].passou = false;

            // Cano superior
            canos[i].retanguloTopo.x = canos[i].x;
            canos[i].retanguloTopo.y = 0;
            canos[i].retanguloTopo.width = larguraCano;
            canos[i].retanguloTopo.height = canos[i].alturaTopo;

            // Cano inferior 
            canos[i].retanguloBase.x = canos[i].x;
            canos[i].retanguloBase.y = posicaoYdoEspaco + espacamentoCano;
            canos[i].retanguloBase.width = larguraCano;
            canos[i].retanguloBase.height = altura - canos[i].retanguloBase.y;
            
            // Movimento vertical do cano pra fase 2
            canos[i].velocidadeVertical = (float)(rand() % 100 + 50);
            canos[i].direcaoVertical = (rand() % 2 == 0) ? 1 : -1;
        }
    }

    inicializarCanos();

    while (!WindowShouldClose()) {
        float deltaTempo = GetFrameTime();

        switch (telaAtual) {
            case FLAPPY_NIVEL1: {
                // Aplicação da gravidade 
                personagem.velocidade += gravidade * deltaTempo;
                
                // Aplicação do pulo
                if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    personagem.velocidade = pulo;
                }

                personagem.y += personagem.velocidade * deltaTempo;
                
                personagem.bateuCano.x = personagem.x + (larguraPersonagem - larguraHitbox) / 2.0f;
                personagem.bateuCano.y = personagem.y + (alturaPersonagem - alturaHitbox) / 2.0f;

                // MOvimnetação do cano 
                for (int i = 0; i < quantidadeCanos; i++) {
                    canos[i].x -= velocidadeCano * deltaTempo;

                    // Aumenta a velocidade do nivel 2 e os canos de cima vao se mover 
                    if (velocidadeCano == 300.0f) {
                        float moveY = canos[i].velocidadeVertical * canos[i].direcaoVertical * deltaTempo;
                        canos[i].retanguloTopo.y += moveY;
                        canos[i].retanguloBase.y += moveY;
                        canos[i].retanguloBase.height = altura - canos[i].retanguloBase.y;

                        // Quando o cano chegar no limite ele volta 
                        if (canos[i].retanguloTopo.y <= -50 || canos[i].retanguloTopo.y >= 50) {
                            canos[i].direcaoVertical *= -1;
                        }
                    }

                    // Reposicionamento do cano 
                    if (canos[i].x + larguraCano < 0) {
                        float maiorX = 0;
                        for (int j = 0; j < quantidadeCanos; j++) {
                            if (canos[j].x > maiorX) maiorX = canos[j].x;
                        }
                        canos[i].x = maiorX + distanciaCano;
                        
                        // Recalcula a altura e posição do novo cano
                        float posicaoYdoEspaco = (float)(rand() % (altura - espacamentoCano - 100) + 50);
                        canos[i].alturaTopo = posicaoYdoEspaco;
                        canos[i].passou = false;
                        
                        canos[i].retanguloTopo.y = 0;
                        canos[i].retanguloTopo.height = canos[i].alturaTopo;

                        canos[i].retanguloBase.y = posicaoYdoEspaco + espacamentoCano;
                        canos[i].retanguloBase.height = altura - canos[i].retanguloBase.y;

                        canos[i].velocidadeVertical = (float)(rand() % 100 + 50);
                        canos[i].direcaoVertical = (rand() % 2 == 0) ? 1 : -1;
                    }

                    canos[i].retanguloTopo.x = canos[i].x;
                    canos[i].retanguloBase.x = canos[i].x;

                    // Pontuação 
                    if (!canos[i].passou && canos[i].x + larguraCano < personagem.x) {
                        pontuacao++;
                        canos[i].passou = true;
                    }
                }

                // Quando atingir 10 vai pro proximo nivel
                if (pontuacao >= 5 && velocidadeCano == 250.0f) {
                    telaAtual = FLAPPY_NIVEL2;
                    velocidadeCano = 0;
                }

                // Detecta colisão
                for (int i = 0; i < quantidadeCanos; i++) {
                    if (CheckCollisionRecs(personagem.bateuCano, canos[i].retanguloTopo) ||
                        CheckCollisionRecs(personagem.bateuCano, canos[i].retanguloBase)) {
                        telaAtual = FIM_DE_JOGO;
                    }
                }

                // Se o personagem cair ou voar demais ele perde tambem 
                Rectangle chao = {0, altura - 80, largura, 80};
                if (CheckCollisionRecs(personagem.bateuCano, chao)) {
                    personagem.y = altura - 80 - (alturaPersonagem - alturaHitbox) / 2 - alturaHitbox;
                    personagem.velocidade = 0;
                    telaAtual = FIM_DE_JOGO;
                }

                // Colisão com o teto (continua a mesma)
                if (personagem.y < 0) {
                    telaAtual = FIM_DE_JOGO;
                }
            } break;

            case FLAPPY_NIVEL2: {
                if (IsKeyPressed(KEY_ENTER)) {
                    velocidadeCano = 300.0f;
                    inicializarCanos();
                    telaAtual = FLAPPY_NIVEL1;
                }
            } break;

            case FIM_DE_JOGO: {
                // Atualiza o recorde
                if (pontuacao > maiorPontuacao) {
                    maiorPontuacao = pontuacao;
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    personagem.y = altura / 2 - 20;
                    personagem.velocidade = 0;
                    pontuacao = 0;
                    velocidadeCano = 250.0f;
                    inicializarCanos();
                    telaAtual = FLAPPY_NIVEL1;
                }
            } break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(background, 0, 0, WHITE);
            
            Rectangle origem_textura_completa = {0, 0, personagem.texturaPular.width, personagem.texturaPular.height};
            Rectangle destino_personagem = {personagem.x, personagem.y, larguraPersonagem, alturaPersonagem};
            Vector2 ponto_de_origem = {0, 0};

            // Define a textura se ta pulando ou caindo
            if (personagem.velocidade < 0) {
                DrawTexturePro(personagem.texturaPular, origem_textura_completa, destino_personagem, ponto_de_origem, 0.0f, WHITE);
            } else {
                DrawTexturePro(personagem.texturaCair, origem_textura_completa, destino_personagem, ponto_de_origem, 0.0f, WHITE);
            }

            // Desenho dos canos 
            for (int i = 0; i < quantidadeCanos; i++) {
                if (velocidadeCano == 300.0f) {
                    DrawRectangle(canos[i].retanguloTopo.x, 0, canos[i].retanguloTopo.width, canos[i].retanguloTopo.y + canos[i].alturaTopo, GREEN);
                } else {
                    DrawRectangleRec(canos[i].retanguloTopo, GREEN);
                }
                DrawRectangleRec(canos[i].retanguloBase, GREEN);
            }

            // Desenho do chao 
            DrawRectangle(0, altura - 80, largura, 80, DARKGREEN);
            DrawRectangle(0, altura - 60, largura, 60, DARKBROWN);
            DrawText(TextFormat("Pontuação: %d", pontuacao), 10, 10, 20, DARKBLUE);
            DrawText(TextFormat("Recorde: %d", maiorPontuacao), 10, 40, 20, RED);

            if (telaAtual == FLAPPY_NIVEL2) {
                DrawText("FASE 2 DESBLOQUEADO!", largura / 2 - 150, altura / 2 - 40, 30, BLUE);
                DrawText("Aperte ENTER para continuar", largura / 2 - 180, altura / 2 + 10, 20, BLACK);
            }
            if (telaAtual == FIM_DE_JOGO) {
                DrawText("GAME OVER", largura / 2 - 100, altura / 2 - 40, 40, RED);
                DrawText("Pressione ENTER ou CLIQUE para reiniciar", largura / 2 - 190, altura / 2 + 10, 20, DARKGRAY);
            }
        EndDrawing();
    }

    UnloadTexture(personagem.texturaPular);
    UnloadTexture(personagem.texturaCair);
    CloseWindow();
    return 0;
}

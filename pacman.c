#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "pacman.h"
#include "mapa.h"
#include "ui.h"
#include "mapa.c"
#include "ui.c"
#include "estatisticas.c"

// Variáveis globais
MAPA m;
POSICAO heroi;
int tempilula = 0;
int pontuacao = 0;
const char* fases[] = {
    "fase1.txt",
    "fase2.txt",
    "fase3.txt",
    "fase4.txt",
    "fase5.txt"
};

// Funções do menu
void inicializajogo();
void exibeestatisticas();
void atualizapontuacao();
void menu();

int ehdirecao(char direcao) {
    return direcao == ESQUERDA || 
           direcao == CIMA || 
           direcao == BAIXO || 
           direcao == DIREITA;
}

void move(char direcao) {
    int proximox = heroi.x;
    int proximoy = heroi.y;

    switch(direcao) {
        case ESQUERDA: proximoy--; break;
        case CIMA: proximox--; break;
        case BAIXO: proximox++; break;
        case DIREITA: proximoy++; break;
    }

    if (!podeandar(&m, HEROI, proximox, proximoy)) return;

    if (ehpersonagem(&m, PILULA, proximox, proximoy)) {
        tempilula = 1;
        pontuacao += 10; // Incrementa pontuação ao pegar pílula
    }

    andanomapa(&m, heroi.x, heroi.y, proximox, proximoy);
    heroi.x = proximox;
    heroi.y = proximoy;
}

int praondefantasmavai(int xatual, int yatual, int* xdestino, int* ydestino) {
    int opcoes[4][2] = {
        {xatual, yatual + 1}, 
        {xatual + 1, yatual},  
        {xatual, yatual - 1}, 
        {xatual - 1, yatual}
    };

    srand(time(0));
    for (int i = 0; i < 10; i++) {
        int posicao = rand() % 4;

        if (podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])) {
            *xdestino = opcoes[posicao][0];
            *ydestino = opcoes[posicao][1];
            return 1;
        }
    }

    return 0;
}

void fantasmas() {
    MAPA copia;
    copiamapa(&copia, &m);

    for (int i = 0; i < copia.linhas; i++) {
        for (int j = 0; j < copia.colunas; j++) {
            if (copia.matriz[i][j] == FANTASMA) {
                int xdestino, ydestino;

                if (praondefantasmavai(i, j, &xdestino, &ydestino)) {
                    andanomapa(&m, i, j, xdestino, ydestino);
                }
            }
        }
    }

    liberamapa(&copia);
}

void explodepilula2(int x, int y, int somax, int somay, int qtd) {
    if (qtd == 0) return;

    int novox = x + somax;
    int novoy = y + somay;

    if (!ehvalida(&m, novox, novoy)) return;
    if (ehparede(&m, novox, novoy)) return;

    m.matriz[novox][novoy] = VAZIO;
    explodepilula2(novox, novoy, somax, somay, qtd - 1);
}

void explodepilula() {
    if (!tempilula) return;

    explodepilula2(heroi.x, heroi.y, 0, 1, 3);
    explodepilula2(heroi.x, heroi.y, 0, -1, 3);
    explodepilula2(heroi.x, heroi.y, 1, 0, 3);
    explodepilula2(heroi.x, heroi.y, -1, 0, 3);

    tempilula = 0;
}

void inicializajogo(Estatisticas* stats, int numeroFase) {
    char nomeArquivo[20];
    sprintf(nomeArquivo, "fase%d.txt", numeroFase);

    FILE* f = fopen(nomeArquivo, "r");
    if (f == NULL) {
        printf("Erro ao carregar a fase %d.\n", numeroFase);
        exit(1);
    }
    fclose(f);

    lemapa(&m, nomeArquivo); // Modifique lemapa para usar `nomeArquivo`
    encontramapa(&m, &heroi, HEROI);

    do {
        printf("Pilula: %s\n", (tempilula ? "SIM" : "NAO"));
        printf("Pontuacao: %d\n", pontuacao);
        imprimemapa(&m);

        char comando;
        scanf(" %c", &comando);

        if (ehdirecao(comando)) move(comando);
        if (comando == BOMBA) explodepilula();

        fantasmas();

    } while (!acabou());

    POSICAO pos;
    int venceu = !encontramapa(&m, &pos, PILULA);

    // Atualiza estatísticas
    atualizarEstatisticas(stats, venceu, pontuacao);
    salvarEstatisticas(stats);

    liberamapa(&m);

    if (venceu) {
        printf("\nParabens! Você completou a fase %d!\n", numeroFase);
    }
}

void jogarFases(Estatisticas* stats) {
    int totalFases = 5; // Total de fases
    for (int i = 1; i <= totalFases; i++) {
        printf("\n=== Fase %d ===\n", i);
        inicializajogo(stats, i);

        if (!encontramapa(&m, &heroi, HEROI)) {
            printf("\nFim de jogo! Você perdeu.\n");
            break;
        }

        if (i == totalFases) {
            printf("\nParabéns! Você completou todas as fases!\n");
        }
    }
}

int acabou() {
    POSICAO pos;
    // Verifica se o herói foi capturado
    if (!encontramapa(&m, &pos, HEROI)) {
        printf("\nVocê perdeu! Pac-Man foi capturado.\n");
        return 1;
    }

    // Verifica se ainda há pílulas
    if (!encontramapa(&m, &pos, PILULA)) {
        printf("\nParabéns! Você completou a fase.\n");
        return 1;
    }

    return 0; // O jogo continua
}

void menu() {
    Estatisticas stats;
    carregarEstatisticas(&stats);

    if (strcmp(stats.nome, "Jogador") == 0) {
        solicitarNomeJogador(&stats);
        salvarEstatisticas(&stats); // Salva o nome no arquivo binário
    }
    
    int opcao;
    do {
        printf("===== Pacman =====\n");
        printf("1. Jogar Fases\n");
        printf("2. Estatisticas\n");
        printf("3. Limpar Estatisticas\n");
        printf("4. Sair\n");
        printf("==================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarFases(&stats); break;
            case 2: exibirEstatisticas(&stats); break;
            case 3: limparEstatisticas(&stats); break;
            case 4: printf("Saindo do jogo...\n"); break;
            default: printf("Opção inválida.\n"); break;
        }
    } while (opcao != 4);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    menu();
    return 0;
}

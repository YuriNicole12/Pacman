#include <stdio.h> 
#include <stdlib.h>
#include "mapa.h"
#include <string.h>

#define MAX_LINHAS 20
#define MAX_COLUNAS 30

void lemapa(MAPA* m, const char* nomeArquivo) {
    FILE* f = fopen(nomeArquivo, "r");
    
    if(f == NULL) {
        printf("Erro na leitura do mapa: %s\n", nomeArquivo);
        exit(1);
    }

    fscanf(f, "%d %d", &(m->linhas), &(m->colunas));
    alocamapa(m);
    
    for(int i = 0; i < m->linhas; i++) {
       fscanf(f, "%s", m->matriz[i]);
    }

    fclose(f);
}

void alocamapa(MAPA* m) {
    int i; // Declare fora do laço
   
    m->matriz = malloc(sizeof(char*) * m->linhas);
    
    if (m->matriz == NULL) {
    	
    printf("Erro ao alocar memória!\n");
    exit(1);
}

    for(i = 0; i < m->linhas; i++) {
        m->matriz[i] = malloc(sizeof(char) * m->colunas + 1);
    }
}

void copiamapa(MAPA* destino, MAPA* origem) {
    int i; // Declare fora do laço
    destino->linhas = origem->linhas;
    destino->colunas = origem->colunas;
    alocamapa(destino);
    for(i = 0; i < origem->linhas; i++) {
        strcpy(destino->matriz[i], origem->matriz[i]);
    }
}

void liberamapa(MAPA* m) {
    int i; // Declare fora do laço
    for(i = 0; i < m->linhas; i++) {
        free(m->matriz[i]);
    }

    free(m->matriz);
}

int encontramapa(MAPA* m, POSICAO* p, char c) {
    int i, j; // Declare fora do laço
    for(i = 0; i < m->linhas; i++) {
        for(j = 0; j < m->colunas; j++) {
            if(m->matriz[i][j] == c) {
                p->x = i;
                p->y = j;
                return 1;
            }
        }
    }

    return 0;
}

int podeandar(MAPA* m, char personagem, int x, int y) {
    return 
        ehvalida(m, x, y) && 
        !ehparede(m, x, y) &&
        !ehpersonagem(m, personagem, x, y);
}

int ehvalida(MAPA* m, int x, int y) {
    if(x >= m->linhas) 
        return 0;
    if(y >= m->colunas) 
        return 0;

    return 1;    
}

int ehpersonagem(MAPA* m, char personagem, int x, int y) {
    return
        m->matriz[x][y] == personagem;
}

int ehparede(MAPA* m, int x, int y) {
    return 
        m->matriz[x][y] == PAREDE_VERTICAL ||
        m->matriz[x][y] == PAREDE_HORIZONTAL;
}

char estado_anterior[MAX_LINHAS][MAX_COLUNAS];

void andanomapa(MAPA* m, int xorigem, int yorigem, int xdestino, int ydestino) {
    char personagem = m->matriz[xorigem][yorigem];

    // Salvar o estado original da célula de destino
    if (personagem == FANTASMA) {
        estado_anterior[xdestino][ydestino] = m->matriz[xdestino][ydestino];
    }

    // Mover personagem
    m->matriz[xdestino][ydestino] = personagem;

    // Restaurar célula de origem
    if (estado_anterior[xorigem][yorigem] != 0) {
        m->matriz[xorigem][yorigem] = estado_anterior[xorigem][yorigem];
        estado_anterior[xorigem][yorigem] = 0; // Reseta o estado original
    } else {
        m->matriz[xorigem][yorigem] = VAZIO;
    }
}


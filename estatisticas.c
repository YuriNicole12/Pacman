#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pacman.h"

#define ARQUIVO_STATS "estatisticas.bin"

void carregarEstatisticas(Estatisticas* stats) {
    FILE* arquivo = fopen(ARQUIVO_STATS, "rb");
    if (arquivo == NULL) {
        // Se o arquivo não existir, inicializa com valores padrão
        strcpy(stats->nome, "Jogador");
        stats->partidasJogadas = 0;
        stats->partidasVencidas = 0;
        stats->partidasPerdidas = 0;
        stats->pontuacaoMaxima = 0;
        return;
    }

    fread(stats, sizeof(Estatisticas), 1, arquivo);
    fclose(arquivo);
}

void solicitarNomeJogador(Estatisticas* stats) {
    printf("Digite o nome do jogador: ");
    scanf(" %49[^\n]", stats->nome); // Lê até 49 caracteres ou até o '\n'
}

void salvarEstatisticas(const Estatisticas* stats) {
    FILE* arquivo = fopen(ARQUIVO_STATS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar as estatísticas.\n");
        return;
    }

    fwrite(stats, sizeof(Estatisticas), 1, arquivo);
    fclose(arquivo);
}

void exibirEstatisticas(const Estatisticas* stats) {
    printf("\n=== Estatisticas ===\n");
    printf("Jogador: %s\n", stats->nome);
    printf("Partidas Jogadas: %d\n", stats->partidasJogadas);
    printf("Partidas Vencidas: %d\n", stats->partidasVencidas);
    printf("Partidas Perdidas: %d\n", stats->partidasPerdidas);
    printf("Pontuacao Maxima: %d\n", stats->pontuacaoMaxima);
    printf("====================\n");
}

void atualizarEstatisticas(Estatisticas* stats, int venceu, int pontuacao) {
    stats->partidasJogadas++;
    if (venceu) {
        stats->partidasVencidas++;
        if (pontuacao > stats->pontuacaoMaxima) {
            stats->pontuacaoMaxima = pontuacao;
        }
    } else {
        stats->partidasPerdidas++;
    }
}

void limparEstatisticas(Estatisticas* stats) {
    stats->partidasJogadas = 0;
    stats->partidasVencidas = 0;
    stats->partidasPerdidas = 0;
    stats->pontuacaoMaxima = 0;
    printf("Estatisticas resetadas com sucesso.\n");
}

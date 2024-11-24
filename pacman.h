#ifndef _FOGEFOGE_H_
#define _FOGEFOGE_H_
#ifndef _ESTATISTICAS_H_
#define _ESTATISTICAS_H_

#define CIMA 'w'
#define BAIXO 's'
#define DIREITA 'd'
#define ESQUERDA 'a'
#define BOMBA 'b'

int acabou();
void move(char direcao);
 
int ehdirecao(char direcao);
void fantasmas();
void explodepilula();
void explodepilula2(int x, int y, int somax, int somay, int qtd);

#endif

typedef struct {
	
    char nome[50];
    int partidasJogadas;
    int partidasVencidas;
    int partidasPerdidas;
    int pontuacaoMaxima;
} Estatisticas;

// Funções para gerenciar estatísticas
void carregarEstatisticas(Estatisticas* stats);
void salvarEstatisticas(const Estatisticas* stats);
void exibirEstatisticas(const Estatisticas* stats);
void atualizarEstatisticas(Estatisticas* stats, int venceu, int pontuacao);

#endif

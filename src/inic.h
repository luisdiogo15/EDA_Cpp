#ifndef INIC_H
#define INIC_H

#include "jogo.h"

// leitura
int carregarNomes(string nomes[], int maximo, const string& nomeFicheiro);
int carregarEquipas(string equipas[], int maximo, const string& nomeFicheiro);

// geracao
void calcularDistribuicaoPosicoes(int capacidade, int& gr, int& def, int& med, int& ava);
Jogador gerarJogador(const string nomes[], int totalNomes, int numero, Posicao posicao);
Jogador gerarJogadorTransferencia(const string nomes[], int totalNomes);

// inicializacao
void inicializarClube(Clube& clube, const string& ficheiroNomes);
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas);

#endif
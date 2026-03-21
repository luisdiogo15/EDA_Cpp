#ifndef JOGO_H
#define JOGO_H

#include <string>
using namespace std;

// Posicoes possiveis dos jogadores
enum Posicao { GR, DEF, MED, AVA };

// Ficha de um jogador
struct Jogador {
    int numero;
    string nome;
    Posicao posicao;
    int idade;
    int probLesao;
    int probCastigo;
    int qualidade;
};

// Dados principais do clube
struct Clube {
    string nome;
    Jogador* plantel;      // array dinamico de jogadores
    int totalPlantel;
    int capacidadePlantel;
};

// Dados do campeonato
struct Campeonato {
    string adversarios[17];
    int jornadaAtual;
};

// Inicializacao
void inicializarClube(Clube& clube, const string& ficheiroNomes);
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas);

// Leitura de ficheiros
int carregarNomes(string nomes[], int maximo, const string& nomeFicheiro);
int carregarEquipas(string equipas[], int maximo, const string& nomeFicheiro);

// Funcoes de apoio
string posicaoParaTexto(Posicao p);
int gerarNumero(int minimo, int maximo);

// Geracao dos jogadores
void calcularDistribuicaoPosicoes(int capacidade, int& gr, int& def, int& med, int& ava);
Jogador gerarJogador(const string nomes[], int totalNomes, int numero, Posicao posicao);

// Mostrar dados
void mostrarPlantel(const Clube& clube);

// Libertar memoria
void libertarMemoria(Clube& clube);

#endif
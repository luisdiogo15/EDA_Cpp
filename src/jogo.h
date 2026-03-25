#ifndef JOGO_H
#define JOGO_H

#include <string>
using namespace std;

enum Posicao { GR, DEF, MED, AVA };

struct Jogador {
    int numero;
    string nome;
    Posicao posicao;
    int idade;
    int probLesao;
    int probCastigo;
    int qualidade;
    int diasTreino; // para aparecer na tabela
};

struct Clube {
    string nome;

    Jogador* plantel;
    int totalPlantel;
    int capacidadePlantel;

    Jogador* lesionados;
    int totalLesionados;

    Jogador* castigados;
    int totalCastigados;

    Jogador* transferencias;
    int totalTransferencias;

    int pontos;
};

struct Campeonato {
    string adversarios[17];
    int jornadaAtual;
};

// criam clube e o campeonato
void inicializarClube(Clube& clube, const string& ficheiroNomes);
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas);

// ler os ficheiros dos nomes e das equipas (.txt)
int carregarNomes(string nomes[], int maximo, const string& nomeFicheiro);
int carregarEquipas(string equipas[], int maximo, const string& nomeFicheiro);

// apoio das outras funcoes
string posicaoParaTexto(Posicao p);
int gerarNumero(int minimo, int maximo);

// criam jogadores e as suas posições
void calcularDistribuicaoPosicoes(int capacidade, int& gr, int& def, int& med, int& ava);
Jogador gerarJogador(const string nomes[], int totalNomes, int numero, Posicao posicao);

// mostra a informação no ecrã sobre o plantel e o estado do clube
void mostrarPlantel(const Clube& clube);
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp);

// memoria
void libertarMemoria(Clube& clube);

#endif

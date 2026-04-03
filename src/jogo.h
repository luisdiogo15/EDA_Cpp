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

    int diasTreino;
    int jogosLesao;
    int jogosCastigo;
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

    Jogador* titulares;
    int totalTitulares;

    Jogador* suplentes;
    int totalSuplentes;

    Jogador* naoConvocados;
    int totalNaoConvocados;

    int pontos;

    // tatica base
    int taticaDEF;
    int taticaMED;
    int taticaAVA;
};

struct Campeonato {
    string adversarios[17];
    int jornadaAtual;
};

// inicializacao
void inicializarClube(Clube& clube, const string& ficheiroNomes);
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas);

// leitura
int carregarNomes(string nomes[], int maximo, const string& nomeFicheiro);
int carregarEquipas(string equipas[], int maximo, const string& nomeFicheiro);

// apoio
string posicaoParaTexto(Posicao p);
int gerarNumero(int minimo, int maximo);

// geracao
void calcularDistribuicaoPosicoes(int capacidade, int& gr, int& def, int& med, int& ava);
Jogador gerarJogador(const string nomes[], int totalNomes, int numero, Posicao posicao);

// convocados
void atualizarConvocados(Clube& clube);
void escolherTitulares(Clube& clube, bool usados[]);
void escolherSuplentes(Clube& clube, bool usados[]);

// mostrar
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp);

// memoria
void libertarMemoria(Clube& clube);

#endif
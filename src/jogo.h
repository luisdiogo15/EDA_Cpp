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

    // tatica atual
    int taticaDEF;
    int taticaMED;
    int taticaAVA;

    // resumo da jornada anterior
    bool haResumo;
    string ultimoAdversario;
    int ultGolosEDA;
    int ultGolosADV;
    string resumoSubs[3];
    int totalResumoSubs;
};

struct Campeonato {
    string adversarios[17];
    int jornadaAtual;
};

// operacoes sobre listas dinamicas
void adicionarJogadorOrdenado(Jogador*& lista, int& total, const Jogador& jogador);
Jogador removerJogadorIndice(Jogador*& lista, int& total, int indice);
int procurarJogadorPorNumero(Jogador* lista, int total, int numero);

#endif
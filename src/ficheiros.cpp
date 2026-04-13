#include <fstream>
#include <iostream>
#include "ficheiros.h"
#include "gestao.h"
#include "memoria.h"

using namespace std;

void gravarJogador(ofstream& ficheiro, const Jogador& j) { //ficheiro aberto para escrita e guardamos jogador
    ficheiro << j.numero << endl;
    ficheiro << j.nome << endl;
    ficheiro << j.posicao << endl;
    ficheiro << j.idade << endl;
    ficheiro << j.probLesao << endl;
    ficheiro << j.probCastigo << endl;
    ficheiro << j.qualidade << endl;
    ficheiro << j.diasTreino << endl;
    ficheiro << j.jogosLesao << endl;
    ficheiro << j.jogosCastigo << endl;
}

// le um jogador do ficheiro
void lerJogador(ifstream& ficheiro, Jogador& j) {
    int pos;

    ficheiro >> j.numero;
    ficheiro.ignore();

    getline(ficheiro, j.nome);

    ficheiro >> pos;
    j.posicao = static_cast<Posicao>(pos); // utilizo isto para converter a posicao q foi guardada como int em enum Posicao para ser mais facil ler 

    ficheiro >> j.idade;
    ficheiro >> j.probLesao;
    ficheiro >> j.probCastigo;
    ficheiro >> j.qualidade;
    ficheiro >> j.diasTreino;
    ficheiro >> j.jogosLesao;
    ficheiro >> j.jogosCastigo;
    ficheiro.ignore();
}

// grava o estado atual do clube e campeonato
bool gravarEstado(const Clube& clube, const Campeonato& camp, const string& nomeFicheiro) {
    ofstream ficheiro(nomeFicheiro);

    if (!ficheiro.is_open()) {
        cout << "Erro ao criar ficheiro.\n";
        return false;
    }

    // guardar dados simples
    ficheiro << camp.jornadaAtual << endl;
    ficheiro << clube.pontos << endl;
    ficheiro << clube.taticaDEF << endl;
    ficheiro << clube.taticaMED << endl;
    ficheiro << clube.taticaAVA << endl;

    // adversarios
    for (int i = 0; i < 17; i++) { // percorrer o array das 17 equipas para guardar cada uma no ficheiro
        ficheiro << camp.adversarios[i] << endl;
    }

    // plantel
    ficheiro << clube.totalPlantel << endl; // primeiro guardo o numero de jogadores e dps percorre array para guardar no ficheiro
    for (int i = 0; i < clube.totalPlantel; i++) {
        gravarJogador(ficheiro, clube.plantel[i]);
    }

    // lesionados
    ficheiro << clube.totalLesionados << endl;
    for (int i = 0; i < clube.totalLesionados; i++) {
        gravarJogador(ficheiro, clube.lesionados[i]);
    }

    // castigados
    ficheiro << clube.totalCastigados << endl;
    for (int i = 0; i < clube.totalCastigados; i++) {
        gravarJogador(ficheiro, clube.castigados[i]);
    }

    // transferencias
    ficheiro << clube.totalTransferencias << endl;
    for (int i = 0; i < clube.totalTransferencias; i++) {
        gravarJogador(ficheiro, clube.transferencias[i]);
    }

    ficheiro.close();
    return true;
}

// carrega um estado guardado anteriormente
bool carregarEstado(Clube& clube, Campeonato& camp, const string& nomeFicheiro) {
    ifstream ficheiro(nomeFicheiro);

    if (!ficheiro.is_open()) {
        cout << "Erro ao abrir ficheiro.\n";
        return false;
    }

    // limpa memoria anterior para evitar possiveis fugas
    libertarMemoria(clube);

    // dados simples
    ficheiro >> camp.jornadaAtual;
    ficheiro >> clube.pontos;
    ficheiro >> clube.taticaDEF;
    ficheiro >> clube.taticaMED;
    ficheiro >> clube.taticaAVA;
    ficheiro.ignore();

    // ler os adversarios
    for (int i = 0; i < 17; i++) {
        getline(ficheiro, camp.adversarios[i]);
    }

    clube.nome = "EDA FC";

    // plantel
    ficheiro >> clube.totalPlantel;
    ficheiro.ignore();
    clube.capacidadePlantel = clube.totalPlantel;
    clube.plantel = new Jogador[clube.totalPlantel]; //alocar memoria com o tamanha correto

    for (int i = 0; i < clube.totalPlantel; i++) {
        lerJogador(ficheiro, clube.plantel[i]); // le cada jogador
    }

    // lesionados
    ficheiro >> clube.totalLesionados;
    ficheiro.ignore();
    if (clube.totalLesionados > 0) {
        clube.lesionados = new Jogador[clube.totalLesionados];
        for (int i = 0; i < clube.totalLesionados; i++) {
            lerJogador(ficheiro, clube.lesionados[i]);
        }
    } else {
        clube.lesionados = nullptr; // caso nao haja ponteiro fica nulo
    }

    // castigados
    ficheiro >> clube.totalCastigados;
    ficheiro.ignore();
    if (clube.totalCastigados > 0) {
        clube.castigados = new Jogador[clube.totalCastigados];
        for (int i = 0; i < clube.totalCastigados; i++) {
            lerJogador(ficheiro, clube.castigados[i]);
        }
    } else {
        clube.castigados = nullptr;
    }

    // transferencias
    ficheiro >> clube.totalTransferencias;
    ficheiro.ignore();
    if (clube.totalTransferencias > 0) {
        clube.transferencias = new Jogador[clube.totalTransferencias];
        for (int i = 0; i < clube.totalTransferencias; i++) {
            lerJogador(ficheiro, clube.transferencias[i]);
        }
    } else {
        clube.transferencias = nullptr;
    }

    // estas listas vao ser recalculadas a cada jornada 
    clube.titulares = nullptr; //usamos null pois titulares é ponteiro para array dinamico jogadores
    clube.totalTitulares = 0; // aqui é um int q  diz quantos jogadores existem nessa lista

    clube.suplentes = nullptr;
    clube.totalSuplentes = 0;

    clube.naoConvocados = nullptr;
    clube.totalNaoConvocados = 0;

    // limpa resumo antigo
    clube.haResumo = false;
    clube.ultimoAdversario = "";
    clube.ultGolosEDA = 0;
    clube.ultGolosADV = 0;
    clube.totalResumoSubs = 0;

    ficheiro.close();

    // depois de carregar vikta a gerar titulares e suplentes
    atualizarConvocados(clube);

    return true;
}
#include <fstream>
#include <iostream>
#include "ficheiros.h"
#include "gestao.h"
#include "memoria.h"

using namespace std;

// guarda um jogador numa linha do ficheiro
void gravarJogador(ofstream& ficheiro, const Jogador& j) {
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
    j.posicao = static_cast<Posicao>(pos);

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

    // dados simples
    ficheiro << camp.jornadaAtual << endl;
    ficheiro << clube.pontos << endl;
    ficheiro << clube.taticaDEF << endl;
    ficheiro << clube.taticaMED << endl;
    ficheiro << clube.taticaAVA << endl;

    // adversarios
    for (int i = 0; i < 17; i++) {
        ficheiro << camp.adversarios[i] << endl;
    }

    // plantel
    ficheiro << clube.totalPlantel << endl;
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

    // limpa memoria anterior
    libertarMemoria(clube);

    // dados simples
    ficheiro >> camp.jornadaAtual;
    ficheiro >> clube.pontos;
    ficheiro >> clube.taticaDEF;
    ficheiro >> clube.taticaMED;
    ficheiro >> clube.taticaAVA;
    ficheiro.ignore();

    // adversarios
    for (int i = 0; i < 17; i++) {
        getline(ficheiro, camp.adversarios[i]);
    }

    clube.nome = "EDA FC";

    // plantel
    ficheiro >> clube.totalPlantel;
    ficheiro.ignore();
    clube.capacidadePlantel = clube.totalPlantel;
    clube.plantel = new Jogador[clube.totalPlantel];

    for (int i = 0; i < clube.totalPlantel; i++) {
        lerJogador(ficheiro, clube.plantel[i]);
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
        clube.lesionados = nullptr;
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

    // estas listas vao ser recalculadas
    clube.titulares = nullptr;
    clube.totalTitulares = 0;

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

    // volta a calcular convocados
    atualizarConvocados(clube);

    return true;
}
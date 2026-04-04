#include <iostream>
#include <fstream>
#include "inic.h"
#include "util.h"

using namespace std;

// le nomes do ficheiro
int carregarNomes(string nomes[], int maximo, const string& nomeFicheiro) {
    ifstream ficheiro(nomeFicheiro);
    string linha;
    int total = 0;

    if (!ficheiro.is_open()) {
        cout << "Erro ao abrir " << nomeFicheiro << endl;
        return 0;
    }

    while (getline(ficheiro, linha) && total < maximo) {
        if (linha != "") {
            nomes[total] = linha;
            total++;
        }
    }

    ficheiro.close();
    return total;
}

// le equipas do ficheiro
int carregarEquipas(string equipas[], int maximo, const string& nomeFicheiro) {
    ifstream ficheiro(nomeFicheiro);
    string linha;
    int total = 0;

    if (!ficheiro.is_open()) {
        cout << "Erro ao abrir " << nomeFicheiro << endl;
        return 0;
    }

    while (getline(ficheiro, linha) && total < maximo) {
        if (linha != "") {
            equipas[total] = linha;
            total++;
        }
    }

    ficheiro.close();
    return total;
}

// calcula quantos jogadores ha por posicao
void calcularDistribuicaoPosicoes(int capacidade, int& gr, int& def, int& med, int& ava) {
    gr = 2;
    def = 7;
    med = 7;
    ava = 4;

    int restantes = capacidade - 20;

    while (restantes > 0) {
        int opcao = gerarNumero(1, 4);

        if (opcao == 1 && gr < 3) {
            gr++;
            restantes--;
        }
        else if (opcao == 2 && def < 10) {
            def++;
            restantes--;
        }
        else if (opcao == 3 && med < 10) {
            med++;
            restantes--;
        }
        else if (opcao == 4 && ava < 7) {
            ava++;
            restantes--;
        }
    }
}

// gera um jogador aleatorio do EDA FC
Jogador gerarJogador(const string nomes[], int totalNomes, int numero, Posicao posicao) {
    Jogador j;

    j.numero = numero;
    j.nome = nomes[gerarNumero(0, totalNomes - 1)];
    j.posicao = posicao;
    j.idade = gerarNumero(18, 35);
    j.probLesao = gerarNumero(0, 15);
    j.probCastigo = gerarNumero(0, 20);
    j.qualidade = gerarNumero(0, 100);

    j.diasTreino = 0;
    j.jogosLesao = 0;
    j.jogosCastigo = 0;

    return j;
}

// gera um jogador novo para a lista de transferencias
Jogador gerarJogadorTransferencia(const string nomes[], int totalNomes) {
    Jogador j;

    j.numero = gerarNumero(1, 99);
    j.nome = nomes[gerarNumero(0, totalNomes - 1)];
    j.posicao = static_cast<Posicao>(gerarNumero(0, 3));
    j.idade = gerarNumero(18, 35);
    j.probLesao = gerarNumero(0, 15);
    j.probCastigo = gerarNumero(0, 20);
    j.qualidade = gerarNumero(0, 100);

    j.diasTreino = 0;
    j.jogosLesao = 0;
    j.jogosCastigo = 0;

    return j;
}

// inicializa o clube
void inicializarClube(Clube& clube, const string& ficheiroNomes) {
    string nomes[1000];
    int totalNomes = carregarNomes(nomes, 1000, ficheiroNomes);

    clube.nome = "EDA FC";
    clube.capacidadePlantel = gerarNumero(20, 30);
    clube.totalPlantel = clube.capacidadePlantel;
    clube.plantel = new Jogador[clube.capacidadePlantel];

    clube.lesionados = nullptr;
    clube.totalLesionados = 0;

    clube.castigados = nullptr;
    clube.totalCastigados = 0;

    clube.transferencias = nullptr;
    clube.totalTransferencias = 0;

    clube.titulares = nullptr;
    clube.totalTitulares = 0;

    clube.suplentes = nullptr;
    clube.totalSuplentes = 0;

    clube.naoConvocados = nullptr;
    clube.totalNaoConvocados = 0;

    clube.pontos = 0;

    // tatica base 1-4-4-2
    clube.taticaDEF = 4;
    clube.taticaMED = 4;
    clube.taticaAVA = 2;

    // ainda nao existe resumo antes da primeira jornada
    clube.haResumo = false;
    clube.ultimoAdversario = "";
    clube.ultGolosEDA = 0;
    clube.ultGolosADV = 0;
    clube.totalResumoSubs = 0;

    int gr, def, med, ava;
    calcularDistribuicaoPosicoes(clube.capacidadePlantel, gr, def, med, ava);

    int indice = 0;
    int numero = 1;

    // plantel fica organizado por posicao
    for (int i = 0; i < gr; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, GR);
        indice++;
        numero++;
    }

    for (int i = 0; i < def; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, DEF);
        indice++;
        numero++;
    }

    for (int i = 0; i < med; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, MED);
        indice++;
        numero++;
    }

    for (int i = 0; i < ava; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, AVA);
        indice++;
        numero++;
    }
}

// inicializa campeonato
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas) {
    string equipas[1000];
    int totalEquipas = carregarEquipas(equipas, 1000, ficheiroEquipas);

    camp.jornadaAtual = 1;

    bool usadas[1000] = {false};

    for (int i = 0; i < 17; i++) {
        int pos;

        do {
            pos = gerarNumero(0, totalEquipas - 1);
        } while (usadas[pos]);

        usadas[pos] = true;
        camp.adversarios[i] = equipas[pos];
    }
}
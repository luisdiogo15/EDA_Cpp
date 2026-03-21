#include <iostream>
#include <fstream>
#include "jogo.h"

using namespace std;

// Gera um numero aleatorio entre minimo e maximo
int gerarNumero(int minimo, int maximo) {
    return rand() % (maximo - minimo + 1) + minimo;
}

// Converte a posicao para texto
string posicaoParaTexto(Posicao p) {
    if (p == GR) return "GR";
    if (p == DEF) return "DEF";
    if (p == MED) return "MED";
    return "AVA";
}

// Le os nomes do ficheiro e guarda no array
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

// Le as equipas do ficheiro e guarda no array
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

// Decide quantos jogadores vais ter em cada posicao
void calcularDistribuicaoPosicoes(int capacidade, int& gr, int& def, int& med, int& ava) {
    // Minimos obrigatorios
    gr = 2;
    def = 7;
    med = 7;
    ava = 4;

    // Jogadores que faltam distribuir
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

// Cria um jogador aleatorio
Jogador gerarJogador(const string nomes[], int totalNomes, int numero, Posicao posicao) {
    Jogador j;

    j.numero = numero;
    j.nome = nomes[gerarNumero(0, totalNomes - 1)];
    j.posicao = posicao;
    j.idade = gerarNumero(18, 35);
    j.probLesao = gerarNumero(0, 15);
    j.probCastigo = gerarNumero(0, 20);
    j.qualidade = gerarNumero(0, 100);

    return j;
}

// Cria o clube e gera o plantel
void inicializarClube(Clube& clube, const string& ficheiroNomes) {
    string nomes[1000];
    int totalNomes = carregarNomes(nomes, 1000, ficheiroNomes);

    clube.nome = "EDA FC";
    clube.capacidadePlantel = gerarNumero(20, 30); // entre 20 e 30
    clube.totalPlantel = clube.capacidadePlantel;
    clube.plantel = new Jogador[clube.capacidadePlantel]; // cria o array dinamico

    int gr, def, med, ava;
    calcularDistribuicaoPosicoes(clube.capacidadePlantel, gr, def, med, ava);

    int indice = 0;
    int numero = 1;

    // Primeiro guarda-redes
    for (int i = 0; i < gr; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, GR);
        indice++;
        numero++;
    }

    // Depois defesas
    for (int i = 0; i < def; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, DEF);
        indice++;
        numero++;
    }

    // Depois medios
    for (int i = 0; i < med; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, MED);
        indice++;
        numero++;
    }

    // Por fim avancados
    for (int i = 0; i < ava; i++) {
        clube.plantel[indice] = gerarJogador(nomes, totalNomes, numero, AVA);
        indice++;
        numero++;
    }
}

// Cria a lista de adversarios do campeonato
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas) {
    string equipas[1000];
    int totalEquipas = carregarEquipas(equipas, 1000, ficheiroEquipas);

    camp.jornadaAtual = 1;

    bool usadas[1000] = {false};

    for (int i = 0; i < 17; i++) {
        int pos;

        // Vai procurar uma equipa ainda nao usada
        do {
            pos = gerarNumero(0, totalEquipas - 1);
        } while (usadas[pos]);

        usadas[pos] = true;
        camp.adversarios[i] = equipas[pos];
    }
}

// Mostra o plantel no ecra
void mostrarPlantel(const Clube& clube) {
    cout << "\nPlantel do " << clube.nome << ":\n";

    for (int i = 0; i < clube.totalPlantel; i++) {
        cout << clube.plantel[i].numero << " - "
             << clube.plantel[i].nome << " - "
             << posicaoParaTexto(clube.plantel[i].posicao) << " - "
             << clube.plantel[i].idade << " anos - Q "
             << clube.plantel[i].qualidade << endl;
    }
}

// Liberta a memoria do plantel
void libertarMemoria(Clube& clube) {
    delete[] clube.plantel;
    clube.plantel = nullptr;
}
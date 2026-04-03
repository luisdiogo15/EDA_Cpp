#include <iostream>
#include <fstream>
#include <iomanip>
#include "jogo.h"

using namespace std;

// gera numero aleatorio entre minimo e maximo
int gerarNumero(int minimo, int maximo) {
    return rand() % (maximo - minimo + 1) + minimo;
}

// converte posicao para texto
string posicaoParaTexto(Posicao p) {
    if (p == GR) return "GR";
    if (p == DEF) return "DEF";
    if (p == MED) return "MED";
    return "AVA";
}

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

// gera um jogador aleatorio
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

// devolve o indice do melhor jogador de uma posicao que ainda nao foi usado
int melhorJogadorPorPosicao(const Clube& clube, Posicao pos, bool usados[]) {
    int melhorIndice = -1;

    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!usados[i] && clube.plantel[i].posicao == pos) {
            if (melhorIndice == -1 || clube.plantel[i].qualidade > clube.plantel[melhorIndice].qualidade) {
                melhorIndice = i;
            }
        }
    }

    return melhorIndice;
}

// devolve o indice do melhor jogador restante, qualquer posicao
int melhorJogadorRestante(const Clube& clube, bool usados[]) {
    int melhorIndice = -1;

    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!usados[i]) {
            if (melhorIndice == -1 || clube.plantel[i].qualidade > clube.plantel[melhorIndice].qualidade) {
                melhorIndice = i;
            }
        }
    }

    return melhorIndice;
}

// escolhe titulares automaticamente
void escolherTitulares(Clube& clube, bool usados[]) {
    clube.totalTitulares = 0;
    clube.titulares = new Jogador[11];

    int indice;

    // 1 GR
    indice = melhorJogadorPorPosicao(clube, GR, usados);
    if (indice != -1) {
        clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
        usados[indice] = true;
    }

    // DEF
    for (int i = 0; i < clube.taticaDEF; i++) {
        indice = melhorJogadorPorPosicao(clube, DEF, usados);
        if (indice != -1) {
            clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // MED
    for (int i = 0; i < clube.taticaMED; i++) {
        indice = melhorJogadorPorPosicao(clube, MED, usados);
        if (indice != -1) {
            clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // AVA
    for (int i = 0; i < clube.taticaAVA; i++) {
        indice = melhorJogadorPorPosicao(clube, AVA, usados);
        if (indice != -1) {
            clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }
}

// escolhe suplentes automaticamente
void escolherSuplentes(Clube& clube, bool usados[]) {
    clube.totalSuplentes = 0;
    clube.suplentes = new Jogador[6];

    int indice;

    // 1 GR
    indice = melhorJogadorPorPosicao(clube, GR, usados);
    if (indice != -1) {
        clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
        usados[indice] = true;
    }

    // 2 DEF
    for (int i = 0; i < 2; i++) {
        indice = melhorJogadorPorPosicao(clube, DEF, usados);
        if (indice != -1) {
            clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // 2 MED
    for (int i = 0; i < 2; i++) {
        indice = melhorJogadorPorPosicao(clube, MED, usados);
        if (indice != -1) {
            clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // 1 AVA
    indice = melhorJogadorPorPosicao(clube, AVA, usados);
    if (indice != -1) {
        clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
        usados[indice] = true;
    }

    // se faltarem suplentes, preenche com os melhores restantes
    while (clube.totalSuplentes < 6) {
        indice = melhorJogadorRestante(clube, usados);
        if (indice == -1) {
            break;
        }

        clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
        usados[indice] = true;
    }
}

// atualiza titulares, suplentes e nao convocados
void atualizarConvocados(Clube& clube) {
    delete[] clube.titulares;
    delete[] clube.suplentes;
    delete[] clube.naoConvocados;

    clube.titulares = nullptr;
    clube.suplentes = nullptr;
    clube.naoConvocados = nullptr;

    clube.totalTitulares = 0;
    clube.totalSuplentes = 0;
    clube.totalNaoConvocados = 0;

    bool* usados = new bool[clube.totalPlantel];
    for (int i = 0; i < clube.totalPlantel; i++) {
        usados[i] = false;
    }

    escolherTitulares(clube, usados);
    escolherSuplentes(clube, usados);

    // conta nao convocados
    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!usados[i]) {
            clube.totalNaoConvocados++;
        }
    }

    if (clube.totalNaoConvocados > 0) {
        clube.naoConvocados = new Jogador[clube.totalNaoConvocados];

        int j = 0;
        for (int i = 0; i < clube.totalPlantel; i++) {
            if (!usados[i]) {
                clube.naoConvocados[j] = clube.plantel[i];
                j++;
            }
        }
    }

    delete[] usados;
}

// mostra lista simples de jogadores
void mostrarListaSimples(Jogador* lista, int total) {
    if (total == 0) {
        cout << "(vazio)" << endl;
        return;
    }

    for (int i = 0; i < total; i++) {
        cout << lista[i].numero << " - "
             << lista[i].nome << " - "
             << posicaoParaTexto(lista[i].posicao) << " - Q "
             << lista[i].qualidade << endl;
    }
}

// mostra estado completo
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp) {
    const int wNome = 24;
    const int wNum = 4;
    const int wPos = 7;
    const int wIdade = 5;
    const int wLesao = 10;
    const int wCastigo = 12;
    const int wQualidade = 10;
    const int wTreino = 11;

    cout << "****************************************\n";
    cout << "* " << clube.nome << " - " << camp.jornadaAtual
         << "a Jornada - " << clube.pontos << " pontos. *\n";
    cout << "****************************************\n\n";

    cout << "*********************** Plantel Disponivel: ***********************\n";

    cout << left
         << setw(wNome)      << "Nome"
         << " | " << setw(wNum)       << "Nr"
         << " | " << setw(wPos)       << "Posicao"
         << " | " << setw(wIdade)     << "Idade"
         << " | " << setw(wLesao)     << "ProbLesao"
         << " | " << setw(wCastigo)   << "ProbCastigo"
         << " | " << setw(wQualidade) << "Qualidade"
         << " | " << setw(wTreino)    << "Dias-Treino"
         << endl;

    cout << string(
        wNome + wNum + wPos + wIdade + wLesao + wCastigo + wQualidade + wTreino + 21, '-'
    ) << endl;

<<<<<<< HEAD
    // verifica a posicao e dps mete linha branco para separar os grupos
    Posicao ultimaPosicao = GR; // comeca na posicao de gr 
    bool primeiraLinha = true;verifica se ainda esta na linha do gr
=======
    Posicao ultimaPosicao = GR;
    bool primeiraLinha = true;
>>>>>>> 0df025b08a06ef186506e59471e64e3f9c7e0132

    for (int i = 0; i < clube.totalPlantel; i++) { // imprimir jogador a jogador
        if (!primeiraLinha && clube.plantel[i].posicao != ultimaPosicao) {
            cout << endl;
        }

        cout << left
             << setw(wNome)      << clube.plantel[i].nome
             << " | " << setw(wNum)       << clube.plantel[i].numero
             << " | " << setw(wPos)       << posicaoParaTexto(clube.plantel[i].posicao)
             << " | " << setw(wIdade)     << clube.plantel[i].idade
             << " | " << setw(wLesao)     << (to_string(clube.plantel[i].probLesao) + "%")
             << " | " << setw(wCastigo)   << (to_string(clube.plantel[i].probCastigo) + "%")
             << " | " << setw(wQualidade) << clube.plantel[i].qualidade
             << " | " << setw(wTreino)    << clube.plantel[i].diasTreino
             << endl;

        ultimaPosicao = clube.plantel[i].posicao;
        primeiraLinha = false;
    }

    cout << string(
        wNome + wNum + wPos + wIdade + wLesao + wCastigo + wQualidade + wTreino + 21, '-'
    ) << endl;

    cout << "\nTitulares:\n";
    mostrarListaSimples(clube.titulares, clube.totalTitulares);

    cout << "\nSuplentes:\n";
    mostrarListaSimples(clube.suplentes, clube.totalSuplentes);

    cout << "\nNao convocados:\n";
    mostrarListaSimples(clube.naoConvocados, clube.totalNaoConvocados);

    cout << "\nLesionados:\n";
    mostrarListaSimples(clube.lesionados, clube.totalLesionados);

    cout << "\nCastigados:\n";
    mostrarListaSimples(clube.castigados, clube.totalCastigados);

    cout << "\n*****************************************************************************\n";
    cout << "Transferencias:\n";
    mostrarListaSimples(clube.transferencias, clube.totalTransferencias);

    cout << "\n(s)seguinte (o)coes\n";
}

// liberta memoria
void libertarMemoria(Clube& clube) {
    delete[] clube.plantel;
    delete[] clube.lesionados;
    delete[] clube.castigados;
    delete[] clube.transferencias;
    delete[] clube.titulares;
    delete[] clube.suplentes;
    delete[] clube.naoConvocados;

    clube.plantel = nullptr;
    clube.lesionados = nullptr;
    clube.castigados = nullptr;
    clube.transferencias = nullptr;
    clube.titulares = nullptr;
    clube.suplentes = nullptr;
    clube.naoConvocados = nullptr;
}
#include <iostream>
#include <fstream>
#include <iomanip>
#include "jogo.h"

using namespace std;

// Gera um numero aleatorio entre minimo e maximo que depois vai ser usado para a idade do jogador , qualidade , problesao e etc intervalo definido na linha 105 
int gerarNumero(int minimo, int maximo) {
    return rand() % (maximo - minimo + 1) + minimo;
}

// recebido como enum e devolve como string 
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
    int total = 0; // por enquanto 0 nomes no array

    if (!ficheiro.is_open()) {
        cout << "Erro ao abrir " << nomeFicheiro << endl;
        return 0;
    }

    while (getline(ficheiro, linha) && total < maximo) { // lê linha a linha
        if (linha != "") { // serve apenas para ignorar as linhas vazias
            nomes[total] = linha;
            total++; // guarda nome no array
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
    j.idade = gerarNumero(18, 35); // intervalo para a idade 18 a 35
    j.probLesao = gerarNumero(5, 12); // intervalo para a lesao
    j.probCastigo = gerarNumero(4, 14); // ... para castigo
    j.qualidade = gerarNumero(78, 91); // .. para qualidade
    j.diasTreino = 0;

    return j;
}

// Cria o clube e gera o plantel
void inicializarClube(Clube& clube, const string& ficheiroNomes) {
    string nomes[1000];
    int totalNomes = carregarNomes(nomes, 1000, ficheiroNomes); // vai ao array dos nomes lê e devolve quantos tem

    clube.nome = "EDA FC";
    clube.capacidadePlantel = gerarNumero(20, 30);
    clube.totalPlantel = clube.capacidadePlantel; // Diz quantos jogadores temos neste momento
    clube.plantel = new Jogador[clube.capacidadePlantel];\\  // reservar espaço de memoria para os jogadores

    // listas vazias para ja
    clube.lesionados = nullptr; // memória
    clube.totalLesionados = 0; //contador

    clube.castigados = nullptr;
    clube.totalCastigados = 0;

    clube.transferencias = nullptr;
    clube.totalTransferencias = 0;

    clube.pontos = 0;

    int gr, def, med, ava;
    calcularDistribuicaoPosicoes(clube.capacidadePlantel, gr, def, med, ava);

    int indice = 0; // posicao 0 do array de jogadores 
    int numero = 1; // atribuir o numero 1 camisola ao tal jogador que esta na posicao 0 ao array

    
    // aqui estamos a criar jogadores para cada posicao

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

// Cria a lista de adversarios do campeonato
void inicializarCampeonato(Campeonato& camp, const string& ficheiroEquipas) {
    string equipas[1000];
    int totalEquipas = carregarEquipas(equipas, 1000, ficheiroEquipas);

    camp.jornadaAtual = 1;

    bool usadas[1000] = {false};  //evita repetir equipas pois é suposto jogar todos contra todos sem repetir se for false é pq ainda n jogou contra essa equipa

    for (int i = 0; i < 17; i++) {
        int pos;

        // Vai procurar uma equipa ainda nao usada
        do {
            pos = gerarNumero(0, totalEquipas - 1);
        } while (usadas[pos]); // faz com que seja garantido  que não se repita as equipas

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

//Estrutura para output geral 

void mostrarEstadoClube(const Clube& clube, const Campeonato& camp) {
    cout << "****************************************\n";
    cout << "* " << clube.nome << " - " << camp.jornadaAtual
         << "ª Jornada - " << clube.pontos << " pontos. *\n";
    cout << "****************************************\n\n";

    cout << "*********************** Plantel Disponivel: ***********************\n";

    cout << left
         << setw(24) << "Nome"
         << setw(6)  << "| Nº"
         << setw(10) << "| Posição"
         << setw(8)  << "| Idade"
         << setw(13) << "| ProbLesão"
         << setw(15) << "| ProbCastigo"
         << setw(12) << "| Qualidade"
         << setw(12) << "| Dias-Treino" << endl;

    cout << "--------------------------------------------------------------------------------------------------------------------------------\n";

    // verifica a posicao e dps mete linha branco para separar os grupos
    Posicao ultimaPosicao = GR;
    bool primeiraLinha = true;

    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!primeiraLinha && clube.plantel[i].posicao != ultimaPosicao) {
            cout << endl; // linha em branco entre grupos de posições
        }

        cout << left
             << setw(24) << clube.plantel[i].nome
             << setw(6)  << ("| " + to_string(clube.plantel[i].numero))
             << setw(10) << ("| " + posicaoParaTexto(clube.plantel[i].posicao))
             << setw(8)  << ("| " + to_string(clube.plantel[i].idade))
             << setw(13) << ("| " + to_string(clube.plantel[i].probLesao) + "%")
             << setw(15) << ("| " + to_string(clube.plantel[i].probCastigo) + "%")
             << setw(12) << ("| " + to_string(clube.plantel[i].qualidade))
             << setw(12) << ("| " + to_string(clube.plantel[i].diasTreino))
             << endl;

        ultimaPosicao = clube.plantel[i].posicao;
        primeiraLinha = false;
    }

    cout << "--------------------------------------------------------------------------------------------------------------------------------\n";

    cout << "\nLesionados:\n";
    if (clube.totalLesionados == 0) { // verificar se ha lesionados caso nao linha vazia se nao imprime o nome dos mesmos
        cout << endl;
    } else {
        for (int i = 0; i < clube.totalLesionados; i++) {
            cout << "- " << clube.lesionados[i].nome << endl; 
        }
    }

    cout << "\nCastigados:\n";
    if (clube.totalCastigados == 0) {
        cout << endl;
    } else {
        for (int i = 0; i < clube.totalCastigados; i++) {
            cout << "- " << clube.castigados[i].nome << endl;
        }
    }

    cout << "\n*****************************************************************************\n";
    cout << "Transferências:\n";
    if (clube.totalTransferencias == 0) {
        cout << endl;
    } else {
        for (int i = 0; i < clube.totalTransferencias; i++) {
            cout << "- " << clube.transferencias[i].nome << " | "
                 << posicaoParaTexto(clube.transferencias[i].posicao)
                 << " | Q " << clube.transferencias[i].qualidade << endl;
        }
    }

    cout << "\n(s)seguinte  (o)ções\n";
}

// limpa a memoria dos arrays dinamicos
void libertarMemoria(Clube& clube) {
    delete[] clube.plantel;
    delete[] clube.lesionados;
    delete[] clube.castigados;
    delete[] clube.transferencias;

    clube.plantel = nullptr;
    clube.lesionados = nullptr;
    clube.castigados = nullptr;
    clube.transferencias = nullptr;
}
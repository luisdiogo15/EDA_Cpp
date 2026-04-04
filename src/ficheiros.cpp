#include <iostream>
#include <fstream>
#include "jogo.h"

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
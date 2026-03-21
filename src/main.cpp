#include <iostream>
#include <ctime>
#include <cstdlib>
#include "jogo.h"

using namespace std;

int main() {
    srand(time(NULL)); // inicia os aleatorios

    Clube eda;         // clube principal
    Campeonato camp;   // campeonato

    inicializarClube(eda, "nomes.txt");         // cria o plantel
    inicializarCampeonato(camp, "equipas.txt"); // cria os adversarios

    cout << "Projeto EDA FC iniciado com sucesso!" << endl;

    mostrarPlantel(eda); // mostra o plantel

    cout << "\nAdversarios do campeonato:\n";
    for (int i = 0; i < 17; i++) {
        cout << i + 1 << " - " << camp.adversarios[i] << endl;
    }

    libertarMemoria(eda); // apaga memoria do plantel
    return 0;
}


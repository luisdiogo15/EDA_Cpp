#include <iostream>
#include <ctime>
#include <cstdlib>
#include "jogo.h"

using namespace std;

int main() {
    srand(time(NULL));

    Clube eda;
    Campeonato camp;

    inicializarClube(eda, "nomes.txt");
    inicializarCampeonato(camp, "equipas.txt");

    mostrarEstadoClube(eda, camp);§

    libertarMemoria(eda);
    return 0;
}


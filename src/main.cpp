#include <iostream>
#include <ctime>
#include <cstdlib>
#include "jogo.h"
#include "inic.h"
#include "gestao.h"
#include "jornada.h"
#include "memoria.h"
#include "ficheiros.h"

using namespace std;

int main() {
    srand(time(NULL));

    Clube eda;
    Campeonato camp;

    inicializarClube(eda, "nomes.txt");
    inicializarCampeonato(camp, "equipas.txt");

    // escolhe automaticamente titulares, suplentes e nao convocados
    atualizarConvocados(eda);

    char opcao;

    do {
        mostrarEstadoClube(eda, camp);
        cin >> opcao;

        if (opcao == 's') {
            simularProximaJornada(eda, camp, "nomes.txt");
        }
        else if (opcao == 'o') {
            menuGestao(eda);
        }
        else if (opcao == 'g') {
            if (gravarEstado(eda, camp, "save.txt")) {
                cout << "Estado gravado com sucesso.\n";
            }
        }
        else if (opcao == 'c') {
            if (carregarEstado(eda, camp, "save.txt")) {
                cout << "Estado carregado com sucesso.\n";
            }
        }

    } while (opcao != 'q');

    libertarMemoria(eda);
    return 0;
}
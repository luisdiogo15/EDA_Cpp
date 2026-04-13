#include "memoria.h"

void libertarMemoria(Clube& clube) { // limpar arrays criados com o new
    delete[] clube.plantel;
    delete[] clube.lesionados;
    delete[] clube.castigados;
    delete[] clube.transferencias;
    delete[] clube.titulares;
    delete[] clube.suplentes;
    delete[] clube.naoConvocados;

    clube.plantel = nullptr; // dps de apagar memoria limpa ponteiro para nao apontar para o nada
    clube.lesionados = nullptr;
    clube.castigados = nullptr;
    clube.transferencias = nullptr;
    clube.titulares = nullptr;
    clube.suplentes = nullptr;
    clube.naoConvocados = nullptr;
}
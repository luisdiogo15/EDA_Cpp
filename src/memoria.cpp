#include "jogo.h"

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
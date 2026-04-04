#include <cstdlib>
#include "util.h"

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
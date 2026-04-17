#include <cstdlib>
#include "util.h"

// gera um numero aleatorio entre o minimo e o maximo (inclusive)
int gerarNumero(int minimo, int maximo) {
    // rand() gera um numero aleatorio
    // o operador % limita esse numero ao intervalo desejado
    // depois somamos o minimo para deslocar o intervalo
    return rand() % (maximo - minimo + 1) + minimo;
}

// converte a posicao guardada em enum para texto,
// para ser apresentada ao utilizador no ecrã
string posicaoParaTexto(Posicao p) {
    if (p == GR) return "GR";
    if (p == DEF) return "DEF";
    if (p == MED) return "MED";

    // se nao for nenhuma das anteriores, entao e avancado
    return "AVA";
}
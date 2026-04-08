#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "jogo.h"

using namespace std;

int gerarNumero(int minimo, int maximo); // gera um número aleatório entre o minimo e o maximo para ser usado para probLesao,qualidade etc
string posicaoParaTexto(Posicao p); //recebe um valor do tipo Posicao (um numero) e devolve o nome por extenso como uma string para GR,DEF...

#endif
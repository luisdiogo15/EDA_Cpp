#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "jogo.h"

bool gravarEstado(const Clube& clube, const Campeonato& camp, const string& nomeFicheiro);
bool carregarEstado(Clube& clube, Campeonato& camp, const string& nomeFicheiro);

#endif
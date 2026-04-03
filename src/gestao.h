#ifndef GESTAO_H
#define GESTAO_H

#include "jogo.h"

// convocados
void atualizarConvocados(Clube& clube);
void escolherTitulares(Clube& clube, bool usados[]);
void escolherSuplentes(Clube& clube, bool usados[]);

// mostrar
void mostrarListaSimples(Jogador* lista, int total);
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp);

// memoria
void libertarMemoria(Clube& clube);

#endif
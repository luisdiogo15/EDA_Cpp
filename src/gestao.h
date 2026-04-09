#ifndef GESTAO_H
#define GESTAO_H

#include "jogo.h"

// convocados
void atualizarConvocados(Clube& clube);
void escolherTitulares(Clube& clube, bool usados[]);
void escolherSuplentes(Clube& clube, bool usados[]);

// mostrar
void mostrarListaSimples(Jogador* lista, int total);
void mostrarTabelaJogadores(Jogador* lista, int total, bool mostrarJogosLesao, bool mostrarJogosCastigo, bool mostrarDiasTreino);
void mostrarResumoJornadaAnterior(const Clube& clube);
void mostrarEstadoAntesProximaJornada(const Clube& clube);
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp);

// alteracoes manuais
int procurarJogadorLista(Jogador* lista, int total, int numero);
void trocarJogadoresListas(Jogador& a, Jogador& b);
void mostrarConvocadosManuais(const Clube& clube);
void menuAlteracoesManuais(Clube& clube);

// treino especifico
void menuTreinoEspecifico(Clube& clube);
void mudarPosicaoJogador(Clube& clube);
void melhorarQualidadeJogador(Clube& clube);

// tatica
void menuAlterarTatica(Clube& clube);

// gestao fisica e disciplinar
void menuGestaoFisicaDisciplinar(Clube& clube);
void aplicarLesao(Clube& clube);
void reduzirLesao(Clube& clube);
void aplicarCastigo(Clube& clube);
void reduzirCastigo(Clube& clube);

// menu de gestao
void menuGestao(Clube& clube);

#endif
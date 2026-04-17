#ifndef GESTAO_H
#define GESTAO_H

#include "jogo.h"

// convocados
void atualizarConvocados(Clube& clube); // recalcula titulares, suplentes e nao convocados
void escolherTitulares(Clube& clube, bool usados[]); // escolhe automaticamente o 11 inicial
void escolherSuplentes(Clube& clube, bool usados[]); // escolhe automaticamente os suplentes

// mostrar
void mostrarListaSimples(Jogador* lista, int total); // mostra uma lista em formato simples
void mostrarTabelaJogadores(Jogador* lista, int total, bool mostrarJogosLesao, bool mostrarJogosCastigo, bool mostrarDiasTreino); // mostra uma lista em tabela
void mostrarResumoJornadaAnterior(const Clube& clube); // mostra o resumo do ultimo jogo
void mostrarEstadoAntesProximaJornada(const Clube& clube); // mostra o estado atual para a jornada seguinte
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp); // mostra o ecra principal do clube

// alteracoes manuais
int procurarJogadorLista(Jogador* lista, int total, int numero); // procura um jogador pelo numero numa lista especifica
void trocarJogadoresListas(Jogador& a, Jogador& b); // troca dois jogadores entre listas
void mostrarConvocadosManuais(const Clube& clube); // mostra titulares, suplentes e nao convocados para alteracoes manuais
void menuAlteracoesManuais(Clube& clube); // menu de trocas manuais antes da jornada

// treino especifico
void menuTreinoEspecifico(Clube& clube); // menu do treino especifico
void mudarPosicaoJogador(Clube& clube); // altera a posicao de um jogador
void melhorarQualidadeJogador(Clube& clube); // aumenta a qualidade de um jogador

// tatica
void menuAlterarTatica(Clube& clube); // altera a tatica atual do clube

// gestao fisica e disciplinar
void menuGestaoFisicaDisciplinar(Clube& clube); // menu de lesoes e castigos
void aplicarLesao(Clube& clube); // move um jogador para a lista de lesionados
void reduzirLesao(Clube& clube); // reduz os jogos de lesao de um jogador
void aplicarCastigo(Clube& clube); // move um jogador para a lista de castigados
void reduzirCastigo(Clube& clube); // reduz os jogos de castigo de um jogador

// menu de gestao
void menuGestao(Clube& clube); // menu geral de gestao da equipa

#endif
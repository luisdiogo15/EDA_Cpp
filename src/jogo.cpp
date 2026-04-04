#include "jogo.h"

// adiciona um jogador a uma lista dinamica, mantendo a ordem por posicao
void adicionarJogadorOrdenado(Jogador*& lista, int& total, const Jogador& jogador) {
    Jogador* novaLista = new Jogador[total + 1];

    int posInsercao = total;
    for (int i = 0; i < total; i++) {
        if (jogador.posicao < lista[i].posicao) {
            posInsercao = i;
            break;
        }
    }

    for (int i = 0; i < posInsercao; i++) {
        novaLista[i] = lista[i];
    }

    novaLista[posInsercao] = jogador;

    for (int i = posInsercao; i < total; i++) {
        novaLista[i + 1] = lista[i];
    }

    delete[] lista;
    lista = novaLista;
    total++;
}

// remove um jogador de uma lista dinamica pelo indice e devolve o jogador removido
Jogador removerJogadorIndice(Jogador*& lista, int& total, int indice) {
    Jogador removido = lista[indice];

    if (total == 1) {
        delete[] lista;
        lista = nullptr;
        total = 0;
        return removido;
    }

    Jogador* novaLista = new Jogador[total - 1];
    int j = 0;

    for (int i = 0; i < total; i++) {
        if (i != indice) {
            novaLista[j] = lista[i];
            j++;
        }
    }

    delete[] lista;
    lista = novaLista;
    total--;

    return removido;
}

// procura um jogador pelo numero numa lista e devolve o indice
int procurarJogadorPorNumero(Jogador* lista, int total, int numero) {
    for (int i = 0; i < total; i++) {
        if (lista[i].numero == numero) {
            return i;
        }
    }
    return -1;
}
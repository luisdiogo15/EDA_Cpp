#include "jogo.h"

// adiciona um jogador a uma lista dinamica,
// mantendo a ordem por posicao
void adicionarJogadorOrdenado(Jogador*& lista, int& total, const Jogador& jogador) {
    // cria uma nova lista com mais 1 posicao
    Jogador* novaLista = new Jogador[total + 1];

    // por defeito, se nao encontrar sitio antes, entra no fim
    int posInsercao = total;

    // procura a primeira posicao onde o novo jogador deve entrar
    // para manter a lista ordenada por posicao
    for (int i = 0; i < total; i++) {
        if (jogador.posicao < lista[i].posicao) {
            posInsercao = i;
            break;
        }
    }

    // copia os jogadores antes da posicao de insercao
    for (int i = 0; i < posInsercao; i++) {
        novaLista[i] = lista[i];
    }

    // coloca o novo jogador no sitio certo
    novaLista[posInsercao] = jogador;

    // copia os restantes jogadores uma posicao para a frente
    for (int i = posInsercao; i < total; i++) {
        novaLista[i + 1] = lista[i];
    }

    // apaga a lista antiga e atualiza o ponteiro
    delete[] lista;
    lista = novaLista;

    // aumenta o numero total de jogadores da lista
    total++;
}

// remove um jogador de uma lista dinamica pelo indice
// e devolve o jogador removido
Jogador removerJogadorIndice(Jogador*& lista, int& total, int indice) {
    // guarda o jogador que vai ser removido
    Jogador removido = lista[indice];

    // caso especial: se a lista so tiver 1 elemento,
    // depois de remover fica vazia
    if (total == 1) {
        delete[] lista;
        lista = nullptr;
        total = 0;
        return removido;
    }

    // cria uma nova lista com menos 1 posicao
    Jogador* novaLista = new Jogador[total - 1];
    int j = 0;

    // copia todos os jogadores menos o que foi removido
    for (int i = 0; i < total; i++) {
        if (i != indice) {
            novaLista[j] = lista[i];
            j++;
        }
    }

    // apaga a lista antiga e atualiza o ponteiro
    delete[] lista;
    lista = novaLista;

    // diminui o numero total de jogadores
    total--;

    // devolve o jogador removido,
    // para ele poder ser usado noutra lista
    return removido;
}

// procura um jogador pelo numero numa lista
// e devolve o indice onde ele esta
int procurarJogadorPorNumero(Jogador* lista, int total, int numero) {
    for (int i = 0; i < total; i++) {
        if (lista[i].numero == numero) {
            return i;
        }
    }

    // se nao encontrar, devolve -1
    return -1;
}
#include <iostream>
#include "jornada.h"
#include "inic.h"
#include "gestao.h"
#include "util.h"

using namespace std;

// diminui os contadores de lesao/castigo e traz de volta quem ja pode regressar
void atualizarRetornos(Clube& clube) {
    for (int i = 0; i < clube.totalLesionados; ) {
        clube.lesionados[i].jogosLesao--;

        if (clube.lesionados[i].jogosLesao <= 0) {
            Jogador j = removerJogadorIndice(clube.lesionados, clube.totalLesionados, i);
            j.jogosLesao = 0;
            adicionarJogadorOrdenado(clube.plantel, clube.totalPlantel, j);
        } else {
            i++;
        }
    }

    for (int i = 0; i < clube.totalCastigados; ) {
        clube.castigados[i].jogosCastigo--;

        if (clube.castigados[i].jogosCastigo <= 0) {
            Jogador j = removerJogadorIndice(clube.castigados, clube.totalCastigados, i);
            j.jogosCastigo = 0;
            adicionarJogadorOrdenado(clube.plantel, clube.totalPlantel, j);
        } else {
            i++;
        }
    }
}

// adiciona 2 jogadores novos a lista de transferencias a cada jornada
void adicionarNovasTransferencias(Clube& clube, const string& ficheiroNomes) {
    string nomes[1000];
    int totalNomes = carregarNomes(nomes, 1000, ficheiroNomes);

    for (int i = 0; i < 2; i++) {
        Jogador j = gerarJogadorTransferencia(nomes, totalNomes);
        adicionarJogadorOrdenado(clube.transferencias, clube.totalTransferencias, j);
    }
}

// tenta encontrar suplente da mesma posicao que ainda nao foi usado
int encontrarSuplenteMesmaPosicao(const Clube& clube, Posicao pos, bool usadoSuplente[]) {
    for (int i = 0; i < clube.totalSuplentes; i++) {
        if (!usadoSuplente[i] && clube.suplentes[i].posicao == pos) {
            return i;
        }
    }
    return -1; // vai ao banco ver se tem jogador e retorna o nº da cadeira onde esta para a posicao precisa se nao retorna -1 pq nao ha jogador
}

// se nao houver da mesma posicao, escolhe qualquer suplente disponivel
int encontrarQualquerSuplente(const Clube& clube, bool usadoSuplente[]) {
    for (int i = 0; i < clube.totalSuplentes; i++) {
        if (!usadoSuplente[i]) {
            return i;
        }
    }
    return -1;
}

// simula uma jornada do EDA FC
bool simularProximaJornada(Clube& clube, Campeonato& camp, const string& ficheiroNomes) {
    // o EDA FC joga 2 vezes com cada uma das 17 equipas -> 34 jornadas
    if (camp.jornadaAtual > 34){
        cout << "\nCampeonato do EDA FC terminado.\n";
        return false;
    }

    // antes de escolher equipa, atualizamos regressos
    atualizarRetornos(clube);

    // escolhe automaticamente a equipa para esta jornada
    atualizarConvocados(clube);

    // descobre o adversario da jornada atual
    int indiceAdversario = (camp.jornadaAtual - 1) % 17;
    string adversario = camp.adversarios[indiceAdversario];

    int totalGolosNoJogo = gerarNumero(0, 8);
    int golosEDA = gerarNumero(0, totalGolosNoJogo);
    int golosADV = totalGolosNoJogo - golosEDA;

    // atualiza pontos
    if (golosEDA > golosADV) {
        clube.pontos += 3;
    } else if (golosEDA == golosADV) {
        clube.pontos += 1; // caso seja derrota como soma 0 pontos nao precisamos criar a hipotese de marcar menos golos q o adv
    } 

    // limpa resumo de substituicoes anterior
    clube.totalResumoSubs = 0;

    // controla suplentes ja usados e limite de substituicoes
    bool usadoSuplente[6] = {false, false, false, false, false, false}; //nng entrou ainda
    int totalSubstituicoes = 0; // respeitar o max das 3 subs por jogo

    // vamos analisar lesoes e castigos nos titulares desta jornada
    for (int i = 0; i < clube.totalTitulares; i++) {
        int numeroJogador = clube.titulares[i].numero;
        int indicePlantel = procurarJogadorPorNumero(clube.plantel, clube.totalPlantel, numeroJogador);

        if (indicePlantel == -1) {
            continue;
        }

        bool lesionou = false;

        // verifica lesao
        if (gerarNumero(0, 99) < clube.plantel[indicePlantel].probLesao) {
            lesionou = true;

            // tenta fazer substituicao, ate ao maximo de 3
            if (totalSubstituicoes < 3) {
                int idxSub = encontrarSuplenteMesmaPosicao(clube, clube.plantel[indicePlantel].posicao, usadoSuplente);

                if (idxSub == -1) {
                    idxSub = encontrarQualquerSuplente(clube, usadoSuplente);
                }

                if (idxSub != -1) {
                    usadoSuplente[idxSub] = true;
                    string texto = clube.plantel[indicePlantel].nome + " -> " + clube.suplentes[idxSub].nome;
                    clube.resumoSubs[clube.totalResumoSubs] = texto;
                    clube.totalResumoSubs++;
                    totalSubstituicoes++;
                }
            }

            Jogador j = removerJogadorIndice(clube.plantel, clube.totalPlantel, indicePlantel);
            j.jogosLesao = gerarNumero(1, 10);
            adicionarJogadorOrdenado(clube.lesionados, clube.totalLesionados, j);
            continue;
        }

        // procura de novo porque o plantel pode ter mudado
        indicePlantel = procurarJogadorPorNumero(clube.plantel, clube.totalPlantel, numeroJogador);

        if (indicePlantel == -1 || lesionou) {
            continue;
        }

        // verifica castigo
        if (gerarNumero(0, 99) < clube.plantel[indicePlantel].probCastigo) {
            Jogador j = removerJogadorIndice(clube.plantel, clube.totalPlantel, indicePlantel);
            j.jogosCastigo = gerarNumero(1, 10);
            adicionarJogadorOrdenado(clube.castigados, clube.totalCastigados, j);
        }
    }

    // no fim da jornada entram 2 novos jogadores para transferencias
    adicionarNovasTransferencias(clube, ficheiroNomes);

    // guarda resumo da jornada para mostrar no ecrã seguinte
    clube.haResumo = true;
    clube.ultimoAdversario = adversario;
    clube.ultGolosEDA = golosEDA;
    clube.ultGolosADV = golosADV;

    // avanca para a proxima jornada
    camp.jornadaAtual++;

    // recalcula convocados para o estado apos a jornada
    atualizarConvocados(clube);

    return true;
}
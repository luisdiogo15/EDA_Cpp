#include <iostream>
#include <iomanip>
#include "gestao.h"

using namespace std;

// devolve o indice do melhor jogador de uma posicao que ainda nao foi usado
int melhorJogadorPorPosicao(const Clube& clube, Posicao pos, bool usados[]) {
    int melhorIndice = -1;

    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!usados[i] && clube.plantel[i].posicao == pos) {
            if (melhorIndice == -1 || clube.plantel[i].qualidade > clube.plantel[melhorIndice].qualidade) {
                melhorIndice = i;
            }
        }
    }

    return melhorIndice;
}

// devolve o indice do melhor jogador restante, qualquer posicao
int melhorJogadorRestante(const Clube& clube, bool usados[]) {
    int melhorIndice = -1;

    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!usados[i]) {
            if (melhorIndice == -1 || clube.plantel[i].qualidade > clube.plantel[melhorIndice].qualidade) {
                melhorIndice = i;
            }
        }
    }

    return melhorIndice;
}

// escolhe titulares automaticamente
void escolherTitulares(Clube& clube, bool usados[]) {
    clube.totalTitulares = 0;
    clube.titulares = new Jogador[11];

    int indice;

    // 1 GR
    indice = melhorJogadorPorPosicao(clube, GR, usados);
    if (indice != -1) {
        clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
        usados[indice] = true;
    }

    // DEF
    for (int i = 0; i < clube.taticaDEF; i++) {
        indice = melhorJogadorPorPosicao(clube, DEF, usados);
        if (indice != -1) {
            clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // MED
    for (int i = 0; i < clube.taticaMED; i++) {
        indice = melhorJogadorPorPosicao(clube, MED, usados);
        if (indice != -1) {
            clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // AVA
    for (int i = 0; i < clube.taticaAVA; i++) {
        indice = melhorJogadorPorPosicao(clube, AVA, usados);
        if (indice != -1) {
            clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }
}

// escolhe suplentes automaticamente
void escolherSuplentes(Clube& clube, bool usados[]) {
    clube.totalSuplentes = 0;
    clube.suplentes = new Jogador[6];

    int indice;

    // 1 GR
    indice = melhorJogadorPorPosicao(clube, GR, usados);
    if (indice != -1) {
        clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
        usados[indice] = true;
    }

    // 2 DEF
    for (int i = 0; i < 2; i++) {
        indice = melhorJogadorPorPosicao(clube, DEF, usados);
        if (indice != -1) {
            clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // 2 MED
    for (int i = 0; i < 2; i++) {
        indice = melhorJogadorPorPosicao(clube, MED, usados);
        if (indice != -1) {
            clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
            usados[indice] = true;
        }
    }

    // 1 AVA
    indice = melhorJogadorPorPosicao(clube, AVA, usados);
    if (indice != -1) {
        clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
        usados[indice] = true;
    }

    // se faltarem suplentes, preenche com os melhores restantes
    while (clube.totalSuplentes < 6) {
        indice = melhorJogadorRestante(clube, usados);
        if (indice == -1) {
            break;
        }

        clube.suplentes[clube.totalSuplentes++] = clube.plantel[indice];
        usados[indice] = true;
    }
}

// atualiza titulares, suplentes e nao convocados
void atualizarConvocados(Clube& clube) {
    delete[] clube.titulares;
    delete[] clube.suplentes;
    delete[] clube.naoConvocados;

    clube.titulares = nullptr;
    clube.suplentes = nullptr;
    clube.naoConvocados = nullptr;

    clube.totalTitulares = 0;
    clube.totalSuplentes = 0;
    clube.totalNaoConvocados = 0;

    bool* usados = new bool[clube.totalPlantel];
    for (int i = 0; i < clube.totalPlantel; i++) {
        usados[i] = false;
    }

    escolherTitulares(clube, usados);
    escolherSuplentes(clube, usados);

    // conta nao convocados
    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!usados[i]) {
            clube.totalNaoConvocados++;
        }
    }

    if (clube.totalNaoConvocados > 0) {
        clube.naoConvocados = new Jogador[clube.totalNaoConvocados];

        int j = 0;
        for (int i = 0; i < clube.totalPlantel; i++) {
            if (!usados[i]) {
                clube.naoConvocados[j] = clube.plantel[i];
                j++;
            }
        }
    }

    delete[] usados;
}

// mostra lista simples de jogadores
void mostrarListaSimples(Jogador* lista, int total) {
    if (total == 0) {
        cout << "(vazio)" << endl;
        return;
    }

    for (int i = 0; i < total; i++) {
        cout << lista[i].numero << " - "
             << lista[i].nome << " - "
             << posicaoParaTexto(lista[i].posicao) << " - Q "
             << lista[i].qualidade << endl;
    }
}

// mostra estado completo
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp) {
    const int wNome = 24;
    const int wNum = 4;
    const int wPos = 7;
    const int wIdade = 5;
    const int wLesao = 10;
    const int wCastigo = 12;
    const int wQualidade = 10;
    const int wTreino = 11;

    cout << "\n****************************************\n";
    cout << "* " << clube.nome << " - " << camp.jornadaAtual
         << "a Jornada - " << clube.pontos << " pontos. *\n";
    cout << "****************************************\n";

    // mostra resumo da jornada anterior, se existir
    if (clube.haResumo) {
        cout << "\nResultado Anterior:\n";
        cout << "Resultado: " << clube.nome << " " << clube.ultGolosEDA
             << " - " << clube.ultGolosADV << " " << clube.ultimoAdversario << "\n";

        cout << "\nSubstituicoes:\n";
        if (clube.totalResumoSubs == 0) {
            cout << "(nenhuma)\n";
        } else {
            for (int i = 0; i < clube.totalResumoSubs; i++) {
                cout << clube.resumoSubs[i] << endl;
            }
        }
        cout << endl;
    }

    cout << "*********************** Plantel Disponivel: ***********************\n";

    cout << left
         << setw(wNome)      << "Nome"
         << " | " << setw(wNum)       << "Nr"
         << " | " << setw(wPos)       << "Posicao"
         << " | " << setw(wIdade)     << "Idade"
         << " | " << setw(wLesao)     << "ProbLesao"
         << " | " << setw(wCastigo)   << "ProbCastigo"
         << " | " << setw(wQualidade) << "Qualidade"
         << " | " << setw(wTreino)    << "Dias-Treino"
         << endl;

    cout << string(
        wNome + wNum + wPos + wIdade + wLesao + wCastigo + wQualidade + wTreino + 21, '-'
    ) << endl;

    Posicao ultimaPosicao = GR;
    bool primeiraLinha = true;

    for (int i = 0; i < clube.totalPlantel; i++) {
        if (!primeiraLinha && clube.plantel[i].posicao != ultimaPosicao) {
            cout << endl;
        }

        cout << left
             << setw(wNome)      << clube.plantel[i].nome
             << " | " << setw(wNum)       << clube.plantel[i].numero
             << " | " << setw(wPos)       << posicaoParaTexto(clube.plantel[i].posicao)
             << " | " << setw(wIdade)     << clube.plantel[i].idade
             << " | " << setw(wLesao)     << (to_string(clube.plantel[i].probLesao) + "%")
             << " | " << setw(wCastigo)   << (to_string(clube.plantel[i].probCastigo) + "%")
             << " | " << setw(wQualidade) << clube.plantel[i].qualidade
             << " | " << setw(wTreino)    << clube.plantel[i].diasTreino
             << endl;

        ultimaPosicao = clube.plantel[i].posicao;
        primeiraLinha = false;
    }

    cout << string(
        wNome + wNum + wPos + wIdade + wLesao + wCastigo + wQualidade + wTreino + 21, '-'
    ) << endl;

    cout << "\nTitulares:\n";
    mostrarListaSimples(clube.titulares, clube.totalTitulares);

    cout << "\nSuplentes:\n";
    mostrarListaSimples(clube.suplentes, clube.totalSuplentes);

    cout << "\nNao convocados:\n";
    mostrarListaSimples(clube.naoConvocados, clube.totalNaoConvocados);

    cout << "\nLesionados:\n";
    mostrarListaSimples(clube.lesionados, clube.totalLesionados);

    cout << "\nCastigados:\n";
    mostrarListaSimples(clube.castigados, clube.totalCastigados);

    cout << "\n*****************************************************************************\n";
    cout << "Transferencias:\n";
    mostrarListaSimples(clube.transferencias, clube.totalTransferencias);

    cout << "\n(s)seguinte (q)sair\n";
}

// liberta memoria
void libertarMemoria(Clube& clube) {
    delete[] clube.plantel;
    delete[] clube.lesionados;
    delete[] clube.castigados;
    delete[] clube.transferencias;
    delete[] clube.titulares;
    delete[] clube.suplentes;
    delete[] clube.naoConvocados;

    clube.plantel = nullptr;
    clube.lesionados = nullptr;
    clube.castigados = nullptr;
    clube.transferencias = nullptr;
    clube.titulares = nullptr;
    clube.suplentes = nullptr;
    clube.naoConvocados = nullptr;
}
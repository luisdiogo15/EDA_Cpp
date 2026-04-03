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

// conta quantos jogadores de uma certa posicao existem numa lista
int contarPosicao(Jogador* lista, int total, Posicao pos) {
    int cont = 0;

    for (int i = 0; i < total; i++) {
        if (lista[i].posicao == pos) {
            cont++;
        }
    }

    return cont;
}

// devolve os limites minimos por posicao
int minimoPosicao(Posicao pos) {
    if (pos == GR) return 2;
    if (pos == DEF) return 7;
    if (pos == MED) return 7;
    return 4;
}

// devolve os limites maximos por posicao
int maximoPosicao(Posicao pos) {
    if (pos == GR) return 3;
    if (pos == DEF) return 10;
    if (pos == MED) return 10;
    return 7;
}

// procura jogador no plantel pelo numero
int procurarNoPlantel(const Clube& clube, int numero) {
    return procurarJogadorPorNumero(clube.plantel, clube.totalPlantel, numero);
}

// procura jogador na lista de lesionados pelo numero
int procurarLesionado(const Clube& clube, int numero) {
    return procurarJogadorPorNumero(clube.lesionados, clube.totalLesionados, numero);
}

// procura jogador na lista de castigados pelo numero
int procurarCastigado(const Clube& clube, int numero) {
    return procurarJogadorPorNumero(clube.castigados, clube.totalCastigados, numero);
}

// mostra plantel resumido com numero para facilitar escolha
void mostrarPlantelResumido(const Clube& clube) {
    cout << "\nPlantel:\n";
    for (int i = 0; i < clube.totalPlantel; i++) {
        cout << clube.plantel[i].numero << " - "
             << clube.plantel[i].nome << " - "
             << posicaoParaTexto(clube.plantel[i].posicao) << " - Q "
             << clube.plantel[i].qualidade << endl;
    }
}

// mostra lista resumida
void mostrarListaResumida(Jogador* lista, int total, const string& titulo) {
    cout << "\n" << titulo << ":\n";

    if (total == 0) {
        cout << "(vazio)" << endl;
        return;
    }

    for (int i = 0; i < total; i++) {
        cout << lista[i].numero << " - "
             << lista[i].nome << " - "
             << posicaoParaTexto(lista[i].posicao) << endl;
    }
}

// muda a posicao de um jogador, respeitando os limites por posicao
void mudarPosicaoJogador(Clube& clube) {
    mostrarPlantelResumido(clube);

    int numero;
    int novaPos;

    cout << "\nNumero do jogador: ";
    cin >> numero;

    int indice = procurarNoPlantel(clube, numero);

    if (indice == -1) {
        cout << "Jogador nao encontrado no plantel.\n";
        return;
    }

    cout << "Nova posicao (0-GR, 1-DEF, 2-MED, 3-AVA): ";
    cin >> novaPos;

    if (novaPos < 0 || novaPos > 3) {
        cout << "Posicao invalida.\n";
        return;
    }

    Posicao antiga = clube.plantel[indice].posicao;
    Posicao nova = static_cast<Posicao>(novaPos);

    if (antiga == nova) {
        cout << "O jogador ja esta nessa posicao.\n";
        return;
    }

    int totalNovaPos = contarPosicao(clube.plantel, clube.totalPlantel, nova);
    int totalAntigaPos = contarPosicao(clube.plantel, clube.totalPlantel, antiga);

    // nao deixa ultrapassar maximos da nova posicao
    if (totalNovaPos >= maximoPosicao(nova)) {
        cout << "Nao e possivel: limite maximo da nova posicao atingido.\n";
        return;
    }

    // nao deixa quebrar minimos da posicao antiga
    if (totalAntigaPos <= minimoPosicao(antiga)) {
        cout << "Nao e possivel: a posicao atual ficaria abaixo do minimo.\n";
        return;
    }

    // remove e volta a inserir para manter ordem por posicao
    Jogador j = removerJogadorIndice(clube.plantel, clube.totalPlantel, indice);
    j.posicao = nova;
    adicionarJogadorOrdenado(clube.plantel, clube.totalPlantel, j);

    atualizarConvocados(clube);

    cout << "Posicao alterada com sucesso.\n";
}

// melhora a qualidade de um jogador ate 5 semanas, +5 por semana, maximo 100
void melhorarQualidadeJogador(Clube& clube) {
    mostrarPlantelResumido(clube);

    int numero;
    int semanas;

    cout << "\nNumero do jogador: ";
    cin >> numero;

    int indice = procurarNoPlantel(clube, numero);

    if (indice == -1) {
        cout << "Jogador nao encontrado no plantel.\n";
        return;
    }

    cout << "Numero de semanas de treino (1 a 5): ";
    cin >> semanas;

    if (semanas < 1 || semanas > 5) {
        cout << "Numero de semanas invalido.\n";
        return;
    }

    clube.plantel[indice].qualidade += semanas * 5;

    if (clube.plantel[indice].qualidade > 100) {
        clube.plantel[indice].qualidade = 100;
    }

    clube.plantel[indice].diasTreino += semanas;

    atualizarConvocados(clube);

    cout << "Qualidade melhorada com sucesso.\n";
}

// menu do treino especifico
void menuTreinoEspecifico(Clube& clube) {
    int op;

    do {
        cout << "\n--- Treino Especifico ---\n";
        cout << "1 - Mudar posicao\n";
        cout << "2 - Melhorar qualidade\n";
        cout << "0 - Voltar\n";
        cout << "Opcao: ";
        cin >> op;

        if (op == 1) {
            mudarPosicaoJogador(clube);
        }
        else if (op == 2) {
            melhorarQualidadeJogador(clube);
        }

    } while (op != 0);
}

// altera a tatica respeitando as restricoes minimas
void menuAlterarTatica(Clube& clube) {
    int def, med, ava;

    cout << "\n--- Alterar Tatica ---\n";
    cout << "Numero de defesas: ";
    cin >> def;
    cout << "Numero de medios: ";
    cin >> med;
    cout << "Numero de avancados: ";
    cin >> ava;

    // 1 GR fixo
    if (1 + def + med + ava != 11) {
        cout << "Tatica invalida: tem de totalizar 11 jogadores.\n";
        return;
    }

    if (def < 3 || med < 2 || ava < 1) {
        cout << "Tatica invalida: minimos sao 3 DEF, 2 MED e 1 AVA.\n";
        return;
    }

    clube.taticaDEF = def;
    clube.taticaMED = med;
    clube.taticaAVA = ava;

    atualizarConvocados(clube);

    cout << "Tatica alterada com sucesso.\n";
}

// aplica lesao a um jogador do plantel
void aplicarLesao(Clube& clube) {
    mostrarPlantelResumido(clube);

    int numero;
    int jogos;

    cout << "\nNumero do jogador a lesionar: ";
    cin >> numero;

    int indice = procurarNoPlantel(clube, numero);

    if (indice == -1) {
        cout << "Jogador nao encontrado no plantel.\n";
        return;
    }

    cout << "Numero de jornadas lesionado (1 a 10): ";
    cin >> jogos;

    if (jogos < 1 || jogos > 10) {
        cout << "Valor invalido.\n";
        return;
    }

    Jogador j = removerJogadorIndice(clube.plantel, clube.totalPlantel, indice);
    j.jogosLesao = jogos;
    adicionarJogadorOrdenado(clube.lesionados, clube.totalLesionados, j);

    atualizarConvocados(clube);

    cout << "Lesao aplicada com sucesso.\n";
}

// reduz lesao e traz de volta se chegar a 0
void reduzirLesao(Clube& clube) {
    mostrarListaResumida(clube.lesionados, clube.totalLesionados, "Lesionados");

    int numero;
    int valor;

    cout << "\nNumero do jogador lesionado: ";
    cin >> numero;

    int indice = procurarLesionado(clube, numero);

    if (indice == -1) {
        cout << "Jogador nao encontrado na lista de lesionados.\n";
        return;
    }

    cout << "Quantas jornadas reduzir: ";
    cin >> valor;

    if (valor < 1) {
        cout << "Valor invalido.\n";
        return;
    }

    clube.lesionados[indice].jogosLesao -= valor;

    if (clube.lesionados[indice].jogosLesao <= 0) {
        Jogador j = removerJogadorIndice(clube.lesionados, clube.totalLesionados, indice);
        j.jogosLesao = 0;
        adicionarJogadorOrdenado(clube.plantel, clube.totalPlantel, j);
        cout << "Jogador regressou ao plantel.\n";
    } else {
        cout << "Lesao reduzida com sucesso.\n";
    }

    atualizarConvocados(clube);
}

// aplica castigo a um jogador do plantel
void aplicarCastigo(Clube& clube) {
    mostrarPlantelResumido(clube);

    int numero;
    int jogos;

    cout << "\nNumero do jogador a castigar: ";
    cin >> numero;

    int indice = procurarNoPlantel(clube, numero);

    if (indice == -1) {
        cout << "Jogador nao encontrado no plantel.\n";
        return;
    }

    cout << "Numero de jogos de castigo (1 a 10): ";
    cin >> jogos;

    if (jogos < 1 || jogos > 10) {
        cout << "Valor invalido.\n";
        return;
    }

    Jogador j = removerJogadorIndice(clube.plantel, clube.totalPlantel, indice);
    j.jogosCastigo = jogos;
    adicionarJogadorOrdenado(clube.castigados, clube.totalCastigados, j);

    atualizarConvocados(clube);

    cout << "Castigo aplicado com sucesso.\n";
}

// reduz castigo e traz de volta se chegar a 0
void reduzirCastigo(Clube& clube) {
    mostrarListaResumida(clube.castigados, clube.totalCastigados, "Castigados");

    int numero;
    int valor;

    cout << "\nNumero do jogador castigado: ";
    cin >> numero;

    int indice = procurarCastigado(clube, numero);

    if (indice == -1) {
        cout << "Jogador nao encontrado na lista de castigados.\n";
        return;
    }

    cout << "Quantos jogos reduzir: ";
    cin >> valor;

    if (valor < 1) {
        cout << "Valor invalido.\n";
        return;
    }

    clube.castigados[indice].jogosCastigo -= valor;

    if (clube.castigados[indice].jogosCastigo <= 0) {
        Jogador j = removerJogadorIndice(clube.castigados, clube.totalCastigados, indice);
        j.jogosCastigo = 0;
        adicionarJogadorOrdenado(clube.plantel, clube.totalPlantel, j);
        cout << "Jogador regressou ao plantel.\n";
    } else {
        cout << "Castigo reduzido com sucesso.\n";
    }

    atualizarConvocados(clube);
}

// menu de gestao manual de lesoes e castigos
void menuGestaoFisicaDisciplinar(Clube& clube) {
    int op;

    do {
        cout << "\n--- Gestao Fisica e Disciplinar ---\n";
        cout << "1 - Aplicar lesao\n";
        cout << "2 - Reduzir lesao\n";
        cout << "3 - Aplicar castigo\n";
        cout << "4 - Reduzir castigo\n";
        cout << "0 - Voltar\n";
        cout << "Opcao: ";
        cin >> op;

        if (op == 1) aplicarLesao(clube);
        else if (op == 2) reduzirLesao(clube);
        else if (op == 3) aplicarCastigo(clube);
        else if (op == 4) reduzirCastigo(clube);

    } while (op != 0);
}

// menu geral de gestao
void menuGestao(Clube& clube) {
    int op;

    do {
        cout << "\n========== MENU DE GESTAO ==========\n";
        cout << "1 - Treino especifico\n";
        cout << "2 - Alterar tatica\n";
        cout << "3 - Gestao fisica e disciplinar\n";
        cout << "0 - Voltar\n";
        cout << "Opcao: ";
        cin >> op;

        if (op == 1) menuTreinoEspecifico(clube);
        else if (op == 2) menuAlterarTatica(clube);
        else if (op == 3) menuGestaoFisicaDisciplinar(clube);

    } while (op != 0);
}
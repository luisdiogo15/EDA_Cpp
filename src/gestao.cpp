#include <iostream>
#include <iomanip>
#include "gestao.h"
#include "util.h"

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

    // se por lesoes/castigos nao der para cumprir a tatica, preenche com os melhores restantes
    while (clube.totalTitulares < 11) {
        indice = melhorJogadorRestante(clube, usados);
        if (indice == -1) {
            break;
        }

        clube.titulares[clube.totalTitulares++] = clube.plantel[indice];
        usados[indice] = true;
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

    // se nao der para respeitar os criterios por posicao, completa com os melhores restantes
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

// mostra uma tabela organizada de jogadores
void mostrarTabelaJogadores(Jogador* lista, int total, bool mostrarJogosLesao, bool mostrarJogosCastigo, bool mostrarDiasTreino) {
    const int wNome = 22;
    const int wNr = 4;
    const int wPos = 8;
    const int wIdade = 6;
    const int wProbLesao = 11;
    const int wProbCastigo = 13;
    const int wQualidade = 11;
    const int wExtra = 14;
    const int wTreino = 12;

    if (total == 0) {
        cout << "(vazio)\n";
        return;
    }

    cout << left
         << setw(wNome) << "Nome"
         << " | " << setw(wNr) << "Nr"
         << " | " << setw(wPos) << "Posicao"
         << " | " << setw(wIdade) << "Idade"
         << " | " << setw(wProbLesao) << "ProbLesao"
         << " | " << setw(wProbCastigo) << "ProbCastigo"
         << " | " << setw(wQualidade) << "Qualidade";

    if (mostrarJogosCastigo) {
        cout << " | " << setw(wExtra) << "JogosCastigo";
    }

    if (mostrarJogosLesao) {
        cout << " | " << setw(wExtra) << "JogosLesao";
    }

    if (mostrarDiasTreino) {
        cout << " | " << setw(wTreino) << "Dias-Treino";
    }

    cout << endl;
    cout << string(120, '-') << endl;

    for (int i = 0; i < total; i++) {
        cout << left
             << setw(wNome) << lista[i].nome
             << " | " << setw(wNr) << lista[i].numero
             << " | " << setw(wPos) << posicaoParaTexto(lista[i].posicao)
             << " | " << setw(wIdade) << lista[i].idade
             << " | " << setw(wProbLesao) << (to_string(lista[i].probLesao) + "%")
             << " | " << setw(wProbCastigo) << (to_string(lista[i].probCastigo) + "%")
             << " | " << setw(wQualidade) << lista[i].qualidade;

        if (mostrarJogosCastigo) {
            cout << " | " << setw(wExtra) << lista[i].jogosCastigo;
        }

        if (mostrarJogosLesao) {
            cout << " | " << setw(wExtra) << lista[i].jogosLesao;
        }

        if (mostrarDiasTreino) {
            cout << " | " << setw(wTreino) << lista[i].diasTreino;
        }

        cout << endl;
    }
}

// mostra o resumo da jornada anterior na ordem certa:
// 11 Inicial -> Suplentes -> Castigados -> Lesionados -> Substituicoes
void mostrarResumoJornadaAnterior(const Clube& clube) {
    if (!clube.haResumo) {
        return;
    }

    cout << "\nResultado Anterior:\n";
    cout << "Resultado: " << clube.nome << " " << clube.ultGolosEDA
         << " - " << clube.ultGolosADV << " " << clube.ultimoAdversario << "\n\n";

    cout << "11 Inicial\n";
    mostrarTabelaJogadores(clube.titulares, clube.totalTitulares, false, false, false);

    cout << "\nSuplentes\n";
    mostrarTabelaJogadores(clube.suplentes, clube.totalSuplentes, false, false, false);

    cout << "\nCastigados:\n";
    mostrarTabelaJogadores(clube.castigados, clube.totalCastigados, false, true, false);

    cout << "\nLesionados:\n";
    mostrarTabelaJogadores(clube.lesionados, clube.totalLesionados, true, false, false);

    cout << "\nSubstituicoes:\n";
    if (clube.totalResumoSubs == 0) {
        cout << "(nenhuma)\n";
    } else {
        for (int i = 0; i < clube.totalResumoSubs; i++) {
            cout << clube.resumoSubs[i] << endl;
        }
    }
}

// mostra apenas o estado do plantel para a proxima jornada
void mostrarEstadoAntesProximaJornada(const Clube& clube) {
    cout << "\n*********************** Plantel Disponivel: ***********************\n";
    mostrarTabelaJogadores(clube.plantel, clube.totalPlantel, false, false, true);

    cout << "\nCastigados:\n";
    mostrarTabelaJogadores(clube.castigados, clube.totalCastigados, false, true, true);

    cout << "\nLesionados:\n";
    mostrarTabelaJogadores(clube.lesionados, clube.totalLesionados, true, false, true);

    cout << "\n*****************************************************************************\n";
    cout << "Transferencias:\n";
    mostrarTabelaJogadores(clube.transferencias, clube.totalTransferencias, false, false, false);
}

// procura um jogador numa lista pelo numero
int procurarJogadorLista(Jogador* lista, int total, int numero) {
    for (int i = 0; i < total; i++) {
        if (lista[i].numero == numero) {
            return i;
        }
    }
    return -1;
}

// troca dois jogadores entre si
void trocarJogadoresListas(Jogador& a, Jogador& b) {
    Jogador temp = a;
    a = b;
    b = temp;
}

// mostra titulares, suplentes e nao convocados para alteracoes manuais
void mostrarConvocadosManuais(const Clube& clube) {
    cout << "\n===== TITULARES =====\n";
    mostrarTabelaJogadores(clube.titulares, clube.totalTitulares, false, false, false);

    cout << "\n===== SUPLENTES =====\n";
    mostrarTabelaJogadores(clube.suplentes, clube.totalSuplentes, false, false, false);

    cout << "\n===== NAO CONVOCADOS =====\n";
    mostrarTabelaJogadores(clube.naoConvocados, clube.totalNaoConvocados, false, false, false);
}

// menu de alteracoes manuais antes do jogo
void menuAlteracoesManuais(Clube& clube) {
    int op;

    do {
        mostrarConvocadosManuais(clube);

        cout << "\n--- Alteracoes Manuais ---\n";
        cout << "1 - Trocar titular por suplente\n";
        cout << "2 - Trocar suplente por nao convocado\n";
        cout << "3 - Trocar titular por nao convocado\n";
        cout << "0 - Voltar\n";
        cout << "Opcao: ";
        cin >> op;

        if (op == 1) {
            int numTitular, numSuplente;

            cout << "Numero do titular: ";
            cin >> numTitular;
            cout << "Numero do suplente: ";
            cin >> numSuplente;

            int i = procurarJogadorLista(clube.titulares, clube.totalTitulares, numTitular);
            int j = procurarJogadorLista(clube.suplentes, clube.totalSuplentes, numSuplente);

            if (i == -1 || j == -1) {
                cout << "Jogador nao encontrado na lista certa.\n";
            } else {
                trocarJogadoresListas(clube.titulares[i], clube.suplentes[j]);
                clube.alteracoesManuaisAtivas = true;
                cout << "Troca realizada com sucesso.\n";
            }
        }
        else if (op == 2) {
            int numSuplente, numNaoConvocado;

            cout << "Numero do suplente: ";
            cin >> numSuplente;
            cout << "Numero do nao convocado: ";
            cin >> numNaoConvocado;

            int i = procurarJogadorLista(clube.suplentes, clube.totalSuplentes, numSuplente);
            int j = procurarJogadorLista(clube.naoConvocados, clube.totalNaoConvocados, numNaoConvocado);

            if (i == -1 || j == -1) {
                cout << "Jogador nao encontrado na lista certa.\n";
            } else {
                trocarJogadoresListas(clube.suplentes[i], clube.naoConvocados[j]);
                clube.alteracoesManuaisAtivas = true;
                cout << "Troca realizada com sucesso.\n";
            }
        }
        else if (op == 3) {
            int numTitular, numNaoConvocado;

            cout << "Numero do titular: ";
            cin >> numTitular;
            cout << "Numero do nao convocado: ";
            cin >> numNaoConvocado;

            int i = procurarJogadorLista(clube.titulares, clube.totalTitulares, numTitular);
            int j = procurarJogadorLista(clube.naoConvocados, clube.totalNaoConvocados, numNaoConvocado);

            if (i == -1 || j == -1) {
                cout << "Jogador nao encontrado na lista certa.\n";
            } else {
                trocarJogadoresListas(clube.titulares[i], clube.naoConvocados[j]);
                clube.alteracoesManuaisAtivas = true;
                cout << "Troca realizada com sucesso.\n";
            }
        }

    } while (op != 0);
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

    if (totalNovaPos >= maximoPosicao(nova)) {
        cout << "Nao e possivel: limite maximo da nova posicao atingido.\n";
        return;
    }

    if (totalAntigaPos <= minimoPosicao(antiga)) {
        cout << "Nao e possivel: a posicao atual ficaria abaixo do minimo.\n";
        return;
    }

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
        cout << "4 - Alteracoes manuais\n";
        cout << "0 - Voltar\n";
        cout << "Opcao: ";
        cin >> op;

        if (op == 1) menuTreinoEspecifico(clube);
        else if (op == 2) menuAlterarTatica(clube);
        else if (op == 3) menuGestaoFisicaDisciplinar(clube);
        else if (op == 4) menuAlteracoesManuais(clube);

    } while (op != 0);
}

// mostra o estado completo do clube na ordem do modelo
void mostrarEstadoClube(const Clube& clube, const Campeonato& camp) {
    cout << "\n****************************************\n";
    cout << "* " << clube.nome << " - " << camp.jornadaAtual
         << "a Jornada - " << clube.pontos << " pontos. *\n";
    cout << "****************************************\n";

    // primeiro mostra o resumo da jornada anterior
    mostrarResumoJornadaAnterior(clube);

    // depois mostra o estado do plantel antes da proxima jornada
    mostrarEstadoAntesProximaJornada(clube);

    cout << "\n(s)seguinte (o)opcoes (g)gravar (c)carregar (q)sair\n";
} 
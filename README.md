# EDA FC · Simulador de futebol

**1.º projeto prático de Estruturas de Dados e Algoritmos · 2025/2026 · Grupo 2**

Simulador em C++ para gerir o plantel do EDA FC e acompanhar o clube ao longo de uma época de 34 jornadas, com dois jogos contra cada um dos 17 adversários. A interação é feita no terminal.

## Funcionalidades

- **Convocatória.** Seleção automática de titulares, suplentes e não convocados, com trocas manuais para a próxima jornada.
- **Gestão do plantel.** Alteração da tática, treino para mudar a posição ou melhorar a qualidade de um jogador, e gestão de lesões e castigos.
- **Simulação de jornadas.** Geração do resultado, atualização da pontuação, registo de substituições e entrada de novos jogadores na lista de transferências.
- **Gravação e retoma.** Gravação do progresso num ficheiro de texto e carregamento a partir do menu.

## Compilar e executar

É necessário um compilador com suporte para C++11. A partir da raiz do repositório, execute os comandos abaixo. O programa procura `nomes.txt` e `equipas.txt` no diretório atual, por isso é executado dentro de `src/`.

```sh
cd src
g++ -std=c++11 *.cpp -o eda_fc.out
./eda_fc.out
```

| Tecla | Ação |
| :---: | --- |
| `s` | Simular a próxima jornada |
| `o` | Abrir o menu de gestão |
| `g` | Gravar o progresso em `src/save.txt` |
| `c` | Carregar o progresso de `src/save.txt` |
| `q` | Sair |

## Organização do código

As listas de jogadores são guardadas em **arrays dinâmicos**. Quando um jogador muda de lista, como acontece numa lesão ou num regresso ao plantel, o programa remove-o do array de origem e insere-o no de destino.

| Módulo | Responsabilidade |
| --- | --- |
| [`main.cpp`](src/main.cpp) | Menu principal e ligação entre os módulos |
| [`jogo.cpp`](src/jogo.cpp) e [`memoria.cpp`](src/memoria.cpp) | Operações sobre os arrays de jogadores e libertação de memória |
| [`inic.cpp`](src/inic.cpp) e [`ficheiros.cpp`](src/ficheiros.cpp) | Inicialização a partir dos dados de exemplo, gravação e carregamento |
| [`gestao.cpp`](src/gestao.cpp) e [`jornada.cpp`](src/jornada.cpp) | Gestão do plantel e simulação das jornadas |

## Autores

| Autor | Principais contribuições |
| --- | --- |
| [Luís Diogo](https://github.com/luisdiogo15) | Estruturas de dados, operações sobre arrays dinâmicos e funções auxiliares |
| [Tomás Pita](https://github.com/tpita10) | Inicialização do clube, leitura de ficheiros, gravação e carregamento do progresso, e libertação de memória |
| Martim Abreu | Gestão do plantel, convocatórias, treino e tática |
| [Rodrigo Abreu](https://github.com/rodrigoabreu970) | Simulação de jornadas, atualização do estado do clube e integração dos módulos |

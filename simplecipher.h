#ifndef SIMPLECIPHER_H
#define SIMPLECIPHER_H

#include <QList>
#include <QString>
#include <QByteArray>
#include <QDebug>

// lista de parametros
#define TAM_BLOCO 4
#define TAM_CHAVE 2
#define DRIFT_PERM_INI 3
#define DRIFT_PERM_FIN 3

// operacoes bit-a-bit
int valorBit(int valor, int posicao);
int setBit(int valor, int posicao);
int resetBit(int valor, int posicao);

// gerar lista de blocos
QList<QByteArray> gerarListaBlocos(QByteArray input);

// print lista de blocos
void printListaBlocos(QList<QByteArray> input);

// realizar/desfazer permutacao inicial
QList<QByteArray> realizarPermutacaoInicial(QList<QByteArray> input);
QList<QByteArray> desfazerPermutacaoInicial(QList<QByteArray> input);

//Realiza swap
QByteArray swapper(QByteArray inputBlock);

// realizar/desfazer permutacao final

// expandir chave

// aplicar uma 'rodada'

#endif // SIMPLECIPHER_H

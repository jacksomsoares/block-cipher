#ifndef SIMPLECIPHER_H
#define SIMPLECIPHER_H

// TODO VERIFICAR E ATUALIZAR!

#include <QList>
#include <QString>
#include <QByteArray>
#include <QDebug>

// lista de parametros
#define TAM_BLOCO 4
#define TAM_CHAVE 2
#define DRIFT_PERM_INI 3
#define DRIFT_PERM_FIN 3
#define QTD_ROUNDS 1

// operacoes bit-a-bit
char valorBit(char valor, int posicao);
char setBit(char valor, int posicao);
char resetBit(char valor, int posicao);

// gerar lista de blocos
QList<QByteArray> breakIntoBlocks(QByteArray input);

// print lista de blocos
void printBlockList(QList<QByteArray> input);

// realizar/desfazer permutacao inicial
QList<QByteArray> doPermutation(QList<QByteArray> input);
QList<QByteArray> undoPermutation(QList<QByteArray> input);

//Realiza swap
QByteArray swapper(QByteArray inputBlock);

//Funcoes de encriptacao
QByteArray encrypt(const QByteArray& plainText);


// realizar/desfazer permutacao final

// expandir chave

// aplicar uma 'rodada'
void chiperRound(QByteArray& blockLeft, QByteArray& blockRight);

#endif // SIMPLECIPHER_H

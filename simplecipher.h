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

// realizar/desfazer permutacao
QList<QByteArray> doPermutation(QList<QByteArray> input);
QList<QByteArray> undoPermutation(QList<QByteArray> input);

//Funcoes de encriptacao
QByteArray encrypt(QByteArray input, QByteArray key);
QByteArray decrypt(QByteArray input, QByteArray key);
//QByteArray oldEncrypt(const QByteArray& plainText);

// expandir chave
QByteArray simpleResize(QByteArray input, int size);

// aplicar uma 'rodada'
QByteArray cipherRound(QByteArray block, QByteArray key);

#endif // SIMPLECIPHER_H

#ifndef SIMPLECIPHER_H
#define SIMPLECIPHER_H

#include <QList>
#include <QString>
#include <QByteArray>
#include <QDebug>

#include <thread>
#include <vector>
#include <future>

//funcoes para operacoes bit-a-bit
char valorBit(char valor, int posicao);
char setBit(char valor, int posicao);
char resetBit(char valor, int posicao);

//classe que executa o cipher
class SimpleCipher
{
public:
    //construtor
    SimpleCipher();

    //configuracoes
    void setBlockSize(int blockSize);
    void setRounds(int rounds);

    //funcoes de encriptacao
    QByteArray encrypt(QByteArray input, QByteArray key);
    QByteArray decrypt(QByteArray input, QByteArray key);

    //funcoes multithread
    QByteArray encryptThread(QByteArray input, QByteArray key);
    QByteArray encryptMultiThread(QByteArray input, QByteArray key);

    //funcao comparacao
    static double differencePercentage(QByteArray input1, QByteArray input2);

private:
    //gerar/juntar lista de blocos
    QList<QByteArray> breakIntoBlocks(QByteArray input);
    QByteArray joinBlocks(QList<QByteArray> input);

    //print lista de blocos
    void printBlock(QByteArray input);
    void printBlockList(QList<QByteArray> input);

    //realizar/desfazer permutacao
    QByteArray doBlockPermutation(QByteArray input);
    QByteArray undoBlockPermutation(QByteArray input);

    //realizar/desfazer permutacao em lista de blocos
    QList<QByteArray> doBlockListPermutation(QList<QByteArray> input);
    QList<QByteArray> undoBlockListPermutation(QList<QByteArray> input);

    //expandir chave
    QByteArray simpleResize(QByteArray input, int size);
    QByteArray prepareKey(QByteArray input);

    //aplicar uma 'rodada'
    QByteArray cipherRound(QByteArray block, QByteArray key);
    QByteArray decipherRound(QByteArray block, QByteArray key);

    //funcao F
    QByteArray functionF(QByteArray halfBlock, QByteArray key);

    //parametros
    int TAM_BLOCO;
    int TAM_CHAVE;
    int DRIFT_PERM_INI;
    int DRIFT_PERM_FIN;
    int QTD_ROUNDS;
};

#endif // SIMPLECIPHER_H

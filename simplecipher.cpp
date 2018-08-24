#include "simplecipher.h"

int valorBit(int valor, int posicao)
{
    return (valor & (1 << posicao)) >> posicao;
}

int setBit(int valor, int posicao)
{
    return valor | (1 << posicao);
}

int resetBit(int valor, int posicao)
{
    return valor & ~(1 << posicao);
}

QList<QByteArray> gerarListaBlocos(QByteArray input)
{
    QList<QByteArray> output;
    int i = 0;
    while(i < input.size())
    {
        QByteArray x;
        for(int j = 0; j < TAM_BLOCO; j++)
        {
            if((i+j) < input.size()) x.append(input[i+j]);
            else x.append((char)0);
        }
        output.append(x);
        i += TAM_BLOCO;
    }
    return output;
}

void printListaBlocos(QList<QByteArray> input)
{
    foreach(QByteArray x, input)
    {
        qDebug() << x << x.size();
    }
}

QList<QByteArray> realizarPermutacaoInicial(QList<QByteArray> input)
{
    QList<QByteArray> output;
    for(int i = 0; i < input.size(); i++) output.append(QByteArray(TAM_BLOCO, (char)0));
    foreach(QByteArray x, input)
    {

    }
    int drift = -1;
    char x = 0;
    for(int i = 0; i < input.size(); i++)
    {
        int oByte = i / 8;
        int oBit = i % 8;
        if(i % DRIFT_PERM_INI == 0) drift += 1;
        int dByte = (i + drift) / 8;
        int dBit = (i + drift) % 8;






    }
}

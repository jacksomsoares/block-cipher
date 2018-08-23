#ifndef MYCRYPT_H
#define MYCRYPT_H

#include <QList>
#include <QString>
#include <QByteArray>
#include <QDebug>

// lista de parametros
#define TAM_BLOCO 4
#define TAM_CHAVE 2

// gerar lista de blocos
QList<QByteArray> gerarListaBlocos(QByteArray input)
{
    QList<QByteArray> output;
    int i = 0;
    while(i < input.size())
    {
        QByteArray x;
        for(int j = 0; j < TAM_BLOCO; j++)
        {
            if(input.size() < (i+j)) x.append(input[i+j]);
            else x.append((char)0);
        }
        i += TAM_BLOCO;
    }
    return output;
}

// print lista de blocos
void printListaBlocos(QList<QByteArray> input)
{
    foreach(QByteArray x, input)
    {
        qDebug() << x << x.size();
    }
}

// realizar/desfazer permutacao inicial

// realizar/desfazer permutacao final

// expandir chave

// aplicar uma 'rodada' (

//

#endif // MYCRYPT_H

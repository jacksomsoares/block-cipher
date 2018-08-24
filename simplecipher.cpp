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
    for(int i = 0; i < input.size(); i++)
    {
        QString string;
        for(int j = 0; j < input[i].size(); j++)
        {
            for(int m = 7; m >= 0; m--)
            {
                string.append(QString::number(valorBit(input[i][j], m)));
            }
            string.append(" ");
        }
        qDebug().noquote() << string;
    }
}

QList<QByteArray> realizarPermutacaoInicial(QList<QByteArray> input)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
        QByteArray y(x.size(), (char)0);
        int drift = -1;
        int driftCounter = 0;
        int secondStepCounter = -1;
        for(int i = 0; i < x.size(); i++)
        {
            for(int m = 7; m >= 0; m--)
            {
                if(driftCounter % DRIFT_PERM_INI == 0) drift++;
                driftCounter++;
                int oByte = i;
                int oBit = m;
                int dByte = i + (((7 - m) + drift) / 8);
                int dBit = (m - drift) % 8;
                if(dBit < 0) dBit = 8 + dBit;
                if(dByte >= y.size())
                {
                    secondStepCounter += DRIFT_PERM_INI + 1;
                    dByte = secondStepCounter / 8;
                    dBit = 7 - (secondStepCounter % 8);
                    //qDebug() << dByte << dBit;
                }
                //qDebug() << drift << "|" << oByte << oBit << "|" << dByte << dBit;
                if(valorBit(x[oByte], oBit) != 0)
                {
                    y[dByte] = setBit(y[dByte], dBit);
                }
            }
        }
        output.append(y);
    }
    return output;
}

QList<QByteArray> desfazerPermutacaoInicial(QList<QByteArray> input)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
        QByteArray y(x.size(), (char)0);
        int drift = -1;
        int driftCounter = 0;
        int secondStepCounter = -1;
        for(int i = 0; i < x.size(); i++)
        {
            for(int m = 7; m >= 0; m--)
            {
                if(driftCounter % DRIFT_PERM_INI == 0) drift++;
                driftCounter++;
                int oByte = i;
                int oBit = m;
                int dByte = i + (((7 - m) + drift) / 8);
                int dBit = (m - drift) % 8;
                if(dBit < 0) dBit = 8 + dBit;
                if(dByte >= y.size())
                {
                    secondStepCounter += DRIFT_PERM_INI + 1;
                    dByte = secondStepCounter / 8;
                    dBit = 7 - (secondStepCounter % 8);
                    //qDebug() << dByte << dBit;
                }
                //qDebug() << drift << "|" << dByte << dBit << "|" << oByte << oBit;
                if(valorBit(x[dByte], dBit) != 0)
                {
                    y[oByte] = setBit(y[oByte], oBit);
                }
            }
        }
        output.append(y);
    }
    return output;
}

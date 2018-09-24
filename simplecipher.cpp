#include "simplecipher.h"

char valorBit(char valor, int posicao)
{
    return (valor & (1 << posicao)) >> posicao;
}

char setBit(char valor, int posicao)
{
    return valor | (1 << posicao);
}

char resetBit(char valor, int posicao)
{
    return valor & ~(1 << posicao);
}

QList<QByteArray> breakIntoBlocks(QByteArray input)
{
    QList<QByteArray> output;
    int i = 0;
    while(i < input.size())
    {
        QByteArray x;
        for(int j = 0; j < TAM_BLOCO; j++)
        {
            if((i+j) < input.size()) x.append(input[i+j]);
            else x.append(static_cast<char>(0));
        }
        output.append(x);
        i += TAM_BLOCO;
    }
    return output;
}

QByteArray joinBlocks(QList<QByteArray> input)
{
    QByteArray output;
    foreach(QByteArray x, input)
    {
        output.append(x);
    }
    return output;
}

void printBlock(QByteArray input)
{
    QString string;
    for(int j = 0; j < input.size(); j++)
    {
        for(int m = 7; m >= 0; m--)
        {
            string.append(QString::number(valorBit(input[j], m), 16));
        }
        string.append(" ");
    }
    qDebug().noquote() << string;
}

void printBlockList(QList<QByteArray> input)
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
    qDebug().noquote() << "";
}

QByteArray doBlockPermutation(QByteArray input, int drift)
{
    QByteArray y(input.size(), static_cast<char>(0));
    //int drift = -1;
    int driftCounter = 0;
    int secondStepCounter = -1;
    for(int i = 0; i < input.size(); i++)
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
            }
            if(valorBit(input[oByte], oBit) != 0)
            {
                y[dByte] = setBit(y[dByte], dBit);
                //qDebug().noquote() << oByte << "[" << oBit << "]  -> " << dByte << "[" << dBit << "]";
            }
        }
    }
    return y;
}

QByteArray undoBlockPermutation(QByteArray input, int drift)
{
    QByteArray y(input.size(), static_cast<char>(0));
    //int drift = -1;
    int driftCounter = 0;
    int secondStepCounter = -1;
    for(int i = 0; i < input.size(); i++)
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
            }
            if(valorBit(input[dByte], dBit) != 0)
            {
                y[oByte] = setBit(y[oByte], oBit);
                //qDebug().noquote() << oByte << "[" << oBit << "]  <- " << dByte << "[" << dBit << "]";
            }
        }
    }
    return y;
}

QList<QByteArray> doBlockListPermutation(QList<QByteArray> input, int drift)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
        output.append(doBlockPermutation(x, drift));
    }
    return output;
}

QList<QByteArray> undoBlockListPermutation(QList<QByteArray> input, int drift)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
       output.append(undoBlockPermutation(x, drift));
    }
    return output;
}


QByteArray simpleResize(QByteArray input, int size)
{
    // verificar se input.size() == 0?
    QByteArray output;
    int i = 0;
    while(output.size() < size) //colocar TAM_BLOCO?
    {
        output.append(input.at(i%input.size()));
    }
    return output;
}

QByteArray functionF(QByteArray right, QByteArray key)
{
    for (int index=0; index < right.size(); index++)
    {
        right[index] = right[index] ^ key[index];
    }
    right = doBlockPermutation(right);
    return right;
}

QByteArray functionG(QByteArray left, QByteArray key)
{
    for (int index=0; index<left.size(); index++)
    {
        left[index] = left[index] ^ key[index];
    }
    left = doBlockPermutation(left);
    return left;
}

QByteArray cipherRound(QByteArray block, QByteArray key)
{
    //expandir/cortar chave
    QByteArray resizedKey = simpleResize(key, TAM_BLOCO);

    //separar bloco em duas partes
    QByteArray left;
    QByteArray right;
    left = block.left(block.size()/2);
    right = block.right(block.size()/2);

    QByteArray temp = functionF(right, key);

    //operacoes bit a bit
    for(int pos = 0; pos < TAM_BLOCO/2; pos++)
    {
        //left XOR temp
        left[pos] = left[pos] ^ temp[pos];
    }

    //juncao invertida
    QByteArray output;
    output.append(right);
    output.append(left);

    //return
    return output;

}

QByteArray decipherRound(QByteArray block, QByteArray key)
{
    //expandir/cortar chave
    QByteArray resizedKey = simpleResize(key, TAM_BLOCO);

    //separar bloco em duas partes
    QByteArray left;
    QByteArray right;
    left = block.left(block.size()/2);
    right = block.right(block.size()/2);

    QByteArray temp = functionG(left, key);

    //operacoes bit a bit
    for(int pos = 0; pos < TAM_BLOCO/2; pos++)
    {
        //right XOR left
        right[pos] = right[pos] ^ temp[pos];
    }

    //juncao invertida
    QByteArray output;
    output.append(right);
    output.append(left);

    //return
    return output;

}

QByteArray encrypt(QByteArray input, QByteArray key)
{
    //quebrar em blocos
    QList<QByteArray> blockList = breakIntoBlocks(input);

    //realizar permutacao
    blockList = doBlockListPermutation(blockList);

    //aplicar cipher
    QList<QByteArray> cipherBlockList;
    foreach(QByteArray block, blockList)
    {
        //numero de rounds
        for (int round = 0; round < QTD_ROUNDS; round++)
        {
            block = cipherRound(block, key);
        }
        cipherBlockList.append(block);
    }

    //desfazer permutacao
    cipherBlockList = undoBlockListPermutation(cipherBlockList);

    //juntar mensagem e returnar
    return joinBlocks(cipherBlockList);
}

QByteArray decrypt(QByteArray input, QByteArray key)
{
    //quebrar em blocos
    QList<QByteArray> blockList = breakIntoBlocks(input);

    //realizar permutacao
    blockList = doBlockListPermutation(blockList);

    //aplicar cipher
    QList<QByteArray> cipherBlockList;
    foreach(QByteArray block, blockList)
    {
        //numero de rounds
        for (int round = 0; round < QTD_ROUNDS; round++)
        {
            block = decipherRound(block, key);
        }
        cipherBlockList.append(block);
    }

    //desfazer permutacao
    cipherBlockList = undoBlockListPermutation(cipherBlockList);

    //juntar mensagem e returnar
    return joinBlocks(cipherBlockList);
}

















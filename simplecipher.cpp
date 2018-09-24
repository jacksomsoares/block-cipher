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
        output.append(doBlockPermutation(x));
    }
    return output;
}

QList<QByteArray> undoBlockListPermutation(QList<QByteArray> input, int drift)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
       output.append(undoBlockPermutation(x));
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

QByteArray cipherRound(QByteArray block, QByteArray key)
{
    //expandir/cortar chave
    QByteArray resizedKey = simpleResize(key, TAM_BLOCO);

    //separar bloco em duas partes
    QByteArray left;
    QByteArray right;
    left = block.left(block.size()/2);
    right = block.right(block.size()/2);

    //operacoes bit a bit
    for(int pos = 0; pos < TAM_BLOCO/2; pos++)
    {
        //right XOR key
        char f = right[pos] ^ key[pos];

        //left XOR temp
        left[pos] = left[pos] ^ f;
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

    //operacoes bit a bit
    for(int pos = 0; pos < TAM_BLOCO/2; pos++)
    {
        //left XOR key
        char temp = left[pos] ^ key[pos];

        //right XOR left
        right[pos] = right[pos] ^ temp;
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

    /*
    printBlockList(blockList);
    //realizar permutacao
    blockList = doBlockListPermutation(blockList);
    printBlockList(blockList);
    */

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
    //cipherBlockList = undoPermutation(cipherBlockList);

    //juntar mensagem e returnar
    return joinBlocks(cipherBlockList);
}

QByteArray decrypt(QByteArray input, QByteArray key)
{
    //quebrar em blocos
    QList<QByteArray> blockList = breakIntoBlocks(input);

    //realizar permutacao
    //blockList = doPermutation(blockList);

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
    //cipherBlockList = undoBlockListPermutation(cipherBlockList);

    //juntar mensagem e returnar
    return joinBlocks(cipherBlockList);
}

/*
QByteArray oldEncrypt(const QByteArray& plainText) //PERGUNTA: O plaintext não deveria ser em QString?
{
    //Do Initial permutation in whole Text
    QByteArray permutedText(plainText);
    if ((permutedText.size()%TAM_BLOCO) > 0){
        permutedText.append(TAM_BLOCO - (permutedText.size()%TAM_BLOCO),' '); //Modify the size of the message be compatible with the block size
    }

    //jeito do Filipe - TESTAR
    QList<QByteArray> blockList = breakIntoBlocks(permutedText);
    QList<QByteArray> resultBlockList;
    foreach(QByteArray block, blockList)
    {
        QByteArray blockLeft;
        QByteArray blockRight;
        blockLeft = block.left(block.size()/2);
        blockRight = block.right(block.size()/2);

        for (int round=0; round<QTD_ROUNDS; round++) //Rounds Loop
        {
            chiperRound(blockLeft, blockRight);
        }
        QByteArray resultBlock;
        resultBlock.append(blockLeft);
        resultBlock.append(blockRight);
        resultBlockList.append(resultBlock);
    }

    //jeito do Jacksom
    QByteArray block;
    QByteArray blockLeft;
    QByteArray blockRight;
    QByteArray encryptedText;
    for (int index=0; index<permutedText.size(); index+=TAM_BLOCO) //Block Loop
    {
        block = permutedText.mid(index, TAM_BLOCO);
        blockLeft = block.left(block.size()/2);
        blockRight = block.right(block.size()/2);

        for (int round=0; round<QTD_ROUNDS; round++) //Rounds Loop
        {
            chiperRound(blockLeft, blockRight);
        }
        encryptedText.append(blockLeft);
        encryptedText.append(blockRight);
    }

    //Do final permutation before return
    return encryptedText;
    return QByteArray();
}
*/

















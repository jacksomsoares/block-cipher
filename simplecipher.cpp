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
}

QList<QByteArray> doPermutation(QList<QByteArray> input)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
        QByteArray y(x.size(), static_cast<char>(0));
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
                }
                if(valorBit(x[oByte], oBit) != 0)
                {
                    y[dByte] = setBit(y[dByte], dBit);
                    //qDebug().noquote() << oByte << "[" << oBit << "]  -> " << dByte << "[" << dBit << "]";
                }
            }
        }
        output.append(y);
    }
    return output;
}

QList<QByteArray> undoPermutation(QList<QByteArray> input)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
        QByteArray y(x.size(), static_cast<char>(0));
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
                }
                if(valorBit(x[dByte], dBit) != 0)
                {
                    y[oByte] = setBit(y[oByte], oBit);
                    //qDebug().noquote() << oByte << "[" << oBit << "]  <- " << dByte << "[" << dBit << "]";
                }
            }
        }
        output.append(y);
    }
    return output;
}

QByteArray swapper(QByteArray inputBlock)
{
    QByteArray output;
    output.append(inputBlock.right(TAM_BLOCO/2));
    output.append(inputBlock.left(TAM_BLOCO/2));

    return output;
}

void chiperRound(QByteArray& blockLeft, QByteArray& blockRight)
{
    //fazer função para expandir a chave para o tamanho do bloco

    //Apply function on the right side. Use ref (&) to avoid instanciation new objects
    for(int index=0; index<TAM_BLOCO/2; index++)
    {
        //desenvolver funções para:
        //1: aplicar XOR entre o bloco e a chave
        //2: aplicar XOR entre os dois lados do bloco
        //3: outras etapas opcionais (?)
        //blockLeft[index] = blockLeft[index] ^ blockRight[index];
    }

    //swapp sides
    QByteArray swapAux = blockLeft;
    blockLeft = blockRight;
    blockRight = swapAux;

}

QByteArray encrypt(const QByteArray& plainText) //PERGUNTA: O plaintext não deveria ser em QString?
{
    //Do Initial permutation in whole Text
    QByteArray permutedText(plainText);
    if ((permutedText.size()%TAM_BLOCO) > 0){
        permutedText.append(TAM_BLOCO - (permutedText.size()%TAM_BLOCO),' '); //Modify the size of the message be compatible with the block size
    }

    //jeito do Filipe - TESTAR
    /*
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
        QByteArray resultBlock = .append(blockLeft)
        resultBlock.append(blockLeft);
        resultBlock.append(blockRight);
        resultBlockList.append(resultBlock);
    }
    */

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
}

























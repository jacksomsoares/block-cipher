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

SimpleCipher::SimpleCipher()
{
    TAM_BLOCO = 8;
    TAM_CHAVE = 4;
    DRIFT_PERM_INI = 3;
    DRIFT_PERM_FIN = 3;
    QTD_ROUNDS = 10;
}

int SimpleCipher::getBlockSize()
{
    return TAM_BLOCO;
}

int SimpleCipher::getRounds()
{
    return QTD_ROUNDS;
}

void SimpleCipher::setBlockSize(int blockSize)
{
    TAM_BLOCO = blockSize;
}

void SimpleCipher::setRounds(int rounds)
{
    QTD_ROUNDS = rounds;
}

QList<QByteArray> SimpleCipher::breakIntoBlocks(QByteArray input)
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

QByteArray SimpleCipher::joinBlocks(QList<QByteArray> input)
{
    QByteArray output;
    foreach(QByteArray x, input)
    {
        output.append(x);
    }
    return output;
}

void SimpleCipher::printBlock(QByteArray input)
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

void SimpleCipher::printBlockList(QList<QByteArray> input)
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

QByteArray SimpleCipher::doBlockPermutation(QByteArray input)
{
    QByteArray y(input.size(), static_cast<char>(0));
    int drift = -1;
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
            }
        }
    }
    return y;
}

QByteArray SimpleCipher::undoBlockPermutation(QByteArray input)
{
    QByteArray y(input.size(), static_cast<char>(0));
    int drift = -1;
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
            }
        }
    }
    return y;
}

QList<QByteArray> SimpleCipher::doBlockListPermutation(QList<QByteArray> input)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
        output.append(doBlockPermutation(x));
    }
    return output;
}

QList<QByteArray> SimpleCipher::undoBlockListPermutation(QList<QByteArray> input)
{
    QList<QByteArray> output;
    foreach(QByteArray x, input)
    {
       output.append(undoBlockPermutation(x));
    }
    return output;
}


QByteArray SimpleCipher::simpleResize(QByteArray input, int size)
{
    // verificar se input.size() == 0?
    QByteArray output;
    int i = 0;
    while(output.size() < size) //colocar TAM_BLOCO?
    {
        output.append(input.at(i%input.size()));
        i++;
    }
    return output;
}

QByteArray SimpleCipher::prepareKey(QByteArray input)
{
    QByteArray output(TAM_BLOCO/2, '\0');

    //expandir chave (se necessario)
    if(input.size() == 0)
        return output;
    else if(input.size() < TAM_BLOCO/2)
        input = simpleResize(input, TAM_BLOCO/2);

    //diminuir tamanho da chave aplicando operacoes XOR
    for(int i = 0; i < input.size(); i++)
    {
        int outputPos = i%output.size();
        output[outputPos] = output[outputPos] ^ input[i];
    }
    return output;
}

QByteArray SimpleCipher::functionF(QByteArray halfBlock, QByteArray key)
{
    for (int index=0; index < halfBlock.size(); index++)
    {
        halfBlock[index] = halfBlock[index] ^ key[index];
    }
    halfBlock = doBlockPermutation(halfBlock);
    return halfBlock;
}

QByteArray SimpleCipher::cipherRound(QByteArray block, QByteArray key)
{
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

QByteArray SimpleCipher::decipherRound(QByteArray block, QByteArray key)
{
    //separar bloco em duas partes
    QByteArray left;
    QByteArray right;
    left = block.left(block.size()/2);
    right = block.right(block.size()/2);

    QByteArray temp = functionF(left, key);

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

QByteArray SimpleCipher::encrypt(QByteArray input, QByteArray key)
{
    //preparar chave
    QByteArray resizedKey = prepareKey(key);

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
            block = cipherRound(block, resizedKey);
        }
        cipherBlockList.append(block);
    }

    //desfazer permutacao
    cipherBlockList = undoBlockListPermutation(cipherBlockList);

    //juntar mensagem e returnar
    return joinBlocks(cipherBlockList);
}

QByteArray SimpleCipher::encryptThread(QByteArray input, QByteArray key)
{
    //realizar permutacao
    input = doBlockPermutation(input);

    //numero de rounds
    for (int round = 0; round < QTD_ROUNDS; round++)
    {
        input = cipherRound(input, key);
    }

    //desfazer permutacao
    input = undoBlockPermutation(input);

    return input;
}

QByteArray SimpleCipher::encryptMultiThread(QByteArray input, QByteArray key)
{
    //preparar chave
    QByteArray resizedKey = prepareKey(key);

    //quebrar em blocos
    QList<QByteArray> blockList = breakIntoBlocks(input);

    //threads
    std::vector<std::future<QByteArray>> asyncVector;
    foreach(QByteArray block, blockList)
    {
        asyncVector.push_back(std::async(std::launch::async, [](SimpleCipher* cipher, QByteArray input, QByteArray key)
        {
            return cipher->encryptThread(input, key);
        }, this, block, resizedKey));
    }

    //join das threads
    QList<QByteArray> cipherBlockList;
    for(unsigned int i = 0; i < asyncVector.size(); i++)
    {
        cipherBlockList.append(asyncVector.at(i).get());
    }

    //juntar mensagem e retornar
    return joinBlocks(cipherBlockList);
}

QByteArray SimpleCipher::decrypt(QByteArray input, QByteArray key)
{
    //preparar chave
    QByteArray resizedKey = prepareKey(key);

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
            block = decipherRound(block, resizedKey);
        }
        cipherBlockList.append(block);
    }

    //desfazer permutacao
    cipherBlockList = undoBlockListPermutation(cipherBlockList);

    //juntar mensagem e returnar
    return joinBlocks(cipherBlockList);
}

double SimpleCipher::differencePercentage(QByteArray input1, QByteArray input2)
{
    int length = input1.size();
    if(input2.size() < input1.size()) length = input2.size();
    int bitsChanged = 0;

    for(int i = 0; i < length; i++)
    {
        int byte = i/8;
        int bit = i%8;
        if(valorBit(input1[byte], bit) != valorBit(input2[byte], bit)) bitsChanged++;
    }
    return (double)bitsChanged / (double)length;
}















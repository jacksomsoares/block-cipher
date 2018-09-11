#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textOriginal = ui->ptext_msg;
    textCriptogrado = ui->ptext_msg_crypt;

    QList<QByteArray> x;
    x.append(QByteArray(TAM_BLOCO, (char)255));
    printListaBlocos(x);
    printListaBlocos(realizarPermutacaoInicial(x));
    printListaBlocos(desfazerPermutacaoInicial(x));


    //Testes swapper
    QByteArray bArray("ana rayar");
    bArray.append(TAM_BLOCO - (bArray.size()%TAM_BLOCO),'-'); //This will add a quantity of spaces to the end of the string to be compatible
    QByteArray bArrayReverse;
    QByteArray bArrayTest;

    for (int index=0; index<bArray.size()-1; index+=TAM_BLOCO) {
        QByteArray b2 = bArray.mid(index, TAM_BLOCO);
        bArrayReverse.append(swapper(b2));
    }

    qDebug() << bArray;
    qDebug() << bArrayReverse;

    for (int index=0; index<bArrayReverse.size()-1; index+=TAM_BLOCO) {
        QByteArray b2 = bArrayReverse.mid(index, TAM_BLOCO);
        bArrayTest.append(swapper(b2));
    }

    qDebug() << bArrayTest;
    //Fim testes swapper

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_crypt_pressed()
{
    /*
    pegarChave();
    QString original = textOriginal->toPlainText();
    QList<QByteArray> blocos = dividirEmBlocos(original);
    QString final;
    foreach(QByteArray a, blocos)
    {
        final.append(QString(a.toHex()));
    }
    textCriptogrado->clear();
    textCriptogrado->appendPlainText(final);
    */
}

void MainWindow::on_button_decrypt_pressed()
{
    /*
    QString final = textCriptogrado->toPlainText();
    QString original = QString(QByteArray::fromHex(final.toLatin1()));
    textOriginal->clear();
    textOriginal->appendPlainText(original);
    */
}

QByteArray MainWindow::pegarChave()
{
    char chave = (char) ui->spin_chave->value();
    QByteArray output;
    output.append(chave);
    output.append(~chave);
    output.append(~chave);
    output.append(chave);
    /*
    int chaveEst = 0;
    chaveEst |= chave << 24;
    chaveEst |= ~chave << 16;
    chaveEst |= ~chave << 8;
    chaveEst |= chave;
    */
    qDebug() << output;
    return output;
}


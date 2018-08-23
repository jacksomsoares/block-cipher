#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textOriginal = ui->ptext_msg;
    textCriptogrado = ui->ptext_msg_crypt;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_crypt_pressed()
{
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
}

void MainWindow::on_button_decrypt_pressed()
{
    QString final = textCriptogrado->toPlainText();
    QString original = QString(QByteArray::fromHex(final.toLatin1()));
    textOriginal->clear();
    textOriginal->appendPlainText(original);
}

QList<QByteArray> MainWindow::dividirEmBlocos(QString str)
{
    QList<QByteArray> output;
    int i = 0;
    int step = 4;
    while(i < str.size())
    {
        QByteArray a = str.mid(i, step).toLatin1();
        while(a.size() < step) a.append((char)0);
        qDebug() << a << a.size();
        output.append(a);
        i += step;
    }
    return output;
}

QList<QByteArray> MainWindow::realizarPermutacao(QList<QByteArray> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        QByteArray a = list[i];
        QByteArray b;
        b.append(a[1]).append(a[2]).append(a[3]).append(a[0]);
        list[i] = b;
    }
    return list;
}

QList<QByteArray> MainWindow::desfazerPermutacao(QList<QByteArray> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        QByteArray a = list[i];
        QByteArray b;
        b.append(a[3]).append(a[0]).append(a[1]).append(a[2]);
        list[i] = b;
    }
    return list;
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


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->setStyle("fusion");

    //teste Encrypt
    QByteArray key("affs");
    QByteArray plainText(QString("The quick brown fox jumps over the lazy dog").toLatin1());
    qDebug() << plainText;
    //printBlock(plainText);
    QByteArray encryptedText = encrypt(plainText, key);
    //printBlock(encryptedText);
    qDebug() << encryptedText;
    QByteArray decryptedText = decrypt(encryptedText, key);
    //printBlock(decryptedText);
    qDebug() << decryptedText;
}

MainWindow::~MainWindow()
{
    delete ui;
}

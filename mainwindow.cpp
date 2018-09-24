#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), cipher()
{
    ui->setupUi(this);

    qApp->setStyle("fusion");
    /*
    //teste Encrypt
    QByteArray key("affs");
    QByteArray plainText(QString("The quick brown fox jumps over the lazy dog").toLatin1());
    qDebug() << plainText;
    //printBlock(plainText);
    QByteArray encryptedText = cipher.encrypt(plainText, key);
    //printBlock(encryptedText);
    qDebug() << encryptedText;
    QByteArray decryptedText = cipher.decrypt(encryptedText, key);
    //printBlock(decryptedText);
    qDebug() << decryptedText;
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printLog(QString log)
{
    ui->log_plaintext->appendPlainText(log);
}

void MainWindow::on_criptografar_button_pressed()
{
    printLog("criptografar_button pressionado!");
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();
    QByteArray mensagem_criptografada = cipher.encrypt(mensagem, chave);
    QString resultado_hexadecimal = mensagem_criptografada.toHex();
    ui->encriptado_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
    ui->mensagem_encriptada_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
}

void MainWindow::on_descriptografar_button_pressed()
{
    printLog("descriptografar_button pressionado!");
    QByteArray mensagem_criptografada = ui->mensagem_encriptada_hexadecimal_plaintext->toPlainText().toLatin1();
    mensagem_criptografada = QByteArray::fromHex(mensagem_criptografada);
    QByteArray chave = ui->chave_line->text().toLatin1();
    QByteArray mensagem_descriptograda = cipher.decrypt(mensagem_criptografada, chave);
    ui->mensagem_desencriptada_plaintext->setPlainText(mensagem_descriptograda);
}

void MainWindow::on_aplicar_config_button_pressed()
{
    cipher.setBlockSize(ui->block_size_spin->value());
    cipher.setRounds(ui->rounds_spin->value());
    printLog("novas configurações aplicadas!");
}

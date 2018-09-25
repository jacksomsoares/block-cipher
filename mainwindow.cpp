#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), cipher()
{
    ui->setupUi(this);
    qApp->setStyle("fusion");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printLog(QString log)
{
    ui->log_plaintext->appendPlainText(log);
    //ui->log_plaintext->verticalScrollBar()->setValue(ui->log_plaintext->verticalScrollBar()->maximum());
}

void MainWindow::on_criptografar_button_pressed()
{
    printLog("criptografar_button pressionado");
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();
    QElapsedTimer timer;
    timer.start();
    QByteArray mensagem_criptografada = cipher.encrypt(mensagem, chave);
    qint64 elapsedTime = timer.nsecsElapsed();
    double msTime = elapsedTime / 1000.0 / 1000.0;
    printLog("a criptografia demorou " + QString::number(msTime, 'f', 3) + " ms");
    QString resultado_hexadecimal = mensagem_criptografada.toHex();
    ui->encriptado_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
    ui->mensagem_encriptada_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
}

void MainWindow::on_criptografar_multithread_button_pressed()
{
    printLog("criptografar_multithread_button pressionado");
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();
    QElapsedTimer timer;
    timer.start();
    QByteArray mensagem_criptografada = cipher.encryptMultiThread(mensagem, chave);
    qint64 elapsedTime = timer.nsecsElapsed();
    double msTime = elapsedTime / 1000.0 / 1000.0;
    printLog("a criptografia demorou " + QString::number(msTime, 'f', 3) + " ms");
    QString resultado_hexadecimal = mensagem_criptografada.toHex();
    ui->encriptado_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
    ui->mensagem_encriptada_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
}

void MainWindow::on_descriptografar_button_pressed()
{
    printLog("descriptografar_button pressionado");
    QByteArray mensagem_criptografada = ui->mensagem_encriptada_hexadecimal_plaintext->toPlainText().toLatin1();
    mensagem_criptografada = QByteArray::fromHex(mensagem_criptografada);
    QByteArray chave = ui->chave_line->text().toLatin1();
    QElapsedTimer timer;
    timer.start();
    QByteArray mensagem_descriptograda = cipher.decrypt(mensagem_criptografada, chave);
    qint64 elapsedTime = timer.nsecsElapsed();
    double msTime = elapsedTime / 1000.0 / 1000.0;
    printLog("a descriptografia demorou " + QString::number(msTime, 'f', 3) + " ms");
    ui->mensagem_desencriptada_plaintext->setPlainText(mensagem_descriptograda);
}

void MainWindow::on_aplicar_config_button_pressed()
{
    cipher.setBlockSize(ui->block_size_spin->value());
    cipher.setRounds(ui->rounds_spin->value());
    ui->encriptado_hexadecimal_plaintext->clear();
    ui->mensagem_encriptada_hexadecimal_plaintext->clear();
    printLog("novas configurações aplicadas!");
}



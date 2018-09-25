#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), cipher()
{
    ui->setupUi(this);
    qApp->setStyle("fusion");
    ui->chave_hexadecimal_line->setText(ui->chave_line->text().toLatin1().toHex());

    //qDebug() << SimpleCipher::differencePercentage(QByteArray(8, (char)15), QByteArray(8, (char)255))*100;
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

    //mensagem e chave
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();

    //timer
    QElapsedTimer timer;
    timer.start();

    //encrypt
    QByteArray mensagem_criptografada = cipher.encrypt(mensagem, chave);

    //timer
    qint64 elapsedTime = timer.nsecsElapsed();
    double msTime = elapsedTime / 1000.0 / 1000.0;
    printLog("a criptografia demorou " + QString::number(msTime, 'f', 3) + " ms");

    //escrever resultado
    QString resultado_hexadecimal = mensagem_criptografada.toHex();
    ui->encriptado_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
    ui->mensagem_encriptada_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);

    //diferenca
    double dif = SimpleCipher::differencePercentage(mensagem, mensagem_criptografada) * 100;
    printLog("força da criptografia: " + QString::number(dif, 'g', 3) + "%");

    //new line
    printLog("");
}

void MainWindow::on_criptografar_multithread_button_pressed()
{
    printLog("criptografar_multithread_button pressionado");

    //mensagem e chave
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();

    //timer
    QElapsedTimer timer;
    timer.start();

    //encrypt
    QByteArray mensagem_criptografada = cipher.encryptMultiThread(mensagem, chave);

    //timer
    qint64 elapsedTime = timer.nsecsElapsed();
    double msTime = elapsedTime / 1000.0 / 1000.0;
    printLog("a criptografia demorou " + QString::number(msTime, 'f', 3) + " ms");

    //escrever resultado
    QString resultado_hexadecimal = mensagem_criptografada.toHex();
    ui->encriptado_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);
    ui->mensagem_encriptada_hexadecimal_plaintext->setPlainText(resultado_hexadecimal);

    //diferenca
    double dif = SimpleCipher::differencePercentage(mensagem, mensagem_criptografada) * 100;
    printLog("força da criptografia: " + QString::number(dif, 'g', 3) + "%");

    //new line
    printLog("");
}

void MainWindow::on_descriptografar_button_pressed()
{
    printLog("descriptografar_button pressionado");

    //mensagem e chave
    QByteArray mensagem_criptografada = ui->mensagem_encriptada_hexadecimal_plaintext->toPlainText().toLatin1();
    mensagem_criptografada = QByteArray::fromHex(mensagem_criptografada);
    QByteArray chave = ui->chave_line->text().toLatin1();

    //timer
    QElapsedTimer timer;
    timer.start();

    //decrypt
    QByteArray mensagem_descriptograda = cipher.decrypt(mensagem_criptografada, chave);
    qint64 elapsedTime = timer.nsecsElapsed();
    double msTime = elapsedTime / 1000.0 / 1000.0;

    //escrever resultado
    printLog("a descriptografia demorou " + QString::number(msTime, 'f', 3) + " ms");
    ui->mensagem_desencriptada_plaintext->setPlainText(mensagem_descriptograda);

    //new line
    printLog("");
}

void MainWindow::on_aplicar_config_button_pressed()
{
    cipher.setBlockSize(ui->block_size_spin->value());
    cipher.setRounds(ui->rounds_spin->value());
    ui->encriptado_hexadecimal_plaintext->clear();
    ui->mensagem_encriptada_hexadecimal_plaintext->clear();
    printLog("novas configurações aplicadas!\n");
}



void MainWindow::on_chave_line_textEdited(const QString &arg1)
{
    ui->chave_hexadecimal_line->setText(arg1.toLatin1().toHex());
}

void MainWindow::on_chave_hexadecimal_line_textEdited(const QString &arg1)
{
    ui->chave_line->setText(QByteArray::fromHex(arg1.toLatin1()));
}

void MainWindow::on_block_size_spin_valueChanged(int arg1)
{
    ui->key_size_spin->setValue(arg1/2);
}

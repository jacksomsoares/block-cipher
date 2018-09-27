#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), cipher()
{
    ui->setupUi(this);
    qApp->setStyle("fusion");
    ui->chave_hexadecimal_line->setText(ui->chave_line->text().toLatin1().toHex());

    ui->block_size_spin->setValue(cipher.getBlockSize());
    ui->rounds_spin->setValue(cipher.getRounds());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printLog(QString log)
{
    ui->log_plaintext->appendPlainText(log);
}

void MainWindow::calculateMaxMinEncriptPower(QVector<double> x, QVector<double> y)
{
    double max = y[2];
    double min = y[2];
    double sum = 0;

    double idealPrecision = 50.0;
    double distance = std::abs(y[2] - idealPrecision);

    double maxX = x[2];
    double minX = x[2];
    double idealX = x[2];
    double idealY = y[2];

    for(int i = 3; i<y.size(); i++)
    {
        double iDistance = std::abs(y[i] - idealPrecision);
        if (iDistance < distance) {
            idealX = x[i];
            idealY = y[i];
            distance = iDistance;
        }

        if (y[i] > max){
            max = y[i];
            maxX = x[i];
        }
        if (y[i] < min){
            min = y[i];
            minX = x[i];
        }

        sum += y[i];
    }

    printLog("Valor Máximo " + QString::number(max) + "% Eixo X: " + QString::number(maxX));
    printLog("Valor Minimo " + QString::number(min) + "% Eixo X: " + QString::number(minX));
    printLog("Média " + QString::number(sum/(y.size() - 3)) + "%");
    printLog("Tamanho ideal: " + QString::number(idealX) + " Com precisão de: " + QString::number(idealY));
    printLog("");
}

void MainWindow::on_criptografar_button_pressed()
{
    printLog("criptografar_button pressionado");

    //mensagem e chave
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();

    //print chave
    printLog("chave utilizada: " + cipher.prepareKey(chave).toHex());

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
    cipher.setBlockSize(arg1);
    ui->encriptado_hexadecimal_plaintext->clear();
    ui->mensagem_encriptada_hexadecimal_plaintext->clear();
}

void MainWindow::on_rounds_spin_valueChanged(int arg1)
{
    cipher.setRounds(arg1);
    ui->encriptado_hexadecimal_plaintext->clear();
    ui->mensagem_encriptada_hexadecimal_plaintext->clear();
}

void MainWindow::on_plot_button_pressed()
{
    printLog("plot_button pressionado");

    //limpar grafico anterior
    QCustomPlot* plot = ui->custom_plot_widget;
    plot->clearPlottables();

    //dados
    QVector<double> eixoY(100);
    QVector<double> eixoYMultiThread(100);
    QVector<double> eixoX(100);
    double maiorValorY = 0;

    //dados do eixo x
    for(int i = 0; i < 100; i++)
    {
        eixoX[i] = i+1;
    }

    //mensagem e chave
    QByteArray mensagem = ui->mensagem_plaintext->toPlainText().toLatin1();
    QByteArray chave = ui->chave_line->text().toLatin1();

    //verificar opcoes escolhidas
    if(ui->eixo_x_block_radio->isChecked())
    {
        for(int i = 0; i < 100; i++)
        {
            if((int)eixoX[i] == 1)
            {
                eixoY[i] = 0.0;
                eixoYMultiThread[i] = 0.0;
            }
            else if((int)eixoX[i] % 2)
            {
                eixoY[i] = eixoY[i-1];
                eixoYMultiThread[i] = eixoYMultiThread[i-1];
            }
            else
            {
                //config cipher
                cipher.setBlockSize((int)eixoX[i]);

                //timer
                QElapsedTimer timer;
                timer.start();

                //encrypt
                QByteArray mensagem_criptografada = cipher.encrypt(mensagem, chave);

                //timer
                qint64 elapsedTime = timer.nsecsElapsed();
                double msTime = elapsedTime / 1000.0 / 1000.0;

                //timer2
                QElapsedTimer timer2;
                timer2.start();

                //encrypt multithread
                cipher.encryptMultiThread(mensagem, chave);

                //timer2
                qint64 elapsedTimeThread = timer2.nsecsElapsed();
                timer2.invalidate();
                double msTimeThread = elapsedTimeThread / 1000.0 / 1000.0;

                //armazenando dados
                if(ui->eixo_y_tempo_radio->isChecked())
                {
                    eixoY[i] = msTime;
                    eixoYMultiThread[i] = msTimeThread;
                    if(msTime > maiorValorY)
                        maiorValorY = msTime;
                    if(msTimeThread > maiorValorY)
                        maiorValorY = msTimeThread;
                }
                else if(ui->eixo_y_forca_radio->isChecked())
                {
                    eixoY[i] = SimpleCipher::differencePercentage(mensagem, mensagem_criptografada) * 100;
                }
            }
        }

    }
    else if(ui->eixo_x_round_radio->isChecked())
    {
        for(int i = 0; i < 100; i++)
        {
            //config cipher
            cipher.setRounds((int)eixoX[i]);

            //timer
            QElapsedTimer timer;
            timer.start();

            //encrypt
            QByteArray mensagem_criptografada = cipher.encrypt(mensagem, chave);

            //timer
            qint64 elapsedTime = timer.nsecsElapsed();
            timer.invalidate();
            double msTime = elapsedTime / 1000.0 / 1000.0;

            //timer2
            QElapsedTimer timer2;
            timer2.start();

            //encrypt multithread
            cipher.encryptMultiThread(mensagem, chave);

            //timer2
            qint64 elapsedTimeThread = timer2.nsecsElapsed();
            timer2.invalidate();
            double msTimeThread = elapsedTimeThread / 1000.0 / 1000.0;

            //armazenando dados
            if(ui->eixo_y_tempo_radio->isChecked())
            {
                eixoY[i] = msTime;
                eixoYMultiThread[i] = msTimeThread;
                if(msTime > maiorValorY)
                    maiorValorY = msTime;
                if(msTimeThread > maiorValorY)
                    maiorValorY = msTimeThread;
            }
            else if(ui->eixo_y_forca_radio->isChecked())
            {
                eixoY[i] = SimpleCipher::differencePercentage(mensagem, mensagem_criptografada) * 100;
            }
        }
    }

    calculateMaxMinEncriptPower(eixoX, eixoY);

    //colocar dados no grafico
    plot->addGraph();
    plot->graph(0)->setData(eixoX, eixoY);
    plot->xAxis->setRange(2, 100);
    if(ui->eixo_y_forca_radio->isChecked()) plot->yAxis->setRange(2, 100);
    else plot->yAxis->setRange(0, maiorValorY);
    plot->graph(0)->setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    if(ui->eixo_y_tempo_radio->isChecked())
    {
        plot->addGraph();
        plot->graph(1)->setData(eixoX, eixoYMultiThread);
        plot->yAxis->setRange(0, maiorValorY);
        plot->graph(1)->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    }
    plot->replot();

    //colocar parametros de volta
    cipher.setBlockSize(ui->block_size_spin->value());
    cipher.setRounds(ui->rounds_spin->value());
}

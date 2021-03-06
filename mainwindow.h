#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPlainTextEdit>
#include <QByteArray>
#include <QSpinBox>
#include <QList>
#include <QDebug>
#include <QApplication>
#include <QTime>
#include <QElapsedTimer>
#include <QScrollBar>
#include <cmath>

#include "simplecipher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_criptografar_button_pressed();
    void printLog(QString log);
    void calculateMaxMinEncriptPower(QVector<double> y, QVector<double> x);
    void on_descriptografar_button_pressed();
    void on_criptografar_multithread_button_pressed();
    void on_chave_line_textEdited(const QString &arg1);
    void on_chave_hexadecimal_line_textEdited(const QString &arg1);
    void on_block_size_spin_valueChanged(int arg1);
    void on_plot_button_pressed();

    void on_rounds_spin_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    SimpleCipher cipher;
};

#endif // MAINWINDOW_H

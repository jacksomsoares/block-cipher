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

    void on_descriptografar_button_pressed();

    void on_aplicar_config_button_pressed();

    void on_criptografar_multithread_button_pressed();

private:
    Ui::MainWindow *ui;
    SimpleCipher cipher;
};

#endif // MAINWINDOW_H

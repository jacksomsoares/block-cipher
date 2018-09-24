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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

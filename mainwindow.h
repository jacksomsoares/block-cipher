#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPlainTextEdit>
#include <QByteArray>
#include <QSpinBox>
#include <QList>


#include <QDebug>

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
    void on_button_decrypt_pressed();
    void on_button_crypt_pressed();

private:
    Ui::MainWindow *ui;

    QPlainTextEdit* textOriginal;
    QPlainTextEdit* textCriptogrado;

    QList<QByteArray> realizarPermutacao(QList<QByteArray> list);
    QList<QByteArray> desfazerPermutacao(QList<QByteArray> list);
    QList<QByteArray> dividirEmBlocos(QString str);
    QByteArray pegarChave();
};

#endif // MAINWINDOW_H

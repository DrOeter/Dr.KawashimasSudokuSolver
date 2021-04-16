#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <bitset>
#include <iostream>

typedef std::vector<unsigned short> usv;
typedef std::vector<std::vector<unsigned short>> ussv;
typedef std::vector<std::vector<bool>> ubbv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void getUi();

private slots:
    void on_button_clicked();

    void correction();

    ussv find_8(ussv field);

    ussv negative(ussv options);

    void p2d(ussv vector);

    void overFly(ussv field);

    usv collectRow(ussv field, int rc, char roc);

private:
    Ui::MainWindow *ui;
    ussv field, testfield, orig, pairs, coords, fills;
    usv choice;
    uint32_t com = 0;
    std::vector<QLineEdit*> pencil, clues;
};
#endif // MAINWINDOW_H

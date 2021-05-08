#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <bitset>
#include <iostream>

typedef std::vector<uint16_t> usv;
typedef std::vector<int16_t> sv;
typedef std::vector<std::vector<uint16_t>> ussv;
typedef std::vector<std::vector<int16_t>> ssv;
typedef std::vector<std::vector<bool>> bbv;

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

    void pB2d(bbv vector);

    void pUsv(usv vector);

    void overFly();

    void updateClues();

    usv collectRow(ussv field, int rc, char roc);

    void boxElim(bbv &box, sv rows, uint16_t i, uint16_t x, uint16_t y);

    void rowColSolve(sv pos_row, uint16_t x, uint16_t y, uint16_t xx, uint16_t yy);

private:
    Ui::MainWindow *ui;
    ussv field, testfield, orig, pairs, coords, fills;
    usv choice;
    uint32_t com = 0;
    std::vector<QLineEdit*> pencil, clues;
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
};
#endif // MAINWINDOW_H

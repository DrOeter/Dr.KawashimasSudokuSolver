#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <bitset>
#include <iostream>
#include <algorithm>

#define ARRAY_POS [ybox[gridxy[1]][yy]][xbox[gridxy[0]][xx]]
#define BOX_POS [ybox[box[1]][yy]][xbox[box[0]][xx]]

typedef std::vector<uint16_t> usv;
typedef std::vector<int16_t> sv;
typedef std::vector<std::vector<uint16_t>> ussv;
typedef std::vector<std::vector<std::vector<uint16_t>>> usssv;
typedef std::vector<std::vector<int16_t>> ssv;
typedef std::vector<std::vector<bool>> bbv;
typedef std::vector<bool> bv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_clicked();

private:
    void getGui();

    void initGui();

    void correction();

    ussv find_8();

    ussv negative(ussv options);

    int16_t find_v(usv v, uint16_t value);

    int16_t search_v(usv in, usv array);

    int16_t find_bv(bv v, uint16_t value);

    void erase(usv &v, uint16_t value);

    void clueElim();

    void pUssv(ussv vector);

    void pBbv(bbv vector);

    void pUsv(usv vector);

    void updateClues();

    void updatePencil();

    void updatePencilxy();

    void untilFind_8();

    void untilOverFly();

    void untilRowColSearch();

    ussv nakedDouble();

    ussv lockedCandidate();

    ussv rowColElim();

    ussv rowColSearch();

    usv findBox(uint16_t x ,uint16_t y);

    ussv overFly();

    usv collectRow(ussv field, int rc, char roc);

    void boxElim(bbv &box, sv rows, uint16_t i, uint16_t x, uint16_t y);

    void rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb);

    Ui::MainWindow *ui;
    usssv fillss;
    ussv field, testfield, orig, pairs, coords, fills;
    usv choice;
    uint32_t com = 0;
    std::vector<QLineEdit*> pencil, clues;
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
};
#endif // MAINWINDOW_H

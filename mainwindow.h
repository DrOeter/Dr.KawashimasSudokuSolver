#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <variant>
#include <string>
#include <cassert>

#define ARRAY_POS [ybox[gridxy[1]][yy]][xbox[gridxy[0]][xx]]

typedef std::vector<uint16_t*> uspv;
typedef std::vector<uint16_t> usv;
typedef std::vector<int16_t> sv;
typedef std::vector<std::vector<uint16_t>> ussv;
typedef std::vector<std::vector<uint16_t>*> usspv;
typedef std::vector<std::vector<uint16_t*>> uspsv;
typedef std::vector<std::vector<std::vector<uint16_t>>> usssv;
typedef std::vector<std::vector<std::vector<uint16_t>*>> ussspv;
typedef std::vector<std::vector<int16_t>> ssv;
typedef std::vector<std::vector<bool>> bbv;
typedef std::vector<bool> bv;
typedef std::vector<void*> vpv;
typedef std::vector<QLineEdit*> Qline_v;
class Sudoku;

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

    Ui::MainWindow *ui;
    Sudoku *sudoku;
    ussv field;
    Qline_v pencil, clues;
};


class Sudoku{
public:
    Sudoku(ussv m_field, Qline_v m_clues, Qline_v m_pencil)
        : field(m_field)
        , clues(m_clues)
        , pencil(m_pencil){
        Solve();
    }

private:
    void Solve();

    vpv find_8();

    ussv negative(ussv options);

    int16_t find_v(usv v, uint16_t value);

    int16_t search_v(usv in, usv array);

    int16_t find_bv(bv v, uint16_t value);

    void erase(usv &v, uint16_t value);

    void eraseP(usv *v, uint16_t value);

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

    void untilNakedDouble();

    void untilLockedCandidate();

    void untilInBoxLockedCandidate();

    ussv nakedDouble();

    ussv lockedCandidate();

    ussv inBoxLockedCandidate();

    ussv rowColElim();

    ussv rowColSearch();

    ussv overFly();

    usv collectRow(ussv field, int rc, char roc);

    void boxElim(bbv &box, sv rows, uint16_t i, uint16_t x, uint16_t y);

    void rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb);

    usssv fillss;
    ussv field, orig, coords, fills;
    std::vector<QLineEdit*> clues, pencil;
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
};

class SudokuBox{
public:
    SudokuBox(ussv m_field):field(m_field){ }

    usv getBox(uint16_t x ,uint16_t y);

    ussv get2dBox(uint16_t x ,uint16_t y);

    static usv findBox(uint16_t x ,uint16_t y);

    ussv field;
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
};

class SudokuBoxOptions{
public:

    SudokuBoxOptions(usssv &m_options):options(m_options){ }

    usspv getBox(uint16_t x ,uint16_t y);

    ussspv  get2dBox(uint16_t x ,uint16_t y);

    uint16_t getPos(bool xy, uint16_t coord);

    usssv &options;
    uint16_t x, y;
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
};

class SudokuRowCol{
public:
    SudokuRowCol(ussv m_field):field(m_field){ }

    usv getRow(uint16_t y);

    usv getCol(uint16_t x);

    ussv field;
};
#endif // MAINWINDOW_H

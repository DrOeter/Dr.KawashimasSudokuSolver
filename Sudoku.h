#ifndef SUDOKU_H
#define SUDOKU_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTime>
#include <QElapsedTimer>
#include <QLabel>
#include <QThread>
#include <QMutex>
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
typedef std::vector<std::vector<std::reference_wrapper<usv>>> usssrv;
typedef std::vector<std::vector<int16_t>> ssv;
typedef std::vector<std::vector<bool>> bbv;
typedef std::vector<bool> bv;
typedef std::vector<void*> vpv;
typedef std::vector<QLineEdit*> Qline_v;
class SudokuBoxOptions;
class MainWindow;

class SudokuThread{
public:
    SudokuThread(){}

    void start(ussv m_field, Qline_v m_clues, Qline_v m_pencil);

    bool hasIntegrity();

public slots:
    void tFinished();

private:
    ussv field;

};

class Sudoku{
public:
     friend SudokuBoxOptions;
     friend MainWindow;
     friend SudokuThread;

    Sudoku(){}

    Sudoku(ussv m_field, Qline_v m_clues, Qline_v m_pencil)
        : field(m_field)
        , clues(m_clues)
        , pencil(m_pencil){}

    Sudoku(ussv m_field, usssv m_fieldOptions)
        : field(m_field)
        , fieldOptions(m_fieldOptions){}

    bool hasIntegrity();

    void Solve();

    void AdvancedSolve(usv combi);

    ussv getField();
    usssv getFieldOptions();

private:
    enum class Axis{
        X,
        Y,
        XY
    };



    void thread( ussv field, usssv fieldOptions, uint16_t algo );

    void useAlgo( uint16_t algo );

    ussv find_8();

    ussv negative(ussv options);

    usv getFieldlist( ussv field );

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
/*
    void untilNakedDouble();

    void untilLockedCandidate();

    void untilInBoxLockedCandidate();
*/
    usssv nakedDouble();

    usssv nakedTriplet();

    void advancedHelper(usv &coords, usv &recoverd, usv position, bool &find );

    usssv lockedCandidate();

    usssv inBoxLockedCandidate();

    ussv rowColElim(Axis axis, uint16_t loop_value, usv coords);

    ussv rowColSearch();

    ussv overFly();

    usv collectRow(ussv field, int rc, Axis axis);

    void boxElim(bbv &box, sv rows, uint16_t i, uint16_t x, uint16_t y);

    void rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb);


    ussv field, orig, fieldOptionList;
    usssv fieldOptions;
    std::vector<QLineEdit*> clues, pencil;
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv listToPos = {{0, 0},
                      {1, 0},
                      {2, 0},
                      {0, 1},
                      {1, 1},
                      {2, 1},
                      {0, 2},
                      {1, 2},
                      {2, 2}};
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

    usv getBox(uint16_t x ,uint16_t y);

    ussv get2dBox(uint16_t x ,uint16_t y);

    usssv get3dBox(uint16_t x ,uint16_t y);

    uint16_t getPos(bool xy, uint16_t coord);

    void erase(uint16_t x, uint16_t y, uint16_t value);

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

class SudokuRowColOptions{
public:
    SudokuRowColOptions(usssv &m_options):options(m_options){ }

    ussv getRow(uint16_t y);

    ussv getCol(uint16_t x);

    usssv &options;
};

#endif

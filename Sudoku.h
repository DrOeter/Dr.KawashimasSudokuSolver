#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <thread>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <variant>
#include <string>
#include <cassert>
#include <chrono>

#define ARRAY_POS [sus::ybox[gridxy[1]][yy]][sus::xbox[gridxy[0]][xx]]

class SudokuBoxOptions;
class MainWindow;
class SudokuField;
class SudokuSolv;

typedef std::vector<uint16_t> usv;
typedef std::vector<int16_t> sv;
typedef std::vector<std::vector<uint16_t>> ussv;
typedef std::vector<std::vector<std::vector<uint16_t>>> usssv;
typedef std::vector<std::vector<int16_t>> ssv;
typedef std::vector<std::vector<bool>> bbv;
typedef std::vector<bool> bv;
typedef std::vector<void*> vpv;

namespace sus{
    const ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    const ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
    const ussv listToPos = {{0, 0},
                            {1, 0},
                            {2, 0},
                            {0, 1},
                            {1, 1},
                            {2, 1},
                            {0, 2},
                            {1, 2},
                            {2, 2}};
    const ssv position = {{1,2,1,2},
                          {1,-1,1,2},
                          {-1,-2,1,2},
                          {1,2,1,-1},
                          {1,-1,1,-1},
                          {-1,-2,1,-1},
                          {1,2,-1,-2},
                          {1,-1,-1,-2},
                          {-1,-2,-1,-2}};
}

class Sudoku{
public:
    Sudoku(){}

    Sudoku(std::string m_field){
        uint16_t i = 0;
        for(uint16_t y=0; y < 9;y++){
            field.push_back( {} );
            for(uint16_t x=0; x < 9;x++){
                field[y].push_back( (m_field.c_str()[i]) - '0' );
                i++;
            }
        }
    }

    Sudoku(ussv m_field):field(m_field){}

    Sudoku(uint16_t *m_field){}

    void start();

    bool hasIntegrity(ussv field = {});

    ussv getField();

    usssv getFieldOptions();

    void setFieldOptions(usssv fieldOptions);

private:
    ussv field;
    usssv fieldOptions;
};

class SudokuLogPack{
public:
    std::string message;
    usv coords;
    usv recoverd;
    usv value;
};

class SudokuLog{
public:
    SudokuLog(){}

    void append(SudokuLogPack m_log){
        log.push_back(m_log);
    }

private:
    std::vector<SudokuLogPack> log;
};

class SudokuSolv: public Sudoku{
public:
    friend SudokuBoxOptions;
    friend MainWindow;
    friend Sudoku;

    class SudokuField{
    public:

        SudokuField(){}
        SudokuField(ussv m_field, usssv m_fieldOptions):field(m_field), fieldOptions(m_fieldOptions){ }
        SudokuField(ussv m_field):field(m_field){ }

        usssv getFieldOptions(){
            return fieldOptions;
        }
        ussv getField(){
            return field;
        }
        uint16_t getAlgo(){
            return algo;
        }
        void setAlgo(uint16_t m_algo){
            algo = m_algo;
        }
        void setFieldOptions(usssv o){
            fieldOptions = o;
        }
        void setField(ussv f){
            field = f;
        }
        void setSudokuSolv(SudokuSolv sudoku){
            this->setField(sudoku.getField());
            this->setFieldOptions(sudoku.getFieldOptions());
        }
        bool operator!=(SudokuField sudoku){
            bool unequal = 0;
            if(this->getField() != sudoku.getField() || this->getFieldOptions() != sudoku.getFieldOptions())unequal = 1;
            return unequal;
        }
        bool operator==(SudokuField sudoku){
            bool equal = 0;
            if(this->getField() == sudoku.getField() && this->getFieldOptions() == sudoku.getFieldOptions())equal = 1;
            return equal;
        }

    private:
        uint16_t algo = 404;
        ussv field;
        usssv fieldOptions;
    };

    bool operator==(SudokuSolv sudoku){
        bool equal = 0;
        if(this->getField() == sudoku.getField() && this->getFieldOptions() == sudoku.getFieldOptions())equal = 1;
        return equal;
    }

    SudokuSolv(){}

    SudokuSolv(SudokuField m_sf){
        field = m_sf.getField();
        fieldOptions = m_sf.getFieldOptions();
    }

    SudokuSolv(ussv m_field)
        : field(m_field){}

    SudokuSolv(ussv m_field, usssv m_fieldOptions)
        : field(m_field)
        , fieldOptions(m_fieldOptions){}

    void Solve();

    ussv getField();

    usssv getFieldOptions();

    SudokuField *getSudokuField();

    void setField(ussv f);

    void setFieldOptions(usssv o);

    void setSudokuField(SudokuField *m_sfield);

    bool hasFailed();

    void pUssv(ussv vector);

    void pBbv(bbv vector);

    void pUsv(usv vector);

private:
    enum class Axis{
        X,
        Y,
        XY
    };

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

    void untilFind_8();

    void untilRefresh();

    void untilOverFly();

    usssv nakedDouble();

    usssv nakedTriplet();

    void advancedHelper(usv &coords, usv &recoverd, usv position, bool &find );

    usssv lockedCandidate();

    usssv inBoxLockedCandidate();

    ussv rowColElim();

    void hiddenSingle();

    ussv overFly();

    usv collectRow(ussv field, int rc, Axis axis);

    void boxElim(bbv &box, sv rows, uint16_t i, uint16_t x, uint16_t y);

    void rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb);

    SudokuField *sfield;
    ussv field, fieldOptionList;
    usssv fieldOptions;
};
typedef std::vector<SudokuSolv::SudokuField> sfv;

class SudokuBox{
public:
    SudokuBox(ussv m_field):field(m_field){ }

    usv getBox(uint16_t x ,uint16_t y);

    ussv get2dBox(uint16_t x ,uint16_t y);

    static usv findBox(uint16_t x ,uint16_t y);

    ussv field;
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

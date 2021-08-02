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

/** @file */

/** @brief  define dient zum durchiterieren durch bestimmte boxen als ob eine Box ein 2D Vector ist */
#define ARRAY_POS [sus::ybox[gridxy[1]][yy]][sus::xbox[gridxy[0]][xx]]

class SudokuBoxOptions;
class MainWindow;
class SudokuField;
class SudokuSolv;

/** @brief  short vector */
typedef std::vector<int16_t> sv;
/** @brief  2D short vector */
typedef std::vector<std::vector<int16_t>> ssv;
/** @brief  usigned short vector */
typedef std::vector<uint16_t> usv;
/** @brief  2D unsigned short vector */
typedef std::vector<std::vector<uint16_t>> ussv;
/** @brief  3D unsigned short vector */
typedef std::vector<std::vector<std::vector<uint16_t>>> usssv;
/** @brief  bool vector */
typedef std::vector<bool> bv;
/** @brief  2D bool vector */
typedef std::vector<std::vector<bool>> bbv;

/** @brief Namespace SudokuSolv für Konstante variablen und
 *         da die Typen SudokuField und SudokuFieldVector
 *         vorher noch nicht vollständig sind */
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

/** @brief Wird in der Implementierung benutzt
 */
class Sudoku{
public:
    /** @brief Default Constructor
     */
    Sudoku(){}

    /** @brief Constructor um einen 81-stelligen String in field zu schreiben
     *  @param m_field Bsp.: "103400005602000030...(bis 81 Stellen)"
     */
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

    /** @brief Constructor der field initialisiert
     *  @param m_field 2D vector
     */
    Sudoku(ussv m_field):field(m_field){}

    void start();

    bool hasIntegrity(ussv field = {});

    ussv getField();

    usssv getFieldOptions();

    void setFieldOptions(usssv fieldOptions);

    bool isCorrect = 0;

private:
    /** @brief 2D vector der die eigentlichen Werte des zu lösenden Sudoku enthält */
    ussv field;
    /** @brief 3D vector der für jedes Kästchen innerhalb des Gitters die Optionen
     *         für dieses Kästchen des zu lösenden Sudoku enthält */
    usssv fieldOptions;
};

struct SudokuLogPack{
    std::string message;
    usv coords;
    usv recoverd;
    usv value;
};

class SudokuLog{
public:
    /** @brief Default Constructor
     */
    SudokuLog(){}

    void append(SudokuLogPack m_log){
        log.push_back(m_log);
    }

private:
    std::vector<SudokuLogPack> log;
};

/** @brief Enthält alle benötigten Algorithmen */
class SudokuSolv: public Sudoku{
public:
    friend SudokuBoxOptions;
    friend MainWindow;
    friend Sudoku;

    /** @brief Fungiert als Knotenpunkt im Entscheidungsbaum */
    class SudokuField{
    public:
        /** @brief Default Constructor
         */
        SudokuField(){}

        /** @brief Constructor der field und fieldOptions initialisiert
         *  @param m_field 2D vector
         *  @param m_fieldOptions 3D vector
         */
        SudokuField(ussv m_field, usssv m_fieldOptions):field(m_field), fieldOptions(m_fieldOptions){ }

        /** @brief Constructor der field initialisiert
         *  @param m_field 2D vector
         */
        SudokuField(ussv m_field):field(m_field){ }

        /** @brief Constructor der field und fieldOptions über SudokuSolv initialisiert
         *  @param m_sudoku SudokuSolv Objekt
         */
        SudokuField(SudokuSolv &m_sudoku){ field = m_sudoku.getField(); fieldOptions = m_sudoku.getFieldOptions(); }

        /** @brief Gibt fieldOptions aus
         *  @return fieldOptions 3D vector
         */
        usssv getFieldOptions(){
            return fieldOptions;
        }

        /** @brief Gibt field aus
         *  @return field 2D vector
         */
        ussv getField(){
            return field;
        }
        uint16_t getAlgo(){
            return algo;
        }
        void setAlgo(uint16_t m_algo){
            algo = m_algo;
        }
        uint16_t getID(){
            return ID;
        }
        void setID(uint16_t m_ID){
            ID = m_ID;
        }
        void setFieldOptions(usssv o){
            fieldOptions = o;
        }
        void setField(ussv f){
            field = f;
        }
        void setSudokuSolv(SudokuSolv &sudoku){
            this->setField(sudoku.getField());
            this->setFieldOptions(sudoku.getFieldOptions());
        }

        /** @brief operator um zu prüfen ob field oder fieldOptions ungleich sind
         *  @param sudoku Das zu vergleichende SudokuField-Objekt
         *  @return 1 wenn ungleich 0 wenn gleich
         */
        bool operator!=(SudokuField &sudoku){
            bool unequal = 0;
            if(this->getField() != sudoku.getField() || this->getFieldOptions() != sudoku.getFieldOptions())unequal = 1;
            return unequal;
        }

        /** @brief operator um zu prüfen ob field und fieldOptions gleich sind
         *  @param sudoku Das zu vergleichende SudokuField-Objekt
         *  @return 1 wenn gleich 0 wenn ungleich
         */
        bool operator==(SudokuField &sudoku){
            bool equal = 0;
            if(this->getField() == sudoku.getField() && this->getFieldOptions() == sudoku.getFieldOptions())equal = 1;
            return equal;
        }

    private:
        uint16_t ID = 0;
        uint16_t algo = 404;

        /** @brief 2D vector der die eigentlichen Werte des zu lösenden Sudoku enthält */
        ussv field;

        /** @brief 3D vector der für jedes Kästchen innerhalb des Gitters die Optionen
         *         für dieses Kästchen des zu lösenden Sudoku enthält */
        usssv fieldOptions;
    };

    /** @brief operator um zu prüfen ob field und fieldOptions gleich sind
     *  @param sudoku Das zu vergleichende SudokuSolv-Objekt
     *  @return 1 wenn gleich 0 wenn ungleich
     */
    bool operator==(SudokuSolv &sudoku){
        bool equal = 0;
        if(this->getField() == sudoku.getField() && this->getFieldOptions() == sudoku.getFieldOptions())equal = 1;
        return equal;
    }

    /** @brief Default Constructor
     */
    SudokuSolv(){}

    /** @brief Constructor der field und fieldOptions über SudokuField initialisiert
     *  @param m_sudoku SudokuSolv Objekt
     */
    SudokuSolv(SudokuField &m_sf){
        field = m_sf.getField();
        fieldOptions = m_sf.getFieldOptions();
    }

    /** @brief Constructor der field initialisiert
     *  @param m_field 2D vector
     */
    SudokuSolv(ussv &m_field)
        : field(m_field){}

    /** @brief Constructor der field und fieldOptions initialisiert
     *  @param m_field 2D vector
     *  @param m_fieldOptions 3D vector
     */
    SudokuSolv(ussv &m_field, usssv &m_fieldOptions)
        : field(m_field)
        , fieldOptions(m_fieldOptions){}

    uint16_t ID = 0;

    void Solve();

    ussv getField();

    usssv getFieldOptions();

    void setField(ussv f);

    void setFieldOptions(usssv o);

    bool hasFailed();

    void pUssv(ussv vector);

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

    void erase(usv &v, uint16_t value);

    void clueElim();

    void untilFind_8();

    void untilRefresh();

    void untilOverFly();

    usssv nakedDouble();

    usssv nakedTriplet();

    void advancedHelper(usv &coords, usv &recoverd, usv position, bool &find, bool isDouble );

    usssv lockedCandidate();

    usssv inBoxLockedCandidate();

    ussv rowColElim();

    void hiddenSingle();

    ussv overFly();

    usv collectRow(ussv field, int rc, Axis axis);

    void boxElim(bbv &box, sv rows, uint16_t i, uint16_t x, uint16_t y);

    void rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb);

    /** @brief 2D vector der die eigentlichen Werte des zu lösenden Sudoku enthält */
    ussv field;
    /** @brief 2D vector der für jedes Kästchen in einer liste die Optionen
     *         für dieses Kästchen des zu lösenden Sudoku enthält */
    ussv fieldOptionList;
    /** @brief 3D vector der für jedes Kästchen innerhalb des Gitters die Optionen
     *         für dieses Kästchen des zu lösenden Sudoku enthält */
    usssv fieldOptions;
};
/** @brief SudokuField als sf */
typedef SudokuSolv::SudokuField sf;
/** @brief Sudokufield vector */
typedef std::vector<sf> sfv;
/** @brief 2D SudokuField vector */
typedef std::vector<sfv> sffv;

/** @brief Selektiert eine bestimmte der 9 Boxen */
class SudokuBox{
public:
    SudokuBox(ussv m_field):field(m_field){ }

    usv getBox(uint16_t x ,uint16_t y);

    ussv get2dBox(uint16_t x ,uint16_t y);

    static usv findBox(uint16_t x ,uint16_t y);

    /** @brief 2D vector der die eigentlichen Werte des zu lösenden Sudoku enthält */
    ussv field;
};

/** @brief Selektiert die Optionen einer der 9 Boxen */
class SudokuBoxOptions{
public:

    SudokuBoxOptions(usssv &m_options):options(m_options){ }

    usv getBox(uint16_t x ,uint16_t y);

    ussv get2dBox(uint16_t x ,uint16_t y);

    usssv get3dBox(uint16_t x ,uint16_t y);

    uint16_t getPos(bool xy, uint16_t coord);

    void erase(uint16_t x, uint16_t y, uint16_t value);

    /** @brief 3D vector der für jedes Kästchen innerhalb des Gitters die Optionen
     *         für dieses Kästchen des zu lösenden Sudoku enthält */
    usssv &options;
    uint16_t x, y;
};

/** @brief Selektiert eine bestimmte der 9 Reihen oder Spalten */
class SudokuRowCol{
public:
    SudokuRowCol(ussv m_field):field(m_field){ }

    usv getRow(uint16_t y);

    usv getCol(uint16_t x);

    /** @brief 2D vector der die eigentlichen Werte des zu lösenden Sudoku */
    ussv field;
};

/** @brief Selektiert Optionen aus einer der 9 Spalten oder Reihen */
class SudokuRowColOptions{
public:
    SudokuRowColOptions(usssv &m_options):options(m_options){ }

    ussv getRow(uint16_t y);

    ussv getCol(uint16_t x);

    /** @brief 3D vector der für jedes Kästchen innerhalb des Gitters die Optionen
     *         für dieses Kästchen des zu lösenden Sudoku enthält */
    usssv &options;
};

#endif

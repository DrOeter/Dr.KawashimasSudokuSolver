#include "Sudoku.h"

usv SudokuRowCol::getRow(uint16_t y){
    usv row;
    for (int x=0; x < 9;x++) {
        row.push_back(field [y][x]);
    }
    return row;
}

usv SudokuRowCol::getCol(uint16_t x){
    usv col;
    for (int y=0; y < 9;y++) {
        col.push_back(field [y][x]);
    }
    return col;
}

ussv SudokuRowColOptions::getRow(uint16_t y){
    ussv row;
    for (int x=0; x < 9;x++) {
        row.push_back(options [y][x]);
    }
    return row;
}

ussv SudokuRowColOptions::getCol(uint16_t x){
    ussv col;
    for (int y=0; y < 9;y++) {
        col.push_back(options [y][x]);
    }
    return col;
}

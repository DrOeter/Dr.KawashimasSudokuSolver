#include "Sudoku.h"

/** @brief Gibt ausgewählte Reihe aus field aus
 *  @param y Position
 *  @return row als 1D vector
 */
usv SudokuRowCol::getRow(uint16_t y){
    usv row;
    for (int x=0; x < 9;x++) {
        row.push_back(field [y][x]);
    }
    return row;
}

/** @brief Gibt ausgewählte Spalte aus field aus
 *  @param x Position
 *  @return col als 1D vector
 */
usv SudokuRowCol::getCol(uint16_t x){
    usv col;
    for (int y=0; y < 9;y++) {
        col.push_back(field [y][x]);
    }
    return col;
}

/** @brief Gibt ausgewählte Reihe der Optionen aus
 *  @param y Position
 *  @return row als 2D vector
 */
ussv SudokuRowColOptions::getRow(uint16_t y){
    ussv row;
    for (int x=0; x < 9;x++) {
        row.push_back(options [y][x]);
    }
    return row;
}

/** @brief Gibt ausgewählte Spalte der Optionen aus
 *  @param x Position
 *  @return col als 2D vector
 */
ussv SudokuRowColOptions::getCol(uint16_t x){
    ussv col;
    for (int y=0; y < 9;y++) {
        col.push_back(options [y][x]);
    }
    return col;
}

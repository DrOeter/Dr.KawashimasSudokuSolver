#include "Sudoku.h"

/** @brief Selektiert eine der 9 Boxen aus der field variable
 *  @param x Boxposition
 *  @param y Boxposition
 *  @return Box als 1D vector
 */
usv SudokuBox::getBox(uint16_t x ,uint16_t y){
    usv box;
    usv gridxy = findBox(x, y);

    for (int yy=0; yy < 3;yy++) {
        for (int xx=0; xx < 3;xx++) {
            box.push_back(field ARRAY_POS);
        }
    }
    return box;
}

/** @brief Selektiert eine der 9 Boxen aus der field variable
 *  @param x Boxposition
 *  @param y Boxposition
 *  @return Box als 2D vector
 */
ussv SudokuBox::get2dBox(uint16_t x ,uint16_t y){
    ussv box;
    usv gridxy = findBox(x, y);

    for (int yy=0; yy < 3;yy++) {
        usv row;
        for (int xx=0; xx < 3;xx++) {
            row.push_back(field ARRAY_POS);
        }
        box.push_back( row );
    }
    return box;
}

/** @brief Gibt aus in welcher Box sich ein Kästchen befindet
 *  @param x fieldposition
 *  @param y fieldposition
 *  @return vector{Xboxposition, Yboxposition}
 */
usv SudokuBox::findBox(uint16_t x ,uint16_t y){
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
    usv gridxy = {0,0};
    for(; gridxy[0] < 3 ; gridxy[0]++)
        if ( std::find(xbox[gridxy[0]].begin(), xbox[gridxy[0]].end(), x) != xbox[gridxy[0]].end()) break;

    for(; gridxy[1] < 3 ; gridxy[1]++)
        if ( std::find(ybox[gridxy[1]].begin(), ybox[gridxy[1]].end(), y) != ybox[gridxy[1]].end()) break;

    return gridxy;
}

/** @brief Selektiert eine der 9 Boxen aus der fieldOptions variable
 *  @param x Boxposition
 *  @param y Boxposition
 *  @return Box als 1D vector
 */
usv SudokuBoxOptions::getBox(uint16_t x ,uint16_t y){
    this->x = x;
    this->y = y;
    usv box;

    for(auto &i: options)
        for(auto &ii: i)
            for(auto iii: ii)
                box.push_back( iii );

    return box;
}

/** @brief Selektiert eine der 9 Boxen aus der fieldOptions variable
 *  @param x Boxposition
 *  @param y Boxposition
 *  @return Box als 2D vector
 */
ussv SudokuBoxOptions::get2dBox(uint16_t x ,uint16_t y){
    this->x = x;
    this->y = y;
    ussv box;
    usv gridxy = SudokuBox::findBox(x, y);

    for (int yy=0; yy < 3;yy++) {
        for (int xx=0; xx < 3;xx++) {
            box.push_back(options ARRAY_POS);
        }
    }
    return box;
}

/** @brief Selektiert eine der 9 Boxen aus der fieldOptions variable
 *  @param x Boxposition
 *  @param y Boxposition
 *  @return Box als 3D vector
 */
usssv SudokuBoxOptions::get3dBox(uint16_t x ,uint16_t y){
    this->x = x;
    this->y = y;
    usssv box;
    usv gridxy = SudokuBox::findBox(x, y);

    for (int yy=0; yy < 3;yy++) {
        ussv row;
        for (int xx=0; xx < 3;xx++) {
            row.push_back(options ARRAY_POS);
        }
        box.push_back( row );
    }
    return box;
}

/** @brief Gibt aus in welcher Reihe oder Spalte sich
 *         ein Kästchen innerhalb einer Box befindet
 *  @param xy Reihe oder Spalte
 *  @param coord Werte von 0 bis 2
 *  @return Position
 */
uint16_t SudokuBoxOptions::getPos(bool xy , uint16_t coord){
    usv gridxy = SudokuBox::findBox(x, y);

    return sus::xbox[gridxy[ xy ]][coord];
}

/** @brief Löscht Element aus fieldOptions nach Wertübereinstimmung
 *  @param x Werte von 0 bis 2
 *  @param y Werte von 0 bis 2
 *  @param value Wert der gesucht wird
 *  @return void
 */
void SudokuBoxOptions::erase(uint16_t x, uint16_t y, uint16_t value){
    uint16_t xx = this->getPos(0, x);
    uint16_t yy = this->getPos(1, y);

    SudokuSolv().erase(options[yy][xx], value);
}

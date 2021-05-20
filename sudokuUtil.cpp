#include "mainwindow.h"

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

usspv SudokuBoxOptions::getBox(uint16_t x ,uint16_t y){
    usspv box;
    usv gridxy = SudokuBox::findBox(x, y);

    for (int yy=0; yy < 3;yy++) {
        for (int xx=0; xx < 3;xx++) {
            box.push_back(&options ARRAY_POS);
        }
    }
    return box;
}

ussspv SudokuBoxOptions::get2dBox(uint16_t x ,uint16_t y){
    ussspv box;
    usv gridxy = SudokuBox::findBox(x, y);

    for (int yy=0; yy < 3;yy++) {
        usspv row;
        for (int xx=0; xx < 3;xx++) {
            row.push_back(&options ARRAY_POS);
        }
        box.push_back( row );
    }
    return box;
}




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


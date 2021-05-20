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
    this->x = x;
    this->y = y;
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

uint16_t SudokuBoxOptions::getPos(bool xy , uint16_t coord){
    usv gridxy = SudokuBox::findBox(x, y);

    return xbox[gridxy[ xy ]][coord];
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



void Sudoku::updateClues(){
    int i = 0;
    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            if( field[y][x] != 0 ){
                clues[i]->setText( QString( std::to_string( field[y][x]).c_str() ) );
                clues[i]->setAlignment(Qt::AlignCenter);
                pencil[i]->hide();

            }
            i++;
        }
    }
}

void Sudoku::updatePencil(){
    for(int i=0; i < 81;i++){
        QString fill;
        for(auto i: fills[i])
            fill += std::to_string(i).c_str();
        pencil[i]->show();
        pencil[i]->setText( QString( fill ) );
        pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
    }
}

void Sudoku::updatePencilxy(){
    uint16_t i=0;
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {

            QString fill;
            for(auto i: fillss[y][x])
                fill += std::to_string(i).c_str();
            pencil[i]->show();
            pencil[i]->setText( QString( fill ) );
            pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
            i++;
        }
    }
}

void Sudoku::untilFind_8(){
    ussv state = field;
    usssv ostate = fillss;

    vpv data = find_8();
    field = *(ussv*)data[0];
    fillss = *(usssv*)data[1];

    while( state != field || ostate != fillss ){
        field = state;
        vpv data = find_8();
        state = *(ussv*)data[0];
        ostate = *(usssv*)data[1];

    }
}

void Sudoku::untilOverFly(){
    ussv state = field;
    field = overFly();

    untilRowColSearch();
    untilFind_8();

    while( state != field ){
        field = state;
        untilRowColSearch();
        untilFind_8();
        state = overFly();


    }
    untilRowColSearch();
    untilFind_8();
}

void Sudoku::untilRowColSearch(){
    ussv state = field;
    field = rowColSearch();

    while( state != field ){
        field = state;
        state = rowColSearch();

    }
}

void Sudoku::untilNakedDouble(){
    ussv state = field;
    field = nakedDouble();

    while( state != field ){
        field = state;
        state = nakedDouble();

    }
}

void Sudoku::untilLockedCandidate(){
    ussv state = field;
    field = lockedCandidate();

    while( state != field ){
        field = state;
        state = lockedCandidate();

    }
}

void Sudoku::untilInBoxLockedCandidate(){
    ussv state = field;
    field = inBoxLockedCandidate();

    while( state != field ){
        field = state;
        state = inBoxLockedCandidate();

    }
}

ussv Sudoku::negative(ussv options){
    ussv positive;
    for(uint32_t i=0; i < options.size();i++){
        positive.push_back( {} );
        int f=1;
        for(; f < 10 ; f++)
            if(std::find(options[i].begin(), options[i].end(), f) == options[i].end() && options[i][0] != 0) positive[i].push_back( f );
            //else if(options[i][0] == 0) positive[i].push_back( 0 );
    }

    return positive;
}

void Sudoku::pUssv(ussv vector){
    for(auto &ii: vector){
        for(auto i: ii){
            std::cout<< i;
        }
        std::cout<<std::endl;
    }
}

void Sudoku::pBbv(bbv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

void Sudoku::pUsv(usv vector){
    for(auto i: vector){
        std::cout<<i;
    }
    std::cout<<std::endl;
}




int16_t Sudoku::find_v(usv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

int16_t Sudoku::find_bv(bv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

void Sudoku::clueElim(){
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {
            if(fillss[y][x].size() == 1) field[y][x] = fillss[y][x][0];
        }
    }
}

void Sudoku::erase(usv &v, uint16_t value){
    v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

void Sudoku::eraseP(usv *v, uint16_t value){
    v->erase(std::remove(v->begin(), v->end(), value), v->end());
}

int16_t Sudoku::search_v(usv in, usv array){
    auto it = std::search(in.begin(), in.end(), array.begin(), array.end());
    if( it != in.end() ) return it - in.begin();
    else if( it == in.end() ) return -1;
    else return ~0L;
}

#include "Sudoku.h"

void SudokuSolv::untilFind_8(){
    ussv state = fieldOptionList;
    fieldOptionList = find_8();

    while( state != fieldOptionList ){
        fieldOptionList = state;
        state = find_8();
    }


    rowColElim();
    find_8();
}

void SudokuSolv::untilRefresh(){
    ussv state = fieldOptionList;
    fieldOptionList = find_8();

    while( state != fieldOptionList ){
        fieldOptionList = state;
        state = find_8();
    }
}

void SudokuSolv::untilOverFly(){
    if(hasIntegrity(field)) return;
    ussv state = field;
    overFly();
    A:

    while( state != field && hasIntegrity(state) ){
        state = field;
        untilFind_8();
        overFly();
    }
    ussv last_state = rowColElim();

    if(last_state != field && !last_state.empty() && !state.empty()) goto A;
    untilFind_8();
}

ussv SudokuSolv::negative(ussv options){
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

void SudokuSolv::pUssv(ussv vector){
    for(auto &ii: vector){
        for(auto i: ii){
            std::cout<< i;
        }
        std::cout<<std::endl;
    }
}

void SudokuSolv::pBbv(bbv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

void SudokuSolv::pUsv(usv vector){
    for(auto i: vector){
        std::cout<<i;
    }
    std::cout<<std::endl;
}

usv SudokuSolv::getFieldlist(ussv field){
    usv fieldList;

    for(auto &i: field){
        for(auto ii: i){
            fieldList.push_back( ii );
        }
    }
    return fieldList;
}

int16_t SudokuSolv::find_v(usv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

int16_t SudokuSolv::find_bv(bv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

void SudokuSolv::clueElim(){
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {
            if(fieldOptions[y][x].size() == 1) {
                field[y][x] = fieldOptions[y][x][0];
                fieldOptions[y][x] = {};
            }
        }
    }
}

bool SudokuSolv::hasFailed(){
    bool failed = 0;
    if(!field.empty() && !fieldOptions.empty()){
        for (int y=0; y < 9;y++) {
            for (int x=0; x < 9;x++) {
                if(fieldOptions[y][x] != usv({}) && field[y][x] == 0) failed = 1;
            }
        }
    }
    return failed;
}

void SudokuSolv::erase(usv &v, uint16_t value){
    v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

void SudokuSolv::eraseP(usv *v, uint16_t value){
    v->erase(std::remove(v->begin(), v->end(), value), v->end());
}

int16_t SudokuSolv::search_v(usv in, usv array){
    auto it = std::search(in.begin(), in.end(), array.begin(), array.end());
    if( it != in.end() ) return it - in.begin();
    else if( it == in.end() ) return -1;
    else return ~0L;
}

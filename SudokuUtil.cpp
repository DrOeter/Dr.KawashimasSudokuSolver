#include "Sudoku.h"

/** @brief Führt find_8() solange aus bis sich fieldOptionList nicht mehr verändert
 *  @return void
 */
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

/** @brief Führt find_8() solange aus bis sich fieldOptionList nicht mehr verändert
 *  @return void
 */
void SudokuSolv::untilRefresh(){
    ussv state = fieldOptionList;
    fieldOptionList = find_8();

    while( state != fieldOptionList ){
        fieldOptionList = state;
        state = find_8();
    }
}

/** @brief Führt overfly() solange aus bis sich field nicht mehr verändert
 *  @return void
 */
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

/** @brief Invertiert die Werte, die in Reihen, Spalten oder Boxen
 *         gefunden wurden, die nicht in einem Spezifischen Kästchen vorkommen
 *         dürfen. Dadurch hat man am Ende einen 2D vector mit allen Optionen.
 *  @param options Optionen die nicht vorkommen können
 *  @return Gültige Optionen
 */
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

/** @brief Druckt 2D vector
 *  @param 2D vector
 *  @return void
 */
void SudokuSolv::pUssv(ussv vector){
    for(auto &ii: vector){
        for(auto i: ii){
            std::cout<< i;
        }
        std::cout<<std::endl;
    }
}

/** @brief Druckt 1D vector
 *  @param 1D vector
 *  @return void
 */
void SudokuSolv::pUsv(usv vector){
    for(auto i: vector){
        std::cout<<i;
    }
    std::cout<<std::endl;
}

/** @brief Gibt field als 1D vector
 *  @param field 2D vector
 *  @return fieldList 1D vector
 */
usv SudokuSolv::getFieldlist(ussv field){
    usv fieldList;

    for(auto &i: field){
        for(auto ii: i){
            fieldList.push_back( ii );
        }
    }
    return fieldList;
}

/** @brief Sucht nach Wert in vector
 *  @param v vector in dem gesucht wird
 *  @param value Wert mit dem gesucht wird
 *  @return -1 wenn nicht gefunden sonst Position
 */
int16_t SudokuSolv::find_v(usv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

/** @brief Wenn ein Kästchen nur eine Option hat wird
 *         diese als Wert für das Kästchen übernommen
 *  @return void
 */
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

/** @brief Löscht Element aus vector nach Wertübereinstimmung
 *  @param v vector aus dem gelöscht wird
 *  @param value Wert nach dem gesucht wird
 *  @return void
 */
void SudokuSolv::erase(usv &v, uint16_t value){
    v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

int16_t SudokuSolv::search_v(usv in, usv array){
    auto it = std::search(in.begin(), in.end(), array.begin(), array.end());
    if( it != in.end() ) return it - in.begin();
    else if( it == in.end() ) return -1;
    else return ~0L;
}

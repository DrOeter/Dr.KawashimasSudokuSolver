#include "Sudoku.h"
#include <windows.h>

std::vector<std::string> permute(std::string a, int l, int r){
    static std::vector<std::string> sv;
    if (l == r) sv.push_back(a);
    else {
        for (int i = l; i <= r; i++) {
            std::swap(a[l], a[i]);
            permute(a, l+1, r);
            std::swap(a[l], a[i]);
        }
    }
    return sv;
}

uint64_t back(sfv list){
uint64_t back = 0;
for(auto i: list){
    if(!i.getField().empty()) back++;
    else break;
}
    return back;
}

void SudokuThread::start(ussv field){
    sfv list;
    std::vector<std::string> combis = permute("01234", 0, 4);
    uint16_t done = 0, c = 0, ci = 0, equals = 0;
    bool first = 0;
    Sudoku sudoku(field);
    sudoku.Solve();

    if(hasIntegrity(sudoku.getField())){
        field = sudoku.getField();
        done = 1;
    }
    Sudoku::SudokuField orig(sudoku.getField(), sudoku.getFieldOptions());

    A:
    QElapsedTimer timer;
    timer.start();
    first = 0;
    c = 0;
    equals = 0;
    Sudoku tmp_sudoku;
    list.clear();
    list.resize(10000);

    std::vector<QThread*> t(5);
    QMutex m;

    while(done == 0){
        for(uint16_t i=0; i < 5;i++){

            if(!list.empty() && first == 1){
                while(list[c].getField().empty() || list[c].getFieldOptions().empty() ){ c--; }
                sudoku.setField(list[c].getField());
                sudoku.setFieldOptions(list[c].getFieldOptions());
                c++;
            }
            if(tmp_sudoku == sudoku) equals++;
            tmp_sudoku = sudoku;

            t[i] = QThread::create([&](Sudoku ssudoku, uint16_t ii, uint16_t *done){
                if( *done == 1 || ssudoku.getField().empty()) return;
                Sudoku::SudokuField before, after;

                if(!ssudoku.hasIntegrity(ssudoku.getField()) && *done == 0){
                    before.setSudoku(ssudoku);

                    ssudoku.useAlgo(ii);

                    after.setSudoku(ssudoku);

                    if(before.getField() != after.getField() || before.getFieldOptions() != after.getFieldOptions()) list[::back(list)] = after;
                }
                if(ssudoku.hasIntegrity(ssudoku.getField()) && *done == 0){
                    *done = 1;
                    field = ssudoku.getField();
                }
            }, sudoku, (combis[ci][i] - '0'), &done );
            t[i]->start();
        }
        for(uint16_t i=0; i < 5;i++)
            t[i]->wait();
        first = 1;

        int mili = timer.elapsed();
        if(mili > 1000 || equals >= 100){
            sudoku.setField(orig.getField());
            sudoku.setFieldOptions(orig.getFieldOptions());
            std::cout<<"Penis "<<ci<<std::endl;
            ci++;
            goto A;
        }
    }

    this->fieldOptions = list[::back(list) - 1].getFieldOptions();
    this->field = field;

}

void Sudoku::Solve(){
    untilFind_8();
    untilOverFly();
}

void Sudoku::AdvancedSolve(usv combi){
    for(auto i: combi)
        useAlgo(i);
}

void SudokuThread::setFieldOptions(usssv o){
    fieldOptions = o;
}

usssv SudokuThread::getFieldOptions(){
    return fieldOptions;
}

ussv SudokuThread::getField(){
    return field;
}

ussv Sudoku::getField(){
    return field;
}

usssv Sudoku::getFieldOptions(){
    return fieldOptions;
}

void Sudoku::setFieldOptions(usssv o){
    fieldOptions = o;
}
void Sudoku::setField(ussv f){
    field = f;
}

void Sudoku::useAlgo(uint16_t algo){
    if(algo == 0) lockedCandidate();
    if(algo == 1) inBoxLockedCandidate();
    if(algo == 2) nakedDouble();
    if(algo == 3) nakedTriplet();
    if(algo == 4) hiddenSingle();
    if(algo == 5) rowColElim();
}

bool SudokuThread::hasIntegrity(ussv field){
    bool integrity = 1;
    if(!field.empty()) this->field = field;

    for(uint16_t y=0; y < 9;y++){
        usv row = SudokuRowCol(this->field).getRow(y);
        for(uint16_t value=1; value < 10;value++){
            uint16_t count = std::count (row.begin(), row.end(), value);
            if(count > 1 || count < 1) integrity = 0;
        }
    }

    for(uint16_t x=0; x < 9;x++){
        usv col = SudokuRowCol(this->field).getCol(x);
        for(uint16_t value=1; value < 10;value++){
            uint16_t count = std::count (col.begin(), col.end(), value);
            if(count > 1 || count < 1) integrity = 0;
        }
    }

    for(uint16_t y=0; y < 3;y++){
        for(uint16_t x=0; x < 3;x++){
            usv box = SudokuBox(this->field).getBox(x, y);
            for(uint16_t value=1; value < 10;value++){
                uint16_t count = std::count (box.begin(), box.end(), value);
                if(count > 1 || count < 1) integrity = 0;
            }
        }
    }

    return integrity;
}

void Sudoku::hiddenSingle(){
    if(hasIntegrity(field)) return;

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = xbox[xBoxPos][0];
            uint16_t y = ybox[yBoxPos][0];
            SudokuBoxOptions boxObj(fieldOptions);
            usssv options = boxObj.get3dBox(x, y);

            for(uint16_t value=1, count=0; value < 10;value++){
                usv coords;
                count = 0;
                for(uint16_t yy=0; yy < 3;yy++){
                    for(uint16_t xx=0; xx < 3;xx++){
                        if(find_v(options[yy][xx], value) != -1){
                            coords.push_back(boxObj.getPos(0, xx));
                            coords.push_back(boxObj.getPos(1, yy));
                            coords.push_back( xx );
                            coords.push_back( yy );
                            count++;
                        }
                    }
                }

                if(count == 1){
                    field[coords[1]][coords[0]] = value;
                    fieldOptions[coords[1]][coords[0]].clear();

                    for(uint16_t x=0; x < 9;x++){
                        if(SudokuBox::findBox(xBoxPos, yBoxPos) != SudokuBox::findBox(x, coords[3])){
                            erase(fieldOptions[coords[1]][x], value);
                        }
                    }
                    for(uint16_t y=0; y < 9;y++){
                        if(SudokuBox::findBox(xBoxPos, yBoxPos) != SudokuBox::findBox(coords[2], y)){
                            erase(fieldOptions[y][coords[0]], value);
                        }
                    }
                    clueElim();
                    find_8();
                }
            }
        }
    }
}

ussv Sudoku::rowColElim(){
    if(hasIntegrity(field)) return ussv();
    usv coords = {404,404,404};

        for(uint16_t y=0; y < 9;y++){
            bv line = {0,0,0,0,0,0,0,0,0};
            for(uint16_t value=1; value < 10;value++){

                uint16_t x_loc = 404;
                line = {0,0,0,0,0,0,0,0,0};
                for(uint16_t x=0; x < 9;x++){
                    if(field[y][x] == 0){
                        if(find_v(fieldOptions[y][x], value) == -1) line[x] = 1;
                        if(find_v(fieldOptions[y][x], value) != -1 && coords[0] != x && coords[1] != x && coords[2] != x) x_loc = x;
                    }
                    else if(field[y][x] != value) line[x] = 1;
                }
                uint16_t complete = 0;
                for(auto i: line)
                    if(i == 0) complete++;

                usv box;
                if(x_loc != 404) box = SudokuBox(field).getBox(x_loc, y);

                if(complete == 1 && line[x_loc] == 0 && x_loc != 404 && find_v(box, value) == -1 ) field[y][x_loc] = value;
            }
        }

        for(int x=0; x < 9;x++){
            bv line = {0,0,0,0,0,0,0,0,0};
            for(uint16_t value=1; value < 10;value++){

                uint16_t y_loc = 404;
                line = {0,0,0,0,0,0,0,0,0};
                for(uint16_t y=0; y < 9;y++){
                    if(field[y][x] == 0){
                        if(find_v(fieldOptions[y][x], value) == -1) line[y] = 1;
                        if(find_v(fieldOptions[y][x], value) != -1 && coords[0] != y && coords[1] != y && coords[2] != y) y_loc = y;
                    }
                    else if(field[y][x] != value) line[y] = 1;
                }
                uint16_t complete = 0;
                for(auto i: line)
                    if(i == 0) complete++;

                usv box;
                if(y_loc != 404) box = SudokuBox(field).getBox(x, y_loc);

                if(complete == 1 && line[y_loc] == 0 && y_loc != 404 && find_v(box, value) == -1 ) field[y_loc][x] = value;
            }
        }
    find_8();

    return field;
}

ussv Sudoku::find_8(){
    if(hasIntegrity(field)) return ussv();
    fieldOptionList.clear();


    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            usv fill;

            for(int yy=0; yy < 9 ; yy++)
                if ( find_v(fill, field[yy][x]) == -1 && field[yy][x] != 0 ) fill.push_back( field[yy][x] );

            for(int xx=0; xx < 9 ; xx++)
                if ( find_v(fill, field[y][xx]) == -1 && field[y][xx] != 0 ) fill.push_back( field[y][xx] );

            ussv box = SudokuBox(field).get2dBox(x, y);

            for(int y=0; y < 3;y++){
                for(int x=0; x < 3;x++){
                    if ( find_v(fill, box[y][x]) == -1 && box[y][x] != 0 )
                        fill.push_back( box[y][x] );
                }
            }

            if(field[y][x] == 0 && fill.size() == 8){
                int f=1;
                for(; f < 10 ; f++)
                    if(std::find(fill.begin(), fill.end(), f) == fill.end()) break;

                if(field[y][x] == 0) field[y][x] = f;
            }
            if(field[y][x] == 0) fieldOptionList.push_back( fill );
            else if(field[y][x] != 0) fieldOptionList.push_back( usv() = {0} );
        }
    }
    fieldOptionList = negative( fieldOptionList );
    ussv new_fills;

    if(!fieldOptions.empty()) {
        uint16_t i = 0;
        for(int y=0; y < 9;y++){
            for(int x=0; x < 9;x++){
                usv values;
                for(int value=1; value < 10;value++){
                    if(find_v(fieldOptions[y][x], value) != -1 && find_v(fieldOptionList[i], value) != -1 ){
                        values.push_back( value );
                    }
                }
                new_fills.push_back( values );
                i++;
            }
        }
    }
    else if(fieldOptions.empty()) new_fills = fieldOptionList;

    fieldOptions.clear();

    uint16_t ii = 0;
    for (uint16_t c=0; c < 9;c++ ) {
        ussv line(9);
        for (auto &i: line ) {
            i = new_fills[ii];
            ii++;
        }
        fieldOptions.push_back(line);
    }
    //rowColElim(Axis::XY, 0 , usv() = {404,404,404});

    clueElim();

    return fieldOptionList;
}


usv Sudoku::collectRow(ussv field, int rc, Axis axis){
    usv collect;
    if(axis == Axis::X){
        for(int i=0; i < 9;i++)
            if( field[i][rc] != 0 ) collect.push_back( field[i][rc] );
    }
    else if(axis == Axis::Y){
        for(int i=0; i < 9;i++)
            if( field[rc][i] != 0 ) collect.push_back( field[rc][i] );
    }

    return collect;
}

void Sudoku::boxElim(bbv &box, sv rows, uint16_t i,uint16_t x, uint16_t y){
    if(i == 0 || i == 1){
        for(int el=0; el < 3;el++)
            box[ el ][ x + rows[i] ] = 1;
    }
    else if(i == 2 || i == 3){
        for(int el=0; el < 3;el++)
            box[ y + rows[i] ][ el ] = 1;
    }

}

void Sudoku::rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb){

    ussv clues;
    clues.push_back( collectRow( field, x + pos_row[0], Axis::X ) );
    clues.push_back( collectRow( field, x + pos_row[1], Axis::X ) );
    clues.push_back( collectRow( field, y + pos_row[2], Axis::Y ) );
    clues.push_back( collectRow( field, y + pos_row[3], Axis::Y ) );

    usv gridxy = SudokuBox::findBox(x, y);

    for (int value=1; value < 10;value++ ){
        bbv box = {{0,0,0},
                   {0,0,0},
                   {0,0,0}};

        ussv selectBox = SudokuBox(field).get2dBox(x, y);

        bool value_isin = 0;

        for (int yy=0; yy < 3;yy++) {
            for (int xx=0; xx < 3;xx++) {
                if( selectBox[yy][xx] != 0) box[yy][xx] = 1;
                if( selectBox[yy][xx] == value) value_isin = 1;
            }
        }
        //std::cout<<"peniskakaka"<<value_isin<<y<<"  "<<x<<std::endl;

        if( value_isin ) continue;
        for(int i=0; i < 4;i++ ){
            if ( find_v(clues[i], value) != -1){
                boxElim(box, pos_row, i, xb, yb);
            }
        }
        uint16_t complete = 0;
        for(auto &ii: box){
            for(auto i: ii){
                if(i == 0) complete++;
            }
        }
        if( box[yb][xb] == 0 && complete == 1) {
            field[y][x] = value;
        }
        if(complete >= 2 && complete <= 9){
            usssv options = SudokuBoxOptions(fieldOptions).get3dBox(x, y);

            uint16_t pos[2], one = 0;
            for (int yy=0; yy < 3;yy++) {
                for (int xx=0; xx < 3;xx++) {
                    if( selectBox[yy][xx] == 0 && box[yy][xx] == 0) {
                        if ( find_v(options[yy][xx], value) != -1){
                            pos[0] = (yy - yb) + y;
                            pos[1] = (xx - xb) + x;
                            one++;
                        }
                    }
                }
            }
            if(one == 1 && field[ pos[0] ][ pos[1] ] == 0 ) field[ pos[0] ][ pos[1] ] = value;
        }
    }
}

ussv Sudoku::overFly(){
    if(hasIntegrity(field)) return ussv();
    ssv position = {{1,2,1,2},
                    {1,-1,1,2},
                    {-1,-2,1,2},
                    {1,2,1,-1},
                    {1,-1,1,-1},
                    {-1,-2,1,-1},
                    {1,2,-1,-2},
                    {1,-1,-1,-2},
                    {-1,-2,-1,-2}};
    ussv newfield = field;

    for(uint16_t y=0; y < 9;y++){
        for(uint16_t x=0; x < 9;x++){
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y + 2
                rowColSolve(newfield, position[0], x, y, 0, 0);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y + 2
                rowColSolve(newfield, position[1], x, y, 1, 0);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x - 1 & x - 2 & y + 1 & y + 2
                rowColSolve(newfield, position[2], x, y, 2, 0);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y - 1
                rowColSolve(newfield, position[3], x, y, 0, 1);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y - 1
                rowColSolve(newfield, position[4], x, y, 1, 1);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 2 & y + 1 & y - 1
                rowColSolve(newfield, position[5], x, y, 2, 1);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x + 2 & y - 1 & y - 2
                rowColSolve(newfield, position[6], x, y, 0, 2);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x - 1 & y - 1 & y - 2
                rowColSolve(newfield, position[7], x, y, 1, 2);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x - 1 & x - 2 & y - 1 & y - 2
                rowColSolve(newfield, position[8], x, y, 2, 2);
            }
        }
    }
    field = newfield;
    return newfield;
}


ussv Sudoku::rowColSearch(){
    if(hasIntegrity(field)) return ussv();

    for(uint16_t y=0; y < 9;y++){
        for(uint16_t x=0; x < 9;x++){
            if(field[y][x] == 0){
                for(uint16_t value=1; value < 10;value++){
                    bv line = {0,0,0,0,0,0,0,0,0};

                    for(uint16_t x_line=0; x_line < 9; x_line++){
                        if(field[y][x_line] == 0){
                            for(uint16_t y_line=0; y_line < 9; y_line++){
                                if(field[y_line][x_line] == value) line[x_line] = 1;
                            }

                            ussv box = SudokuBox(field).get2dBox(x_line, y);
                            for (int yy=0; yy < 3;yy++) {
                                for (int xx=0; xx < 3;xx++) {
                                    if( box[yy][xx] == value) line[x_line] = 1;
                                }
                            }
                        }
                        else line[x_line] = 1;
                    }
                    uint16_t complete = 0;
                    for(auto i: line)
                        if(i == 0) complete++;

                    if(complete == 1 && line[x] == 0 && find_v(field[y], value) == -1) field[y][x] = value;

                    line = {0,0,0,0,0,0,0,0,0};

                    for(uint16_t y_line=0; y_line < 9; y_line++){
                        if(field[y_line][x] == 0){
                            for(uint16_t x_line=0; x_line < 9; x_line++){
                                if(field[y_line][x_line] == value) line[y_line] = 1;
                            }

                            ussv box = SudokuBox(field).get2dBox(x, y_line);
                            for (int yy=0; yy < 3;yy++) {
                                for (int xx=0; xx < 3;xx++) {
                                    if(box[yy][xx] == value) line[y_line] = 1;
                                }
                            }
                        }
                        else line[y_line] = 1;
                    }
                    complete = 0;
                    for(auto i: line)
                        if(i == 0) complete++;

                    bool found = 0;
                    for(uint16_t i=0; i < 9; i++)
                        if(field[i][x] == value) found = 1;

                    if(complete == 1 && line[y] == 0 && found == 0) field[y][x] = value;
                }
            }
        }
    }
    return field;
}

usssv Sudoku::lockedCandidate(){
    if(hasIntegrity(field)) return usssv();

    for(uint16_t y=0; y < 9; y++){
        for(uint16_t value=1; value < 10; value++){
            uint16_t full_row = 0;
            bv box = {0,0,0};
             usv old_box = {404,404};
            for(uint16_t x=0; x < 9; x++){
                usv dec_box = SudokuBox::findBox(x, y);
                if(!fieldOptions[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fieldOptions[y][x], value) != -1 && find_v(xbox[i], x) != -1 ) box[i] = 1;
                }
                else if(field[y][xbox[dec_box[0]][0]] != 0 && field[y][xbox[dec_box[0]][1]] != 0 && field[y][xbox[dec_box[0]][2]] != 0 && old_box != dec_box)
                    full_row++;

                old_box = dec_box;
            }
            if( full_row == 2 )break;

            uint16_t complete = 0;
            for(auto i: box)
                if(i == 1)complete++;

            usv gridxy;
            usssv options;
            SudokuBoxOptions boxObj(fieldOptions);

            for(uint16_t i=0; i < 3; i++)
                if(box[i] == 1 && complete == 1)  {
                    options = boxObj.get3dBox(xbox[i][0], y);
                    gridxy = SudokuBox::findBox(xbox[i][0], y);
                }

            if(!options.empty()){
                for (int yy=0; yy < 3;yy++) {
                    for (int xx=0; xx < 3;xx++) {

                        int16_t pos = find_v(options[yy][xx], value);
                        if( boxObj.getPos(1, yy) != y && pos != -1 && !options[yy][xx].empty()) {
                            boxObj.erase( xx, yy, value);
                            clueElim();
                            find_8();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        for(uint16_t value=1; value < 10; value++){
            uint16_t full_col = 0;
            bv box = {0,0,0};
            usv old_box = {404,404};
            for(uint16_t y=0; y < 9; y++){
                usv dec_box = SudokuBox::findBox(x, y);
                if(!fieldOptions[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fieldOptions[y][x], value) != -1 && find_v(xbox[i], y) != -1 ) box[i] = 1;
                }

                else if(field[ybox[dec_box[1]][0]][x] != 0 && field[ybox[dec_box[1]][1]][x] != 0 && field[ybox[dec_box[1]][2]][x] != 0 && old_box != dec_box){
                    full_col++;
                    //box[SudokuBox::findBox(x, y)[1]] = 0;
                    //std::cout<<"hallo: "<<x<<" "<<y<<std::endl;
                    //std::cout<<"hoden: "<<field[ybox[dec_box[1]][0]][x]<<" "<<field[ybox[dec_box[1]][1]][x]<<" "<<field[ybox[dec_box[1]][2]][x]<<std::endl;
                }
                old_box = dec_box;
            }
            if( full_col == 2) break;
            uint16_t complete = 0;
            for(auto i: box)
                if(i == 1)complete++;

            usv gridxy;
            usssv options;
            SudokuBoxOptions boxObj(fieldOptions);

            for(uint16_t i=0; i < 3; i++)
                if(box[i] == 1 && complete == 1) {
                    options = boxObj.get3dBox(x, ybox[i][0]);
                    gridxy = SudokuBox::findBox(x, ybox[i][0]);
                }

            if(!options.empty()){
                for (int xx=0; xx < 3;xx++) {
                    for (int yy=0; yy < 3;yy++) {

                        int16_t pos = find_v(options[yy][xx], value);
                        if( boxObj.getPos(0, xx) != x && pos != -1 && !options[yy][xx].empty()) {
                            boxObj.erase( xx, yy, value);
                            clueElim();
                            find_8();
                        }
                    }
                }
            }
        }
    }
    untilFind_8();
    return fieldOptions;
}


usssv Sudoku::inBoxLockedCandidate(){
    if(hasIntegrity(field)) return usssv();

    usv gridxy = {0,0};
    for(gridxy[1] = 0;gridxy[1] < 3;gridxy[1]++){
        for(gridxy[0] = 0;gridxy[0] < 3;gridxy[0]++){
            for(uint16_t value=1; value < 10; value++){

                bv row = {0,0,0};
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if(!fieldOptions ARRAY_POS.empty()){
                            if(find_v(fieldOptions ARRAY_POS, value) != -1){
                                row[yy] = 1;
                            }
                        }
                    }
                }
                uint16_t complete = 0;
                for(auto i: row)
                    if(i == 1)complete++;

                for(uint16_t y=0; y < 3; y++){
                    if(row[y] == 1 && complete == 1) {
                        for(uint16_t x=0; x < 9; x++){
                            if( find_v(xbox[gridxy[0]], x) == -1 && find_v(fieldOptions[ybox[gridxy[1]][y]][x], value) != -1 ){
                                erase(fieldOptions[ybox[gridxy[1]][y]][x], value);
                                clueElim();
                                find_8();
                            }
                        }
                    }
                }
            }
        }
    }
    gridxy.clear();
    gridxy = {0,0};

    for(gridxy[1] = 0;gridxy[1] < 3;gridxy[1]++){
        for(gridxy[0] = 0;gridxy[0] < 3;gridxy[0]++){
            for(uint16_t value=1; value < 10; value++){

                bv col = {0,0,0};
                for(uint16_t xx=0; xx < 3; xx++){
                    for(uint16_t yy=0; yy < 3; yy++){
                        if(!fieldOptions ARRAY_POS.empty()){
                            if(find_v(fieldOptions ARRAY_POS, value) != -1){
                                col[xx] = 1;
                            }
                        }
                    }
                }
                uint16_t complete = 0;
                for(auto i: col)
                    if(i == 1)complete++;

                for(uint16_t x=0; x < 3; x++){
                    if(col[x] == 1 && complete == 1) {
                        for(uint16_t y=0; y < 9; y++){
                            if( find_v(ybox[gridxy[1]], y) == -1 && find_v(fieldOptions[y][xbox[gridxy[0]][x]], value) != -1 ){
                                erase(fieldOptions[y][xbox[gridxy[0]][x]], value);
                                clueElim();
                                find_8();
                            }
                        }
                    }
                }
            }
        }
    }
    untilFind_8();

    return fieldOptions;
}

usssv Sudoku::nakedDouble(){
    if(hasIntegrity(field)) return usssv();

    for(uint16_t y=0; y < 9; y++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {0, y}, find);

        if(find == 1 && coords.size() == 2){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
            }

            if(SudokuBox::findBox(coords[0], y) == SudokuBox::findBox(coords[1], y)){
                SudokuBoxOptions boxObj(fieldOptions);
                usssv options = boxObj.get3dBox(coords[0], y);
                usv gridxy = SudokuBox::findBox(coords[0], y);

                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && boxObj.getPos(1, yy) != y){
                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);

                            clueElim();
                            find_8();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {1, x}, find);

        if(find == 1 && coords.size() == 2){
            for(uint16_t y=0; y < 9; y++){
                if(coords[0] != y && coords[1] != y && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != y && coords[1] != y && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
            }

            if(SudokuBox::findBox(x, coords[0]) == SudokuBox::findBox(x, coords[1])){
                SudokuBoxOptions boxObj(fieldOptions);
                usssv options = boxObj.get3dBox(x, coords[0]);
                usv gridxy = SudokuBox::findBox(x, coords[0]);

                for(uint16_t xx=0; xx < 3; xx++){
                    for(uint16_t yy=0; yy < 3; yy++){
                        if( !options[yy][xx].empty() && boxObj.getPos(0, xx) != x){
                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);

                            clueElim();
                            find_8();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = xbox[xBoxPos][0];
            uint16_t y = ybox[yBoxPos][0];
            SudokuBoxOptions boxObj(fieldOptions);
            usssv options = boxObj.get3dBox(x, y);
            bool find = 0;
            usv recoverd = {404,404,404};
            usv coords;

            advancedHelper(coords, recoverd, {2, x, y}, find);

            if(find == 1 && coords.size() == 2){
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && options[yy][xx] != recoverd
                            && listToPos[coords[0]] != usv{xx, yy}
                            && listToPos[coords[1]] != usv{xx, yy} ){

                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                            clueElim();
                            find_8();
                        }
                    }
                }
            }
        }
    }

    untilFind_8();

    return fieldOptions;
}

usssv Sudoku::nakedTriplet(){
    if(hasIntegrity(field)) return usssv();

    for(uint16_t y=0; y < 9; y++){
        usv coords;
        usv recoverd = {404,404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {0, y}, find);

        if(find == 1 && coords.size() == 3){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fieldOptions[y][x], recoverd[2]) != -1 ) erase(fieldOptions[y][x], recoverd[2]);
            }

            if( SudokuBox::findBox(coords[0], y) == SudokuBox::findBox(coords[1], y) ){
                if( SudokuBox::findBox(coords[1], y) == SudokuBox::findBox(coords[2], y)){
                    SudokuBoxOptions boxObj(fieldOptions);
                    usssv options = boxObj.get3dBox(coords[0], y);
                    usv gridxy = SudokuBox::findBox(coords[0], y);

                    for(uint16_t yy=0; yy < 3; yy++){
                        for(uint16_t xx=0; xx < 3; xx++){
                            if( !options[yy][xx].empty() && boxObj.getPos(1, yy) != y){
                                if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                                if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                                if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                                clueElim();
                                find_8();
                            }
                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        usv coords;
        usv recoverd = {404,404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {1, x}, find);

        if(find == 1 && coords.size() == 3){
            for(uint16_t y=0; y < 9; y++){
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fieldOptions[y][x], recoverd[2]) != -1 ) erase(fieldOptions[y][x], recoverd[2]);
            }

            if(SudokuBox::findBox(x, coords[0]) == SudokuBox::findBox(x, coords[1])){
                if(SudokuBox::findBox(x, coords[1]) == SudokuBox::findBox(x, coords[2])){
                    SudokuBoxOptions boxObj(fieldOptions);
                    usssv options = boxObj.get3dBox(x, coords[0]);
                    usv gridxy = SudokuBox::findBox(x, coords[0]);

                    for(uint16_t xx=0; xx < 3; xx++){
                        for(uint16_t yy=0; yy < 3; yy++){
                            if( !options[yy][xx].empty() && boxObj.getPos(0, xx) != x){
                                if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                                if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                                if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                                clueElim();
                                find_8();
                            }
                        }
                    }
                }
            }
        }
    }

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = xbox[xBoxPos][0];
            uint16_t y = ybox[yBoxPos][0];
            SudokuBoxOptions boxObj(fieldOptions);
            usssv options = boxObj.get3dBox(x, y);
            bool find = 0;
            usv recoverd = {404,404,404};
            usv coords;

            advancedHelper(coords, recoverd, {2, x, y}, find);

            if(find == 1 && coords.size() == 3){
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && options[yy][xx] != recoverd
                            && listToPos[coords[0]] != usv{xx, yy}
                            && listToPos[coords[1]] != usv{xx, yy}
                            && listToPos[coords[2]] != usv{xx, yy}  ){

                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                            if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                            clueElim();
                            find_8();
                        }
                    }
                }
            }
        }
    }
    untilFind_8();

    return fieldOptions;
}


void Sudoku::advancedHelper(usv &coords, usv &recoverd, usv position, bool &find ){
    coords.clear();
    recoverd = {404,404,404};
    ussv rowColBox;
    if(position[0] == 0) rowColBox = SudokuRowColOptions(fieldOptions).getRow(position[1]);
    if(position[0] == 1) rowColBox = SudokuRowColOptions(fieldOptions).getCol(position[1]);
    if(position[0] == 2) rowColBox = SudokuBoxOptions(fieldOptions).get2dBox(position[1], position[2]);

    for(auto first = rowColBox.begin(); first != rowColBox.end(); first++) {

        if((*first).size() == 2){
            for(auto second = first + 1; second != rowColBox.end(); second++){

                if((*second).size() == 2 && (*first) == (*second) ){
                    find = 1;
                    coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                    coords.push_back( (uint16_t)(second - rowColBox.begin()) );

                    recoverd = (*first);
                    goto A;
                }

                if((*second).size() == 2
                    && ( ( ( (*first)[0] == (*second)[0]
                    && (*first)[1] != (*second)[1] )
                    || ( (*first)[0] != (*second)[0]
                    && (*first)[1] == (*second)[1] ) )

                    || ( ( (*first)[0] == (*second)[1]
                    && (*first)[1] != (*second)[0] )
                    || ( (*first)[0] != (*second)[1]
                    && (*first)[1] == (*second)[0] ) ) ) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                            && ( ( (*first)[0] == (*second)[1]
                            && (*first)[1] != (*second)[0]
                            && (*first)[1] == (*third)[1]
                            && (*first)[0] != (*third)[0]
                            && (*second)[0] == (*third)[0]
                            && (*second)[1] != (*third)[1] )

                            || ( (*first)[1] == (*second)[0]
                            && (*first)[0] != (*second)[1]
                            && (*first)[0] == (*third)[0]
                            && (*first)[1] != (*third)[1]
                            && (*second)[1] == (*third)[1]
                            && (*second)[0] != (*third)[0] )

                            || ( (*first)[1] == (*second)[1]
                            && (*first)[0] != (*second)[0]
                            && (*first)[0] == (*third)[1]
                            && (*first)[1] != (*third)[0]
                            && (*second)[0] == (*third)[0]
                            && (*second)[1] != (*third)[1] )

                            || ( (*first)[0] == (*second)[0]
                            && (*first)[1] != (*second)[1]
                            && (*first)[1] == (*third)[0]
                            && (*first)[0] != (*third)[1]
                            && (*second)[1] == (*third)[1]
                            && (*second)[0] != (*third)[0] )

                            || ( (*first)[1] == (*second)[1]
                            && (*first)[0] != (*second)[0]
                            && (*first)[0] == (*third)[0]
                            && (*first)[1] != (*third)[1]
                            && (*second)[0] == (*third)[1]
                            && (*second)[1] != (*third)[0] )

                            || ( (*first)[0] == (*second)[0]
                            && (*first)[1] != (*second)[1]
                            && (*first)[1] == (*third)[1]
                            && (*first)[0] != (*third)[0]
                            && (*second)[1] == (*third)[0]
                            && (*second)[0] != (*third)[1] ) ) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 2
                    && ( ( ( (*first)[0] == (*second)[0]
                    && (*first)[1] != (*second)[1] )
                    || ( (*first)[0] != (*second)[0]
                    && (*first)[1] == (*second)[1] ) )

                    || ( ( (*first)[0] == (*second)[1]
                    && (*first)[1] != (*second)[0] )
                    || ( (*first)[0] != (*second)[1]
                    && (*first)[1] == (*second)[0] ) ) ) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3
                            && find_v(*third, (*second)[0]) != -1
                            && find_v(*third, (*second)[1]) != -1
                            && find_v(*third, (*first)[0]) != -1
                            && find_v(*third, (*first)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 3
                    && find_v(*second, (*first)[0]) != -1
                    && find_v(*second, (*first)[1]) != -1 ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                           && find_v(*second, (*third)[0]) != -1
                           && find_v(*second, (*third)[1]) != -1

                           && ( ( ( (*first)[0] == (*third)[0]
                           && (*first)[1] != (*third)[1] )
                           || ( (*first)[0] != (*third)[0]
                           && (*first)[1] == (*third)[1] ) )

                           || ( ( (*first)[0] == (*third)[1]
                           && (*first)[1] != (*third)[0] )
                           || ( (*first)[0] != (*third)[1]
                           && (*first)[1] == (*third)[0] ) ) ) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*second);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 3
                    && find_v(*second, (*first)[0]) != -1
                    && find_v(*second, (*first)[1]) != -1  ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3
                            && (*second) == (*third)
                            && find_v(*third, (*first)[0]) != -1
                            && find_v(*third, (*first)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }
            }
        }


        else if((*first).size() == 3){
            for(auto second = first + 1; second != rowColBox.end(); second++){

                if((*second).size() == 3 && (*first) == (*second) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3 && (*third) == (*first) && (*second) == (*third) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 2
                    && find_v(*first, (*second)[0]) != -1
                    && find_v(*first, (*second)[1]) != -1 ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3
                            && (*third) == (*first)
                            && find_v(*third, (*second)[0]) != -1
                            && find_v(*third, (*second)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 3
                    && (*second) == (*first) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                           && find_v(*first, (*third)[0]) != -1
                           && find_v(*first, (*third)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*second);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 2
                    && find_v(*first, (*second)[0]) != -1
                    && find_v(*first, (*second)[1]) != -1  ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                            && find_v(*first, (*third)[0]) != -1
                            && find_v(*first, (*third)[1]) != -1

                            && ( ( ( (*second)[0] == (*third)[0]
                            && (*second)[1] != (*third)[1] )
                            || ( (*second)[0] != (*third)[0]
                            && (*second)[1] == (*third)[1] ) )

                            || ( ( (*second)[0] == (*third)[1]
                            && (*second)[1] != (*third)[0] )
                            || ( (*second)[0] != (*third)[1]
                            && (*second)[1] == (*third)[0] ) ) ) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*first);
                                goto A;
                        }
                    }
                }
            }
        }
    }
    A:
    return;
}

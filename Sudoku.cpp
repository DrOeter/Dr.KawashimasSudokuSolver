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
namespace sus {
    using namespace sus;

    uint64_t back(sfv list){
        uint64_t back = 0;
        for(auto i: list){
            if(!i.getField().empty()) back++;
            else break;
        }
        return back;
    }

    bool Pred(SudokuSolv::SudokuField a, SudokuSolv::SudokuField b){
            if (a.getAlgo() == b.getAlgo() && a.getAlgo() == 404)  return 1;
            else return 0;
        }

        void resize(sfv &list){
            //std::sort(list.begin(), list.end());
            auto ip = std::unique(list.begin(), list.end(), Pred);
            list.resize(std::distance(list.begin(), ip));
        }
}

void Sudoku::start(){
    sfv list, b_list;
    sffv history;
    ussv unsolved = field;

    std::vector<std::string> combis = permute("01234", 0, 4);
    uint16_t done = 0, c = 0, ci = 0, equals = 0, tmp_ci = 0, tmp_thread = 0;
    bool first = 0;
    SudokuSolv sudoku(field);
    sudoku.Solve();

    if(hasIntegrity(sudoku.getField())){
        field = sudoku.getField();
        done = 1;
    }
    SudokuSolv::SudokuField orig(sudoku.getField(), sudoku.getFieldOptions());

    /*A:
    sudoku.setField(orig.getField());
    sudoku.setFieldOptions(orig.getFieldOptions());
    ci = tmp_ci;*/

    while(done == 0){
        auto start = std::chrono::system_clock::now();
        first = 0;
        c = 0;
        equals = 0;
        tmp_thread = 0;
        list.clear();
        list.resize(10000);
        b_list.clear();
        b_list.resize(10000);

        std::vector<std::thread> t(5);

        while(done == 0){
            for(uint16_t i=0; i < 5;i++){


                t[i] = std::thread([&](SudokuSolv ssudoku, uint16_t ii, uint64_t id, uint16_t *done){
                    if( *done == 1 || ssudoku.getField().empty()) return;

                    SudokuSolv::SudokuField before, after;

                    if(!ssudoku.hasIntegrity(ssudoku.getField()) && *done == 0){


                        before.setSudokuSolv(ssudoku);
                        before.setAlgo(ii);
                        before.ID = ssudoku.ID;

                        ssudoku.useAlgo(ii);

                        after.setSudokuSolv(ssudoku);
                        after.setAlgo(ii);
                        after.ID = id + 1;

                       // std::cout<<"yeet1 "<<loc<<std::endl;


                        if(before != after){
                         //   std::cout<<"yeet2 "<<loc<<" "<<after.getAlgo()<<std::endl;
                            if(*done == 0) {
                                b_list[id] = before;
                                list[id] = after;

                                //std::cout<<"ii: "<<ii<<std::endl;
                            }
                        }
                    }

                    if(ssudoku.hasIntegrity(ssudoku.getField()) && *done == 0){
                        *done = 1;
                        field = ssudoku.getField();
                    }

                }, sudoku, (combis[ci][i] - '0'), (tmp_thread + i), &done );
                t[i].join();

            }
            //for(uint16_t i=0; i < 5;i++)
                //t[i].join();
            tmp_thread+=5;


          //  for(auto i: list)
            //        std::cout<<i.getAlgo()<<std::endl;

            if(!list.empty()){
                //while(list[c].getAlgo() == 404 ){ c++; }
                sudoku.setField(list[c].getField());
                sudoku.setFieldOptions(list[c].getFieldOptions());
                sudoku.ID = list[c].ID;
                //list[c].setAlgo(i);
                c++;
            }

           // std::cout<<"kak "<<c<<std::endl;
            /*else if(first == 1) c--;

            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double mili = elapsed.count();
            if(mili > 10 || equals >= 100){
                sudoku.setField(orig.getField());
                sudoku.setFieldOptions(orig.getFieldOptions());
                std::cout<<"Cycle "<<ci<<std::endl;
                ci++;
                break;
            }*/
        }
    }

    sus::resize(b_list);

    for(auto i=b_list.begin(); i != b_list.end() - 1; i++)
        if(i->getAlgo() == 404) b_list.erase(i);

    b_list.erase(b_list.end() - 1);

    sus::resize(list);

    for(auto i=list.begin(); i != list.end() - 1; i++)
        if(i->getAlgo() == 404) list.erase(i);

    list.erase(list.end() - 1);




    for(int16_t i=list.size()-1; i >= 0 ; i--){
        history.push_back({b_list[i], list[i]});

    }

    /*list.pop_back();
    b_list.pop_back();

    for(uint16_t i=0;i < list.size(); i++){
        std::cout<<b_list[i].getAlgo()<<std::endl;
        SudokuSolv().pUssv(b_list[i].getField());
        std::cout<<list[i].getAlgo()<<std::endl;
        SudokuSolv().pUssv(list[i].getField());

        std::cout<<std::endl;
    }*/


    /*SudokuSolv finalTest(orig);

    for(auto i: list)
        finalTest.useAlgo(i.getAlgo());

    if(finalTest.hasIntegrity(finalTest.getField())){

        std::cout<<"correct"<<std::endl;
    }*/


    sffv solution;
    bool start = 0;
    int64_t tmp_loc = 0, id = 0;
    int64_t loc = b_list.size() - 1;
    ussv tmp_field = field;
    uint16_t tmp_pos = 0;

        for(auto i=history.begin(); i != history.end();i++){
            /*std::cout<<(*i)[0].getAlgo()<<std::endl;
            SudokuSolv().pUssv((*i)[0].getField());
            std::cout<<(*i)[1].getAlgo()<<std::endl;
            SudokuSolv().pUssv((*i)[1].getField());*/
            if(sudoku.hasIntegrity((*i)[1].getField()) && start == 0) {
                start = 1;
                id = (*i)[0].ID;
                solution.push_back(*i);
            }
            else if(!sudoku.hasIntegrity((*i)[1].getField()) && start == 0)  continue;


            for(uint16_t pos=1; pos < history.size() - (i - history.begin());pos++){
                //std::cout<<"loc: "<<loc<<" "<<((*i)[0].getField() == (*(i + pos))[1].getField());
                //std::cout<<" op: "<<((*i)[0].getFieldOptions() == (*(i + pos))[1].getFieldOptions())<<std::endl;

                if( id == (*(i + pos))[1].ID ){
                    solution.push_back((*(i + pos)));
                    id = (*(i + pos))[0].ID;
                    break;
                }
            }
            if(solution.size() > 20) break;

            sffv tmp_sol = solution;
            std::reverse(tmp_sol.begin(), tmp_sol.end());

            SudokuSolv finalTest(orig);


            for(auto i: tmp_sol)
                finalTest.useAlgo(i[0].getAlgo());

            if(finalTest.hasIntegrity(finalTest.getField())){
                std::cout<<"correct"<<std::endl;
                isCorrect = 1;
                break;
            }
        }


    return;

    for(auto i: list){
            std::cout<<i.getAlgo()<<std::endl;
            SudokuSolv().pUssv(i.getField());
        }
        std::cout<<std::endl;
        for(auto i: solution){
            std::cout<<i[0].getAlgo()<<std::endl;
        }
        std::cout<<std::endl;




    //std::cout<<"sdfsdfs "<<loc<<std::endl;

   /* while(loc >= 0){
        while(loc >= 0){
            SudokuSolv test(b_list[loc].getField(), b_list[loc].getFieldOptions());
            test.useAlgo(b_list[loc].getAlgo());
            SudokuSolv::SudokuField tmp_test(test.getField(), test.getFieldOptions());

            std::cout<<"loc: "<<loc<<" "<<(tmp_test.getField() == tmp_sol.getField());
            std::cout<<" op: "<<(tmp_test.getFieldOptions() == tmp_sol.getFieldOptions())<<std::endl;

            SudokuSolv().pUssv(tmp_test.getField());

            std::cout<<std::endl;

            SudokuSolv().pUssv(tmp_sol.getField());

            std::cout<<std::endl;

            if( (start == 0 && tmp_test.getField() == tmp_sol.getField() && test.hasIntegrity(tmp_sol.getField()) )
                || (start == 1 && tmp_test == tmp_sol && !test.hasIntegrity(tmp_sol.getField()) ) ){
                solution.push_back(b_list[loc]);
                //b_list.erase(b_list.begin() + loc);
                tmp_loc = loc;
                start = 1;
                loc = b_list.size() - 1;
                break;

            }
            loc--;
        }
        if(!solution.empty())tmp_sol = solution.back();




    while(tmp_loc >= 0){
        if(!list[loc].getField().empty()) row = list[loc];

        for(uint16_t i = 0; i < 5; i++){
            SudokuSolv test(orig.getField(), orig.getFieldOptions());
            test.useAlgo(i);
            SudokuSolv::SudokuField tmp_test(test.getField(), test.getFieldOptions());

            if(tmp_test == row){
                //std::cout<<"yeettter"<<i<<std::endl;
                orig.setAlgo(i);
                solution.push_back(orig);
                loc = -1;
                break;
            }
        }
        loc--;
    }*/

  /*  std::reverse(solution.begin(), solution.end());

    SudokuSolv finalTest(orig);

    for(auto i: list){
    //  sudoku.pUssv(i.getField());
        std::cout<<i.getAlgo()<<std::endl;
    }
    std::cout<<std::endl;

    for(auto i: solution)
        finalTest.useAlgo(i.getAlgo());

    if(!finalTest.hasIntegrity(finalTest.getField())){
        done = 0;
        tmp_ci = ci;
        goto A;
    }
*/
    //if(!list.empty()) this->fieldOptions = list[sus::back(list) - 1].getFieldOptions();
}

void SudokuSolv::Solve(){
    untilFind_8();
    untilOverFly();
}

void Sudoku::setFieldOptions(usssv o){
    fieldOptions = o;
}

usssv Sudoku::getFieldOptions(){
    return fieldOptions;
}

ussv Sudoku::getField(){
    return field;
}

ussv SudokuSolv::getField(){
    return field;
}

usssv SudokuSolv::getFieldOptions(){
    return fieldOptions;
}

void SudokuSolv::setFieldOptions(usssv o){
    fieldOptions = o;
}
void SudokuSolv::setField(ussv f){
    field = f;
}
SudokuSolv::SudokuField *SudokuSolv::getSudokuField(){
    return sfield;
}
void SudokuSolv::setSudokuField(SudokuSolv::SudokuField *m_sfield){
    sfield = m_sfield;
}

void SudokuSolv::useAlgo(uint16_t algo){
    if(algo == 0) lockedCandidate();
    if(algo == 1) inBoxLockedCandidate();
    if(algo == 2) nakedDouble();
    if(algo == 3) nakedTriplet();
    if(algo == 4) hiddenSingle();
    //if(algo == 5) rowColElim();
}

bool Sudoku::hasIntegrity(ussv field){
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

void SudokuSolv::hiddenSingle(){
    if(hasIntegrity(field)) return;

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = sus::xbox[xBoxPos][0];
            uint16_t y = sus::ybox[yBoxPos][0];
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
                    untilRefresh();
                }
            }
        }
    }
}

ussv SudokuSolv::rowColElim(){
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
    untilRefresh();

    return field;
}

ussv SudokuSolv::find_8(){
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


usv SudokuSolv::collectRow(ussv field, int rc, Axis axis){
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

void SudokuSolv::boxElim(bbv &box, sv rows, uint16_t i,uint16_t x, uint16_t y){
    if(i == 0 || i == 1){
        for(int el=0; el < 3;el++)
            box[ el ][ x + rows[i] ] = 1;
    }
    else if(i == 2 || i == 3){
        for(int el=0; el < 3;el++)
            box[ y + rows[i] ][ el ] = 1;
    }

}

void SudokuSolv::rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb){

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

ussv SudokuSolv::overFly(){
    if(hasIntegrity(field)) return ussv();

    ussv newfield = field;

    for(uint16_t y=0; y < 9;y++){
        for(uint16_t x=0; x < 9;x++){
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y + 2
                rowColSolve(newfield, sus::position[0], x, y, 0, 0);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y + 2
                rowColSolve(newfield, sus::position[1], x, y, 1, 0);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x - 1 & x - 2 & y + 1 & y + 2
                rowColSolve(newfield, sus::position[2], x, y, 2, 0);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y - 1
                rowColSolve(newfield, sus::position[3], x, y, 0, 1);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y - 1
                rowColSolve(newfield, sus::position[4], x, y, 1, 1);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 2 & y + 1 & y - 1
                rowColSolve(newfield, sus::position[5], x, y, 2, 1);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x + 2 & y - 1 & y - 2
                rowColSolve(newfield, sus::position[6], x, y, 0, 2);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x - 1 & y - 1 & y - 2
                rowColSolve(newfield, sus::position[7], x, y, 1, 2);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x - 1 & x - 2 & y - 1 & y - 2
                rowColSolve(newfield, sus::position[8], x, y, 2, 2);
            }
        }
    }
    field = newfield;
    return newfield;
}

usssv SudokuSolv::lockedCandidate(){
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
                        if( find_v(fieldOptions[y][x], value) != -1 && find_v(sus::xbox[i], x) != -1 ) box[i] = 1;
                }
                else if(field[y][sus::xbox[dec_box[0]][0]] != 0 && field[y][sus::xbox[dec_box[0]][1]] != 0 && field[y][sus::xbox[dec_box[0]][2]] != 0 && old_box != dec_box)
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
                    options = boxObj.get3dBox(sus::xbox[i][0], y);
                    gridxy = SudokuBox::findBox(sus::xbox[i][0], y);
                }

            if(!options.empty()){
                for (int yy=0; yy < 3;yy++) {
                    for (int xx=0; xx < 3;xx++) {

                        int16_t pos = find_v(options[yy][xx], value);
                        if( boxObj.getPos(1, yy) != y && pos != -1 && !options[yy][xx].empty()) {
                            boxObj.erase( xx, yy, value);
                            clueElim();
                            untilRefresh();
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
                        if( find_v(fieldOptions[y][x], value) != -1 && find_v(sus::xbox[i], y) != -1 ) box[i] = 1;
                }

                else if(field[sus::ybox[dec_box[1]][0]][x] != 0 && field[sus::ybox[dec_box[1]][1]][x] != 0 && field[sus::ybox[dec_box[1]][2]][x] != 0 && old_box != dec_box){
                    full_col++;
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
                    options = boxObj.get3dBox(x, sus::ybox[i][0]);
                    gridxy = SudokuBox::findBox(x, sus::ybox[i][0]);
                }

            if(!options.empty()){
                for (int xx=0; xx < 3;xx++) {
                    for (int yy=0; yy < 3;yy++) {

                        int16_t pos = find_v(options[yy][xx], value);
                        if( boxObj.getPos(0, xx) != x && pos != -1 && !options[yy][xx].empty()) {
                            boxObj.erase( xx, yy, value);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }
    untilFind_8();
    return fieldOptions;
}


usssv SudokuSolv::inBoxLockedCandidate(){
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
                            if( find_v(sus::xbox[gridxy[0]], x) == -1 && find_v(fieldOptions[sus::ybox[gridxy[1]][y]][x], value) != -1 ){
                                erase(fieldOptions[sus::ybox[gridxy[1]][y]][x], value);
                                clueElim();
                                untilRefresh();
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
                            if( find_v(sus::ybox[gridxy[1]], y) == -1 && find_v(fieldOptions[y][sus::xbox[gridxy[0]][x]], value) != -1 ){
                                erase(fieldOptions[y][sus::xbox[gridxy[0]][x]], value);
                                clueElim();
                                untilRefresh();
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

usssv SudokuSolv::nakedDouble(){
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
                            untilRefresh();
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
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = sus::xbox[xBoxPos][0];
            uint16_t y = sus::ybox[yBoxPos][0];
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
                            && sus::listToPos[coords[0]] != usv{xx, yy}
                            && sus::listToPos[coords[1]] != usv{xx, yy} ){

                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }

    untilFind_8();

    return fieldOptions;
}

usssv SudokuSolv::nakedTriplet(){
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
                                untilRefresh();
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
                                untilRefresh();
                            }
                        }
                    }
                }
            }
        }
    }

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = sus::xbox[xBoxPos][0];
            uint16_t y = sus::ybox[yBoxPos][0];
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
                            && sus::listToPos[coords[0]] != usv{xx, yy}
                            && sus::listToPos[coords[1]] != usv{xx, yy}
                            && sus::listToPos[coords[2]] != usv{xx, yy}  ){

                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                            if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }
    untilFind_8();

    return fieldOptions;
}


void SudokuSolv::advancedHelper(usv &coords, usv &recoverd, usv position, bool &find ){
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

                                recoverd.clear();
                                recoverd.push_back((*first)[0]);
                                recoverd.push_back((*first)[1]);
                                recoverd.push_back((*second)[0]);
                                recoverd.push_back((*second)[1]);
                                recoverd.push_back((*third)[0]);
                                recoverd.push_back((*third)[1]);

                                std::sort(recoverd.begin(), recoverd.end());
                                auto ip = std::unique(recoverd.begin(), recoverd.end());
                                recoverd.resize(std::distance(recoverd.begin(), ip));

                                //pUsv(recoverd);

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

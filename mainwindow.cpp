#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Dr. Kawashima's Sudoku Solver");

    initGui();
    for(uint16_t i=0; i < 81;i++){
        pencil[i]->hide();
        clues[i]->setAlignment(Qt::AlignCenter);
        clues[i]->setMaxLength(1);
    }

    on_button_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_clicked()
{
    getGui();
    sudoku = new Sudoku(field, clues, pencil);
}

void Sudoku::Solve(){
    orig = field;
    QElapsedTimer myTimer;
    myTimer.start();

    untilFind_8();


    untilOverFly();

    lockedCandidate();
    nakedDouble();
    nakedTriplet();




    inBoxLockedCandidate();

    nakedDouble();

    lockedCandidate();

    int mili = myTimer.elapsed();
    std::cout<<"yeet "<<mili<<std::endl;

    updatePencilxy();
    updateClues();
}


ussv Sudoku::rowColElim(Axis axis, uint16_t loop_value, usv coords){
    if( axis == Axis::X || axis == Axis::XY){

        for(uint16_t y=0; y < 9;y++){
            bv line = {0,0,0,0,0,0,0,0,0};
            for(uint16_t value=1; value < 10;value++){

                uint16_t x_loc = 404;
                line = {0,0,0,0,0,0,0,0,0};
                for(uint16_t x=0; x < 9;x++){
                    if(field[y][x] == 0){
                        if(find_v(fillss[y][x], value) == -1) line[x] = 1;
                        if(find_v(fillss[y][x], value) != -1 && coords[0] != x && coords[1] != x && coords[2] != x) x_loc = x;

                        if(find_v(fillss[y][x], value) != -1 && loop_value != 0)
                            for(auto i: coords)
                                if(i == x && loop_value == value) line[x] = 1;

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
    }

    if( axis == Axis::Y || axis == Axis::XY){

        for(int x=0; x < 9;x++){
            bv line = {0,0,0,0,0,0,0,0,0};
            for(uint16_t value=1; value < 10;value++){

                uint16_t y_loc = 404;
                line = {0,0,0,0,0,0,0,0,0};
                for(uint16_t y=0; y < 9;y++){
                    if(field[y][x] == 0){
                        if(find_v(fillss[y][x], value) == -1) line[y] = 1;
                        if(find_v(fillss[y][x], value) != -1 && coords[0] != y && coords[1] != y && coords[2] != y) y_loc = y;

                        if(find_v(fillss[y][x], value) != -1 && loop_value != 0)
                            for(auto i: coords)
                                if(i == y && loop_value == value) line[y] = 1;

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
    }
    return field;
}

ussv Sudoku::find_8(){
    fills.clear();


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
            if(field[y][x] == 0) fills.push_back( fill );
            else if(field[y][x] != 0) fills.push_back( usv() = {0} );
        }
    }
    fills = negative( fills );
    ussv new_fills;

    if(!fillss.empty()) {
        uint16_t i = 0;
        for(int y=0; y < 9;y++){
            for(int x=0; x < 9;x++){
                usv values;
                for(int value=1; value < 10;value++){
                    if(find_v(fillss[y][x], value) != -1 && find_v(fills[i], value) != -1 ){
                        values.push_back( value );
                    }
                }
                new_fills.push_back( values );
                i++;
            }
        }
    }
    else if(fillss.empty()) new_fills = fills;

    fillss.clear();

    uint16_t ii = 0;
    for (uint16_t c=0; c < 9;c++ ) {
        ussv line(9);
        for (auto &i: line ) {
            i = new_fills[ii];
            ii++;
        }
        fillss.push_back(line);
    }
    //rowColElim(Axis::XY, 0 , usv() = {404,404,404});

    clueElim();

    return fills;
}


usv Sudoku::collectRow(ussv field, int rc, char roc){
    usv collect;
    if(roc == 'x'){
        for(int i=0; i < 9;i++)
            if( field[i][rc] != 0 ) collect.push_back( field[i][rc] );
    }
    else if(roc == 'y'){
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
    clues.push_back( collectRow( field, x + pos_row[0], 'x' ) );
    clues.push_back( collectRow( field, x + pos_row[1], 'x' ) );
    clues.push_back( collectRow( field, y + pos_row[2], 'y' ) );
    clues.push_back( collectRow( field, y + pos_row[3], 'y' ) );

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
            usssv options = SudokuBoxOptions(fillss).get3dBox(x, y);

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
    return newfield;
}


ussv Sudoku::rowColSearch(){

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
    for(uint16_t y=0; y < 9; y++){
        for(uint16_t value=1; value < 10; value++){
            uint16_t full_row = 0;
            bv box = {0,0,0};
             usv old_box = {404,404};
            for(uint16_t x=0; x < 9; x++){
                usv dec_box = SudokuBox::findBox(x, y);
                if(!fillss[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fillss[y][x], value) != -1 && find_v(xbox[i], x) != -1 ) box[i] = 1;
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
            SudokuBoxOptions boxObj(fillss);

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
                            erase(fillss ARRAY_POS, value);
                            clueElim();
                        }
                    }
                }
                untilFind_8();
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
                if(!fillss[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fillss[y][x], value) != -1 && find_v(xbox[i], y) != -1 ) box[i] = 1;
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
            SudokuBoxOptions boxObj(fillss);

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
                            erase(fillss ARRAY_POS, value);
                            clueElim();
                        }
                    }
                }
                untilFind_8();
            }
        }
    }
    return fillss;
}


usssv Sudoku::inBoxLockedCandidate(){
    usv gridxy = {0,0};
    for(gridxy[1] = 0;gridxy[1] < 3;gridxy[1]++){
        for(gridxy[0] = 0;gridxy[0] < 3;gridxy[0]++){
            for(uint16_t value=1; value < 10; value++){

                bv row = {0,0,0};
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if(!fillss ARRAY_POS.empty()){
                            if(find_v(fillss ARRAY_POS, value) != -1){
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
                            if( find_v(xbox[gridxy[0]], x) == -1 && find_v(fillss[ybox[gridxy[1]][y]][x], value) != -1 ) erase(fillss[ybox[gridxy[1]][y]][x], value);
                        }
                    }
                }
            }
            clueElim();
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
                        if(!fillss ARRAY_POS.empty()){
                            if(find_v(fillss ARRAY_POS, value) != -1){
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
                            if( find_v(ybox[gridxy[1]], y) == -1 && find_v(fillss[y][xbox[gridxy[0]][x]], value) != -1 ) erase(fillss[y][xbox[gridxy[0]][x]], value);
                        }
                    }
                }
            }
            clueElim();
        }
    }
    untilFind_8();

    return fillss;
}

usssv Sudoku::nakedDouble(){
    for(uint16_t y=0; y < 9; y++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404};
        bool find = 0;

        for(uint16_t x=0; x < 9; x++){
            if( fillss[y][x].size() == 2){

                pair.push_back(fillss[y][x]);
                coords.push_back(x);
                for(auto it = pair.begin(); it != pair.end() - 1; it++)
                    if(pair.size() > 1 && *it == pair.back()){
                        for(auto it2 = pair.begin(); it2 != pair.end() - 1; it2++){
                            uint16_t pos = it2 - pair.begin();
                            if(*it2 != pair.back()) coords.erase(coords.begin()+pos);
                        }
                        recoverd = pair.back();
                        find = 1;
                    }

                if(find == 1) break;
            }
        }
        if(find == 1 && coords.size() == 2){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && find_v(fillss[y][x], recoverd[0]) != -1 ) erase(fillss[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && find_v(fillss[y][x], recoverd[1]) != -1 ) erase(fillss[y][x], recoverd[1]);
            }

            if(SudokuBox::findBox(coords[0], y) == SudokuBox::findBox(coords[1], y)){
                SudokuBoxOptions boxObj(fillss);
                usssv options = boxObj.get3dBox(coords[0], y);
                usv gridxy = SudokuBox::findBox(coords[0], y);

                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && boxObj.getPos(1, yy) != y){
                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) erase( fillss ARRAY_POS, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) erase( fillss ARRAY_POS, recoverd[1]);
                        }
                    }
                }
            }
        }
        clueElim();
    }

    for(uint16_t x=0; x < 9; x++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404};
        bool find = 0;

        for(uint16_t y=0; y < 9; y++){
            if( fillss[y][x].size() == 2){

                pair.push_back(fillss[y][x]);
                coords.push_back(y);
                for(auto it = pair.begin(); it != pair.end() - 1; it++)
                    if(pair.size() > 1 && *it == pair.back()){
                        for(auto it2 = pair.begin(); it2 != pair.end() - 1; it2++){
                            uint16_t pos = it2 - pair.begin();
                            if(*it2 != pair.back()) coords.erase(coords.begin()+pos);
                        }
                        recoverd = pair.back();
                        find = 1;
                    }

                if(find == 1) break;
            }
        }

        if(find == 1 && coords.size() == 2){
            for(uint16_t y=0; y < 9; y++){
                if(coords[0] != y && coords[1] != y && find_v(fillss[y][x], recoverd[0]) != -1 ) erase(fillss[y][x], recoverd[0]);
                if(coords[0] != y && coords[1] != y && find_v(fillss[y][x], recoverd[1]) != -1 ) erase(fillss[y][x], recoverd[1]);
            }

            if(SudokuBox::findBox(x, coords[0]) == SudokuBox::findBox(x, coords[1])){
                SudokuBoxOptions boxObj(fillss);
                usssv options = boxObj.get3dBox(x, coords[0]);
                usv gridxy = SudokuBox::findBox(x, coords[0]);

                for(uint16_t xx=0; xx < 3; xx++){
                    for(uint16_t yy=0; yy < 3; yy++){
                        if( !options[yy][xx].empty() && boxObj.getPos(0, xx) != x){
                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) erase( fillss ARRAY_POS, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) erase( fillss ARRAY_POS, recoverd[1]);
                        }
                    }
                }
            }
        }
        clueElim();
    }
    untilFind_8();

    return fillss;
}

usssv Sudoku::nakedTriplet(){
    for(uint16_t y=0; y < 9; y++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404,404};
        bool find = 0;

        for(uint16_t x=0; x < 9; x++){
            if( fillss[y][x].size() == 3){

                pair.push_back(fillss[y][x]);
                coords.push_back(x);
                for(auto it = pair.begin(); it != pair.end() - 1; it++)
                    if(pair.size() > 2 && *it == pair.back()){
                        for(auto it2 = pair.begin(); it2 != pair.end() - 1; it2++){
                            uint16_t pos = it2 - pair.begin();
                            if(*it2 != pair.back()) coords.erase(coords.begin()+pos);
                        }
                        recoverd = pair.back();
                        find = 1;
                    }

                if(find == 1) break;
            }
        }
        if(find == 1 && coords.size() == 3){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fillss[y][x], recoverd[0]) != -1 ) erase(fillss[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fillss[y][x], recoverd[1]) != -1 ) erase(fillss[y][x], recoverd[1]);
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fillss[y][x], recoverd[2]) != -1 ) erase(fillss[y][x], recoverd[2]);
            }

            if( SudokuBox::findBox(coords[0], y) == SudokuBox::findBox(coords[1], y) ){
                if( SudokuBox::findBox(coords[1], y) == SudokuBox::findBox(coords[2], y)){
                    SudokuBoxOptions boxObj(fillss);
                    usssv options = boxObj.get3dBox(coords[0], y);
                    usv gridxy = SudokuBox::findBox(coords[0], y);

                    for(uint16_t yy=0; yy < 3; yy++){
                        for(uint16_t xx=0; xx < 3; xx++){
                            if( !options[yy][xx].empty() && boxObj.getPos(1, yy) != y){
                                if( find_v(options[yy][xx], recoverd[0]) != -1 ) erase( fillss ARRAY_POS, recoverd[0]);
                                if( find_v(options[yy][xx], recoverd[1]) != -1 ) erase( fillss ARRAY_POS, recoverd[1]);
                                if( find_v(options[yy][xx], recoverd[2]) != -1 ) erase( fillss ARRAY_POS, recoverd[2]);
                            }
                        }
                    }
                }
            }
        }
        clueElim();
    }

    for(uint16_t x=0; x < 9; x++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404,404};
        bool find = 0;

        for(uint16_t y=0; y < 9; y++){
            if( fillss[y][x].size() == 3){

                pair.push_back(fillss[y][x]);
                coords.push_back(y);
                for(auto it = pair.begin(); it != pair.end() - 1; it++)
                    if(pair.size() > 2 && *it == pair.back()){
                        for(auto it2 = pair.begin(); it2 != pair.end() - 1; it2++){
                            uint16_t pos = it2 - pair.begin();
                            if(*it2 != pair.back()) coords.erase(coords.begin()+pos);
                        }
                        recoverd = pair.back();
                        find = 1;
                    }

                if(find == 1) break;
            }
        }

        if(find == 1 && coords.size() == 3){
            for(uint16_t y=0; y < 9; y++){
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fillss[y][x], recoverd[0]) != -1 ) erase(fillss[y][x], recoverd[0]);
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fillss[y][x], recoverd[1]) != -1 ) erase(fillss[y][x], recoverd[1]);
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fillss[y][x], recoverd[2]) != -1 ) erase(fillss[y][x], recoverd[2]);
            }

            if(SudokuBox::findBox(x, coords[0]) == SudokuBox::findBox(x, coords[1])){
                if(SudokuBox::findBox(x, coords[1]) == SudokuBox::findBox(x, coords[2])){
                    SudokuBoxOptions boxObj(fillss);
                    usssv options = boxObj.get3dBox(x, coords[0]);
                    usv gridxy = SudokuBox::findBox(x, coords[0]);

                    for(uint16_t xx=0; xx < 3; xx++){
                        for(uint16_t yy=0; yy < 3; yy++){
                            if( !options[yy][xx].empty() && boxObj.getPos(0, xx) != x){
                                if( find_v(options[yy][xx], recoverd[0]) != -1 ) erase( fillss ARRAY_POS, recoverd[0]);
                                if( find_v(options[yy][xx], recoverd[1]) != -1 ) erase( fillss ARRAY_POS, recoverd[1]);
                                if( find_v(options[yy][xx], recoverd[2]) != -1 ) erase( fillss ARRAY_POS, recoverd[2]);
                            }
                        }
                    }
                }
            }
        }
        clueElim();
    }
    untilFind_8();

    return fillss;
}

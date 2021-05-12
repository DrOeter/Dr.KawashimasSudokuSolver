#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    field.clear();
    getGui();
    initGui();
    orig = field;
    on_button_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_clicked()
{

    correction();

    updateClues();
    std::cout<<std::endl;

    //p2d(field);

    std::cout<<std::endl;

    //p2d( fills );




}

void MainWindow::updateClues(){
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

void MainWindow::updatePencil(){
    for(int i=0; i < 81;i++){
        QString fill;
        for(auto i: fills[i])
            fill += std::to_string(i).c_str();

        pencil[i]->setText( QString( fill ) );
        pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
    }
}

void MainWindow::updatePencilxy(){
    uint16_t i=0;
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {

            QString fill;
            for(auto i: fillss[y][x])
                fill += std::to_string(i).c_str();

            pencil[i]->setText( QString( fill ) );
            pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
            i++;
        }
    }
}

void MainWindow::untilFind_8(){
    ussv state = field;
    field = find_8();

    while( state != field ){
        field = state;
        state = find_8();

    }
}

void MainWindow::untilOverFly(){
    ussv state = field;
    field = overFly();

    while( state != field ){
        field = state;
        state = overFly();

    }
}

void MainWindow::untilRowColSearch(){
    ussv state = field;
    field = rowColSearch();

    while( state != field ){
        field = state;
        state = rowColSearch();

    }
}

void MainWindow::correction(){
    untilFind_8();
    untilRowColSearch();
    untilOverFly();
    untilRowColSearch();
    untilFind_8();
    untilOverFly();
    untilFind_8();

    lockedCandidate();

    nakedDouble();

    updateClues();
    updatePencilxy();
}


ussv MainWindow::negative(ussv options){
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

void MainWindow::pUssv(ussv vector){
    for(auto &ii: vector){
        for(auto i: ii){
            std::cout<< i;
        }
        std::cout<<std::endl;
    }
}

void MainWindow::pBbv(bbv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

void MainWindow::pUsv(usv vector){
    for(auto i: vector){
        std::cout<<i;
    }
    std::cout<<std::endl;
}

ussv MainWindow::rowColElim(){
    for(int y=0; y < 9;y++){
        bv line = {0,0,0,0,0,0,0,0,0};
        for(int value=1; value < 10;value++){

            int x_loc = 404;
            line = {0,0,0,0,0,0,0,0,0};
            for(int x=0; x < 9;x++){
                if(field[y][x] == 0){
                    if(std::find(fillss[y][x].begin(), fillss[y][x].end(), value) == fillss[y][x].end()) line[x] = 1;
                    if(std::find(fillss[y][x].begin(), fillss[y][x].end(), value) != fillss[y][x].end()) x_loc = x;
                }
                else if(field[y][x] != value) line[x] = 1;
            }
            uint16_t complete = 0;
            for(auto i: line)
                if(i == 0) complete++;

            if(complete == 1 && line[x_loc] == 0 && x_loc != 404) field[y][x_loc] = value;
        }
    }


    for(int x=0; x < 9;x++){
        bv line = {0,0,0,0,0,0,0,0,0};
        for(int value=1; value < 10;value++){

            int y_loc = 404;
            line = {0,0,0,0,0,0,0,0,0};
            for(int y=0; y < 9;y++){
                if(field[y][x] == 0){
                    if(std::find(fillss[y][x].begin(), fillss[y][x].end(), value) == fillss[y][x].end()) line[y] = 1;
                    if(std::find(fillss[y][x].begin(), fillss[y][x].end(), value) != fillss[y][x].end()) y_loc = y;
                }
                else if(field[y][x] != value) line[y] = 1;
            }
            uint16_t complete = 0;
            for(auto i: line)
                if(i == 0) complete++;

            if(complete == 1 && line[y_loc] == 0 && y_loc != 404) field[y_loc][x] = value;
        }
    }
    return field;
}

ussv MainWindow::find_8(){
    fills.clear();
    fillss.clear();

    for(int i=0; i < 9;i++){
        for(int ii=0; ii < 9;ii++){
            usv fill;

            for(int y=0; y < 9 ; y++)
                if ( std::find(fill.begin(), fill.end(), field[y][ii]) == fill.end() && field[y][ii] != 0 ) fill.push_back( field[y][ii] );

            for(int x=0; x < 9 ; x++)
                if ( std::find(fill.begin(), fill.end(), field[i][x]) == fill.end() && field[i][x] != 0 ) fill.push_back( field[i][x] );

            usv gridxy = findBox(ii, i);

            for(int yy=0; yy < 3;yy++){
                for(int xx=0; xx < 3;xx++){
                    if ( std::find(fill.begin(), fill.end(), field ARRAY_POS) == fill.end()
                    && field ARRAY_POS != 0 )

                        fill.push_back( field ARRAY_POS );
                }
            }


            //std::cout<<fill.size()<<" pennis2 "<<ii + 1<<" "<<i + 1<<std::endl;

            if(field[i][ii] == 0 && fill.size() == 8){
                int f=1;
                for(; f < 10 ; f++)
                    if(std::find(fill.begin(), fill.end(), f) == fill.end()) break;

                if(field[i][ii] == 0) field[i][ii] = f;

                //std::cout<<fill.size()<<" penis2 "<<f<<"  "<<field[i][ii]<<std::endl;
            }

            if(field[i][ii] == 0) fills.push_back( fill );
            else fills.push_back( usv() = {0} );
        }
    }
    fills = negative( fills );
    uint16_t ii = 0;
    for (uint16_t c=0; c < 9;c++ ) {
        ussv line(9);
        for (auto &i: line ) {
            i = fills[ii];
            ii++;
        }
        /*pUssv(fills);
        std::cout<<"\npenis: "<<std::endl;
        pUssv(line);*/
        fillss.push_back(line);
    }

    rowColElim();


    return field;
}


usv MainWindow::collectRow(ussv field, int rc, char roc){
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

void MainWindow::boxElim(bbv &box, sv rows, uint16_t i,uint16_t x, uint16_t y){
    if(i == 0 || i == 1){
        for(int el=0; el < 3;el++)
            box[ el ][ x + rows[i] ] = 1;
    }
    else if(i == 2 || i == 3){
        for(int el=0; el < 3;el++)
            box[ y + rows[i] ][ el ] = 1;
    }

}

usv MainWindow::findBox(uint16_t x ,uint16_t y){
    usv gridxy = {0,0};
    for(; gridxy[0] < 3 ; gridxy[0]++)
        if ( std::find(xbox[gridxy[0]].begin(), xbox[gridxy[0]].end(), x) != xbox[gridxy[0]].end()) break;

    for(; gridxy[1] < 3 ; gridxy[1]++)
        if ( std::find(ybox[gridxy[1]].begin(), ybox[gridxy[1]].end(), y) != ybox[gridxy[1]].end()) break;

    return gridxy;
}

void MainWindow::rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb){

    ussv clues;
    clues.push_back( collectRow( field, x + pos_row[0], 'x' ) );
    clues.push_back( collectRow( field, x + pos_row[1], 'x' ) );
    clues.push_back( collectRow( field, y + pos_row[2], 'y' ) );
    clues.push_back( collectRow( field, y + pos_row[3], 'y' ) );

    usv gridxy = findBox(x, y);

    for (int value=1; value < 10;value++ ){
        bbv box = {{0,0,0},
                   {0,0,0},
                   {0,0,0}};

        bool value_isin = 0;

        for (int yy=0; yy < 3;yy++) {
            for (int xx=0; xx < 3;xx++) {
                if( field ARRAY_POS != 0) box[yy][xx] = 1;
                if( field ARRAY_POS == value) value_isin = 1;
            }
        }
        if( value_isin ) continue;
        for(int i=0; i < 4;i++ ){
            if ( std::find(clues[i].begin(), clues[i].end(), value) != clues[i].end()){
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
            pBbv(box);
        }
        if(complete >= 2 && complete <= 9){
            //std::cout<<"*********************************************************************************"<<value<<std::endl;
            uint16_t pos[2], one = 0;
            for (int yy=0; yy < 3;yy++) {
                for (int xx=0; xx < 3;xx++) {
                    if( field ARRAY_POS == 0 && box[yy][xx] == 0) {
                        if ( std::find(fillss ARRAY_POS.begin(), fillss ARRAY_POS.end(), value) != fillss ARRAY_POS.end()){
                            pos[0] = (yy - yb) + y;
                            pos[1] = (xx - xb) + x;
                            one++;
                            /*for(auto i: fillss ARRAY_POS)
                                std::cout<< i;
                            std::cout<<"\npenissack: "<<value<<std::endl;*/
                        }
                    }
                }
            }
            if(one == 1 && field[ pos[0] ][ pos[1] ] == 0 ) field[ pos[0] ][ pos[1] ] = value;
        }
    }
}

ussv MainWindow::overFly(){
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


ussv MainWindow::rowColSearch(){

    for(uint16_t y=0; y < 9;y++){
        for(uint16_t x=0; x < 9;x++){
            if(field[y][x] == 0){
                for(uint16_t value=1; value < 10;value++){
                    bv line = {0,0,0,0,0,0,0,0,0};
                    usv gridxy = findBox(x, y);

                    for(uint16_t x_line=0; x_line < 9; x_line++){
                        if(field[y][x_line] == 0){
                            for(uint16_t y_line=0; y_line < 9; y_line++){
                                if(field[y_line][x_line] == value) line[x_line] = 1;
                            }

                            usv box = findBox(x_line, y);
                                for (int yy=0; yy < 3;yy++) {
                                    for (int xx=0; xx < 3;xx++) {
                                        if(field BOX_POS == value) line[x_line] = 1;
                                    }
                                }
                        }
                        else line[x_line] = 1;
                    }
                    uint16_t complete = 0;
                    for(auto i: line)
                        if(i == 0) complete++;

                    if(complete == 1 && line[x] == 0
                       && std::find(field[y].begin(), field[y].end(), value) == field[y].end()) field[y][x] = value;



                    line = {0,0,0,0,0,0,0,0,0};
                    gridxy = findBox(x, y);

                    for(uint16_t y_line=0; y_line < 9; y_line++){
                        if(field[y_line][x] == 0){
                            for(uint16_t x_line=0; x_line < 9; x_line++){
                                if(field[y_line][x_line] == value) line[y_line] = 1;
                            }

                            usv box = findBox(x, y_line);
                            for (int yy=0; yy < 3;yy++) {
                                for (int xx=0; xx < 3;xx++) {
                                    if(field BOX_POS == value) line[y_line] = 1;
                                }
                            }
                        }
                        else line[y_line] = 1;
                    }
                    complete = 0;
                    for(auto i: line)
                        if(i == 0) complete++;

                    bool found;
                    for(uint16_t i=0; i < 9; i++)
                        if(field[i][x] == value) found = 1;

                    if(complete == 1 && line[y] == 0 && found == 0) field[y][x] = value;
                }
            }
        }
    }
    return field;
}

int16_t MainWindow::find_v(usv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

int16_t MainWindow::find_bv(bv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

void MainWindow::clueElim(){
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {
            if(fillss[y][x].size() == 1) field[y][x] = fillss[y][x][0];
        }
    }
}

ussv MainWindow::lockedCandidate(){
    for(uint16_t y=0; y < 9; y++){
        for(uint16_t value=1; value < 10; value++){
            bv box = {0,0,0};
            for(uint16_t x=0; x < 9; x++){
                usv dec_box = findBox(x, y);
                if(!fillss[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fillss[y][x], value) != -1 && find_v(xbox[i], x) != -1 ) box[i] = 1;
                    //if(value == 3 && y == 0)std::cout<<"yeet "<<find_v(fillss[y][x], value)<<std::endl;
                }
                else if(field[y][xbox[dec_box[0]][0]] != 0 && field[y][xbox[dec_box[0]][1]] != 0 && field[y][xbox[dec_box[0]][2]] != 0 )
                    box[findBox(x, y)[0]] = 1;
            }
            uint16_t complete = 0;
            for(auto i: box)
                if(i == 1)complete++;

            usv gridxy;
            for(uint16_t i=0; i < 3; i++)
                if(box[i] == 1 && complete == 1) gridxy = findBox(i, y);

           /* if(y == 1 && value == 3)
                for(auto i: box)
                    std::cout<<i;*/

            if(!gridxy.empty()){
                for (int yy=0; yy < 3;yy++) {
                    for (int xx=0; xx < 3;xx++) {

                        int16_t pos = find_v(fillss ARRAY_POS, value);
                        if(xbox[gridxy[0]][yy] != y && pos != -1 && !fillss ARRAY_POS.empty()) {
                            fillss ARRAY_POS.erase(std::remove(fillss ARRAY_POS.begin(), fillss ARRAY_POS.end(), value), fillss ARRAY_POS.end());
                            clueElim();

                            //if( y == 6) qInfo()<<"yee"<<gridxy[0]<<gridxy[1];

                            untilFind_8();
                            find_8();


                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        for(uint16_t value=1; value < 10; value++){
            bv box = {0,0,0};
            for(uint16_t y=0; y < 9; y++){
                usv dec_box = findBox(x, y);
                if(!fillss[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fillss[y][x], value) != -1 && find_v(xbox[i], y) != -1 ) box[i] = 1;
                    //if(value == 3 && y == 0)std::cout<<"yeet "<<find_v(fillss[y][x], value)<<std::endl;
                }
                else if(field[ybox[dec_box[0]][0]][x] != 0 && field[ybox[dec_box[0]][0]][x] != 0 && field[ybox[dec_box[0]][0]][x] != 0 )
                    box[findBox(x, y)[1]] = 1;
            }
            uint16_t complete = 0;
            for(auto i: box)
                if(i == 1)complete++;

            usv gridxy;
            for(uint16_t i=0; i < 3; i++)
                if(box[i] == 1 && complete == 1) gridxy = findBox(x, i);

            if(!gridxy.empty()){
                for (int yy=0; yy < 3;yy++) {
                    for (int xx=0; xx < 3;xx++) {

                        int16_t pos = find_v(fillss ARRAY_POS, value);
                        if(xbox[gridxy[1]][xx] != x && pos != -1 && !fillss ARRAY_POS.empty()) {
                            fillss ARRAY_POS.erase(std::remove(fillss ARRAY_POS.begin(), fillss ARRAY_POS.end(), value), fillss ARRAY_POS.end());
                            clueElim();

                            //if( y == 6) qInfo()<<"yee"<<gridxy[0]<<gridxy[1];

                            untilFind_8();
                            find_8();


                        }
                    }
                }
            }
        }
    }

    return field;
}

void MainWindow::erase(usv &v, uint16_t value){
    v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

int16_t MainWindow::search_v(usv in, usv array){
    auto it = std::search(in.begin(), in.end(), array.begin(), array.end());
    if( it != in.end() ) return it - in.begin();
    else if( it == in.end() ) return -1;
    else return ~0L;
}

ussv MainWindow::nakedDouble(){
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
                        for(auto it = pair.begin(); it != pair.end() - 1; it++){
                            uint16_t pos = it - pair.begin();
                            if(*it != pair.back()) coords.erase(coords.begin()+pos);
                        }
                        recoverd = pair.back();
                        find = 1;
                    }

                if(find == 1) break;
            }
        }

       /* pUssv(pair);
        for(auto i: coords)
            std::cout<<"x: "<<i;

        std::cout<<"\nfind "<<find<<std::endl;*/

        if(find == 1 && coords.size() == 2){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && find_v(fillss[y][x], recoverd[0]) != -1 ) erase(fillss[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && find_v(fillss[y][x], recoverd[1]) != -1 ) erase(fillss[y][x], recoverd[1]);
            }
        }
        clueElim();
        untilFind_8();
        find_8();
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
                        for(auto it = pair.begin(); it != pair.end() - 1; it++){
                            uint16_t pos = it - pair.begin();
                            if(*it != pair.back()) coords.erase(coords.begin()+pos);
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
        }
        clueElim();
        untilFind_8();
        find_8();
    }

    return field;
}

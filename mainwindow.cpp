#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    std::cout<<"penis"<<std::endl;

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

void MainWindow::untilFind_8(){
    ussv state = field;
    field = find_8();

    while( state != field ){
        field = state;
        state = find_8();

        std::cout<<"hallo123"<<std::endl;
    }
}

void MainWindow::untilOverFly(){
    ussv state = field;
    field = overFly();

    while( state != field ){
        field = state;
        state = overFly();

        std::cout<<"hallo123"<<std::endl;
    }
}

void MainWindow::untilRowColSearch(){
    ussv state = field;
    field = rowColSearch();

    while( state != field ){
        field = state;
        state = rowColSearch();

        std::cout<<"hallo123"<<std::endl;
    }
}

void MainWindow::correction(){
    untilFind_8();
    untilRowColSearch();

    //pUssv( fills );
    //pUssv(orig);
    //std::cout<<std::endl;

    untilOverFly();
    untilRowColSearch();
    untilFind_8();
    updateClues();
    updatePencil();
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
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
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
            std::cout<<"*********************************************************************************"<<value<<std::endl;
            uint16_t pos[2], one = 0;
            for (int yy=0; yy < 3;yy++) {
                for (int xx=0; xx < 3;xx++) {
                    if( field ARRAY_POS == 0 && box[yy][xx] == 0) {
                        if ( std::find(fillss ARRAY_POS.begin(), fillss ARRAY_POS.end(), value) != fillss ARRAY_POS.end()){
                            pos[0] = (yy - yb) + y;
                            pos[1] = (xx - xb) + x;
                            one++;
                            for(auto i: fillss ARRAY_POS)
                                std::cout<< i;
                            std::cout<<"\npenissack: "<<value<<std::endl;
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

                    if(x == 3 && y == 7 && value == 3){
                        for(auto i: line)
                            std::cout<<i;
                        std::cout<<"end"<<std::endl;
                    }



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

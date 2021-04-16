#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    std::cout<<"start"<<std::endl;

    on_button_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_clicked()
{
    field.clear();
    getUi();
    orig = field;
    correction();


    std::cout<<std::endl;

    //p2d(field);

    std::cout<<std::endl;

    //p2d( fills );




}


void MainWindow::correction(){
    ussv state = field;
    field = find_8( field );

    while( state != field ){
        field = state;
        state = find_8( field );

        std::cout<<"hallo123"<<std::endl;
    }
    fills = negative( fills );

    //p2d( fills );


    for(int i=0; i < 81;i++){
        QString fill;
        for(auto i: fills[i])
            fill += std::to_string(i).c_str();

        pencil[i]->setText( QString( fill ) );

        pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
    }

    p2d(orig);

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


    std::cout<<std::endl;


    overFly( field );



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

void MainWindow::p2d(ussv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

ussv MainWindow::find_8(ussv field){
    ussv x = {{0,1,2},{3,4,5},{6,7,8}};
    ussv y = {{0,1,2},{3,4,5},{6,7,8}};
    fills.clear();


    for(int i=0; i < 9;i++){
        for(int ii=0; ii < 9;ii++){
            usv fill;

            for(int y=0; y < 9 ; y++)
                if ( std::find(fill.begin(), fill.end(), field[y][ii]) == fill.end() && field[y][ii] != 0 ) fill.push_back( field[y][ii] );

            for(int x=0; x < 9 ; x++)
                if ( std::find(fill.begin(), fill.end(), field[i][x]) == fill.end() && field[i][x] != 0 ) fill.push_back( field[i][x] );

            int gridx=0;
            int gridy=0;

            //std::cout<<fill.size()<<" pennis"<<std::endl;

            for(; gridx < 3 ; gridx++)
                if ( std::find(x[gridx].begin(), x[gridx].end(), ii) != x[gridx].end()) break;

            for(; gridy < 3 ; gridy++)
                if ( std::find(y[gridy].begin(), y[gridy].end(), i) != y[gridy].end()) break;

            for(int yy=0; yy < 3;yy++){
                for(int xx=0; xx < 3;xx++){
                    if ( std::find(fill.begin(), fill.end(), field [y[gridy][yy]] [x[gridx][xx]]) == fill.end()
                    && field [y[gridy][yy]] [x[gridx][xx]] != 0 )

                        fill.push_back( field [y[gridy][yy]] [x[gridx][xx]] );
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

            if(orig[i][ii] == 0) fills.push_back( fill );

            else fills.push_back( usv() = {0} );
        }
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

void MainWindow::overFly(ussv field){
    ussv xbox = {{0,1,2},{3,4,5},{6,7,8}};
    ussv ybox = {{0,1,2},{3,4,5},{6,7,8}};
    ubbv box = {{0,0,0},
                {0,0,0},
                {0,0,0}};


    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y + 2
                ussv clues;
                clues.push_back( collectRow( field, x + 1, 'x' ) );
                clues.push_back( collectRow( field, x + 2, 'x' ) );
                clues.push_back( collectRow( field, y + 1, 'y' ) );
                clues.push_back( collectRow( field, y + 2, 'y' ) );

                int gridx=0;
                int gridy=0;

                for(; gridx < 3 ; gridx++)
                    if ( std::find(xbox[gridx].begin(), xbox[gridx].end(), x) != xbox[gridx].end()) break;

                for(; gridy < 3 ; gridy++)
                    if ( std::find(ybox[gridy].begin(), ybox[gridy].end(), y) != ybox[gridy].end()) break;

                for (int yy=0; yy < 3;yy++) {
                    for (int xx=0; xx < 3;xx++) {
                        if( field [ybox[gridy][yy]] [xbox[gridx][xx]] != 0) box[yy][xx] = 1;
                    }
                }

                for (int i=0; i < 9;i++ ){
                    if ( std::find(clues[0].begin(), clues[0].end(), i) != clues[0].end()){
                        box[0][1] = 1;
                        box[1][1] = 1;
                        box[2][1] = 1;
                    }
                    if ( std::find(clues[1].begin(), clues[1].end(), i) != clues[1].end()){
                        box[0][2] = 1;
                        box[1][2] = 1;
                        box[2][2] = 1;
                    }
                    if ( std::find(clues[2].begin(), clues[2].end(), i) != clues[2].end()){
                        box[1][0] = 1;
                        box[1][1] = 1;
                        box[1][2] = 1;
                    }
                    if ( std::find(clues[3].begin(), clues[3].end(), i) != clues[3].end()){
                        box[2][0] = 1;
                        box[2][1] = 1;
                        box[2][2] = 1;
                    }
                }


            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y + 2
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x - 1 & x - 2 & y + 1 & y + 2
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y - 1
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y - 1
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 2 & y + 1 & y - 1
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x + 2 & y - 1 & y - 2
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x - 1 & y - 1 & y - 2
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x - 1 & x - 2 & y - 1 & y - 2
            }

        }
    }

}



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(483, 550);
    this->setWindowTitle("Dr. Kawashima's SudokuSolv Solver");

    initGui();
    for(uint16_t i=0; i < 81;i++){
        pencil[i]->hide();
        pencil[i]->setReadOnly(false);
        clues[i]->setAlignment(Qt::AlignCenter);
        clues[i]->setMaxLength(1);
        clues[i]->installEventFilter(this);
        connect(clues[i], &QLineEdit::selectionChanged, clues[i], &QLineEdit::deselect);
        connect(pencil[i], &QLineEdit::selectionChanged, pencil[i], &QLineEdit::deselect);
    }

    //on_button_clicked();
}

bool MainWindow::eventFilter(QObject* object, QEvent* event){
    uint16_t find = 0;
    QLineEdit *line = NULL;

    for(auto i: clues)
        if(object == i){
            line = i;
            find++;
        }

    if( find == 1 && event->type() == QEvent::KeyPress) {
        line->setStyleSheet("QLineEdit { background-color: #1f1f1f } ");
        line->setText("");
        return false;
    }
    if( find == 1 && event->type() == QEvent::FocusOut) {
        char c = line->text().toStdString().c_str()[0];

        if( line->text().isEmpty() ) line->setStyleSheet("QLineEdit { background-color: #303030 } ");
        if( !line->text().isEmpty() && ( c < 49 || c > 57) ) {
            line->setStyleSheet("QLineEdit { background-color: #303030 } ");
            line->clear();
        }

        return false;
    }
    return false;
}

MainWindow::~MainWindow(){
    delete ui;
}

#define MODE_M
void MainWindow::on_button_clicked(){
    ui->button->setEnabled(false);
#ifdef MODE_S
    std::ifstream fi("C:\\Users\\Whoami\\Desktop\\Sudoku\\single.txt");
    std::ofstream fo("C:\\Users\\Whoami\\Desktop\\Sudoku\\single.txt", std::ios::app);
    ussv fileField;
    std::string line;
    uint16_t i = 0;
    std::getline(fi, line);

    for(uint16_t y=0; y < 9;y++){
        fileField.push_back( {} );
        for(uint16_t x=0; x < 9;x++){
            fileField[y].push_back( (line.c_str()[i]) - '0' );
            i++;
        }
    }

    getGui();

    std::string toFile;
    usv fieldList = SudokuSolv().getFieldlist( field );

    for(auto i: fieldList)
        toFile.append(std::to_string(i));
    toFile.append("\n");

    if(!line.empty() && field != fileField && std::count(fieldList.begin(), fieldList.end(), 0) == 81) field = fileField;
    else fo.write(toFile.c_str(), toFile.size());

    fo.close();
    fi.close();

    uint16_t it = 0;

    for(auto &i: field)
        for(auto ii: i){
            if( ii != 0 ) clues[it]->setStyleSheet("QLineEdit { background-color: #1f1f1f } ");
            it++;
        }



    Sudoku sudoku(field);

    sudoku.start();

/*    SudokuSolv sudoku(field);

    sudoku.untilFind_8();
    sudoku.untilOverFly();


    sudoku.useAlgo(2);
    sudoku.useAlgo(3);
    sudoku.useAlgo(2);
    sudoku.useAlgo(4);
    sudoku.useAlgo(4);*/
    //sudoku.useAlgo(0);


    /*sudoku.nakedDouble();
    sudoku.inBoxLockedCandidate();
    sudoku.lockedCandidate();
    sudoku.lockedCandidate();

    sudoku.lockedCandidate();
    sudoku.inBoxLockedCandidate();
    sudoku.nakedDouble();
    sudoku.inBoxLockedCandidate();
    sudoku.nakedTriplet();
    sudoku.inBoxLockedCandidate();
    */


    if(sudoku.hasIntegrity(sudoku.getField())){
        ui->label->setText("Solved");
        ui->label->setStyleSheet("QLabel { color: green } ");
    }
    else if(!sudoku.hasIntegrity(sudoku.getField())){
        ui->label->setText("Unsolved");
        ui->label->setStyleSheet("QLabel { color: red } ");
    }

    //updatePencilxy(sudoku.getFieldOptions());
    updateClues(sudoku.getField());
#endif

#ifdef MODE_M
    std::ifstream list("C:\\Users\\Whoami\\Desktop\\Sudoku\\college.txt");
    std::string line;
    uint64_t correct = 1, count = 1, combi = 0;

    while(!list.eof()){
        ussv ffield;
        uint16_t i = 0;
        std::getline(list, line);

        for(uint16_t y=0; y < 9;y++){
            ffield.push_back( {} );
            for(uint16_t x=0; x < 9;x++){
                ffield[y].push_back( (line.c_str()[i]) - '0' );
                i++;
            }
        }
        std::cout<<line<<std::endl;
        QElapsedTimer myTimer;
        myTimer.start();

        Sudoku sudoku(ffield);

        sudoku.start();

        int mili = myTimer.elapsed();
        std::cout<<"time: "<<mili<<std::endl;

        if(sudoku.hasIntegrity()) {
            if(sudoku.isCorrect) combi++;
            std::cout<<correct<<" / "<<count<<" "<<combi<<" CORRECT!!!!!!!!!!!!!!!!!"<<std::endl;
            correct++;

        }
        else if(!sudoku.hasIntegrity()) std::cout<<"FALSE\n"<<line<<std::endl;
        count++;
    }
#endif
}


void MainWindow::on_pushButton_clicked(){
    field.clear();
    ui->label->clear();
    for(auto i: clues){
        i->clear();
        i->setStyleSheet("QLineEdit { background-color: #303030 } ");
        i->setReadOnly(false);
    }
    for(auto i: pencil)
        i->clear();

    ui->button->setEnabled(true);
}

void MainWindow::updateClues(ussv field){
    int i = 0;
    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            if( field[y][x] != 0 ){
                clues[i]->setText( QString( std::to_string( field[y][x]).c_str() ) );
                pencil[i]->hide();
            }
            clues[i]->setReadOnly(true);
            i++;
        }
    }
}


void MainWindow::updatePencilxy(usssv fieldOptions){
    uint16_t i=0;
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {

            QString fill;
            for(auto i: fieldOptions[y][x])
                fill += std::to_string(i).c_str();
            pencil[i]->show();
            pencil[i]->setText( QString( fill ) );
            pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
            pencil[i]->setReadOnly(true);
            i++;
        }
    }
}

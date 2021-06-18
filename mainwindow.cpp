#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(483, 550);
    this->setWindowTitle("Dr. Kawashima's Sudoku Solver");

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

void MainWindow::on_button_clicked(){
    ui->button->setEnabled(false);
    std::ifstream fi("C:\\Users\\Whoami\\Documents\\PROJEKTE\\QT\\Sudoku\\SudokuOld\\Sudoku\\sudokus");
    std::ofstream fo("C:\\Users\\Whoami\\Documents\\PROJEKTE\\QT\\Sudoku\\SudokuOld\\Sudoku\\sudokus", std::ios::app);
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
    usv fieldList = Sudoku().getFieldlist( field );

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

    SudokuThread sudoku;

    sudoku.start(field, clues, pencil);

    if(sudoku.hasIntegrity(sudoku.getField())){
        ui->label->setText("Solved");
        ui->label->setStyleSheet("QLabel { color: green } ");
    }
    else if(!sudoku.hasIntegrity(sudoku.getField())){
        ui->label->setText("Unsolved");
        ui->label->setStyleSheet("QLabel { color: red } ");
    }


   /* std::ifstream list("C:\\Users\\Whoami\\Desktop\\sudokusfinal.txt");
    std::string line;
    uint64_t correct = 1, count = 1;

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



        SudokuThread sudoku;

        sudoku.start(field, clues, pencil);

        if(sudoku.hasIntegrity()) {
            std::cout<<correct<<" / "<<count<<" CORRECT!!!!!!!!!!!!!!!!!"<<std::endl;
            correct++;

        }
        else if(!sudoku.hasIntegrity()) std::cout<<"FALSE\n"<<line<<std::endl;
        count++;
    }*/

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

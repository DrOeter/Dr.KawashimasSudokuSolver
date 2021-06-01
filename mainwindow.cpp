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
    delete sudoku;
}

void MainWindow::on_button_clicked(){
    getGui();
    uint16_t it = 0;

    for(auto &i: field)
        for(auto ii: i){
            if( ii != 0 ) clues[it]->setStyleSheet("QLineEdit { background-color: #1f1f1f } ");
            it++;
        }

    sudoku = new Sudoku(field, clues, pencil);
}


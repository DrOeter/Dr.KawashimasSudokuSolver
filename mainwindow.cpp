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
        connect(clues[i], &QLineEdit::selectionChanged, clues[i], &QLineEdit::deselect);
        connect(pencil[i], &QLineEdit::selectionChanged, pencil[i], &QLineEdit::deselect);
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

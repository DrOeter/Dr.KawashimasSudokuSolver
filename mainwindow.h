#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QElapsedTimer>
#include "Sudoku.h"


typedef std::vector<QLineEdit*> Qline_v;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_clicked();

private:
    void getGui();

    void initGui();

    Ui::MainWindow *ui;
    Sudoku *sudoku;
    ussv field;
    Qline_v pencil, clues;
};



#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Sudoku.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject* object, QEvent* event) override;

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

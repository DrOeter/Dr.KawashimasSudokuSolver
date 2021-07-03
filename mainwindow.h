#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Sudoku.h"
#include <fstream>
#include <QMainWindow>
#include <QLineEdit>
#include <QElapsedTimer>

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

    bool eventFilter(QObject* object, QEvent* event) override;

private slots:
    void on_button_clicked();

    void on_pushButton_clicked();

private:
    void getGui();

    void initGui();

    void updateClues(ussv field);

    void updatePencilxy(usssv fieldOptions);

    Ui::MainWindow *ui;
    ussv field;
    Qline_v pencil, clues;
};
#endif // MAINWINDOW_H

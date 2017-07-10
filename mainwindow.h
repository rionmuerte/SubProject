#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SrtCell.h"
#include "SubCell.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum FileStyle{
        SRT,
        SUB,
        NONE
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_SelectFile_clicked();

    void on_SaveInput_clicked();

    void on_PrevButton_clicked();

    void on_NextButton_clicked();

    void on_InserButton_clicked();

    void on_DeleteButton_clicked();

    void on_SaveOutput_clicked();

    void on_SaveAll_clicked();

private:

    void ReadFromSRTFile(const char * filename, MainCell * begining );
    void ReadFromSUBFile(const char * filename, MainCell * begining );
    void RefreshDisplayed();
    void LoadData();
    bool FirstCondition() ;

    FileStyle DetermineFileType() ;

    QString InputFileName ;
    QString OutputFileName ;

    MainCell * InputFirst ;
    MainCell * InputCurrent ;
    MainCell * OutputFirst ;
    MainCell * OutputCurrent ;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

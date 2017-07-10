#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    InputFileName = "" ;
    OutputFileName = "" ;
    InputFirst = nullptr ;
    InputCurrent = nullptr ;
    OutputFirst = nullptr ;
    OutputCurrent = nullptr ;

    ui->setupUi(this);
    //ui->SelectFile->setStyleSheet(QString::fromUtf8("background-image: url(:Images\FileIconA.bmp;"));
}

MainWindow::~MainWindow()
{
    if(InputFirst!=nullptr) delete InputFirst ;
    if(OutputFirst!=nullptr) delete OutputFirst;
    delete ui;
}

void MainWindow::on_SelectFile_clicked()
{
        InputFileName = QFileDialog::getOpenFileName(this,"Select a file","","MicroDVD file (*.sub);;SubRip file (*.srt);;All files (*.*)");
        //qDebug() << "dziala "<< InputFileName[InputFileName.length()-2] ;
        try
        {
            switch (DetermineFileType()) {
            case SRT:
                if(InputFirst!=nullptr)
                {
                    delete InputFirst ;
                    delete OutputFirst ;
                }

                InputFirst = new SrtCell("","","");
                OutputFirst = new SrtCell("","","");
                ReadFromSRTFile(InputFileName.toLatin1(),InputFirst);
                break;
            case SUB:
                if(InputFirst!=nullptr)
                {
                    delete InputFirst ;
                    delete OutputFirst ;
                }

                InputFirst = new SubCell("","","");
                OutputFirst = new SubCell("","","");
                ReadFromSUBFile(InputFileName.toLatin1(),InputFirst);
                break;
            default:
                break;
            }

            InputCurrent = InputFirst ;
            OutputFirst->CopyTimeSnaps(InputFirst);
            OutputCurrent = OutputFirst ;
            RefreshDisplayed();
        }
        catch(std::string message)
        {
            QMessageBox::information(this,"Opening File Error",message.c_str()) ;
        }
}

void MainWindow::ReadFromSRTFile(const char *filename, MainCell *begining)
{
    std::fstream file ;
    file.open(filename,std::ios::in) ;
    if(file.is_open())
    {
        qDebug() << "plik otwarty" ;
        std::string temp ;
        std::getline(file,temp) ;
        qDebug() << "wczytana pierwsza linia";
        begining->ReadFromFile(&file);
        file.close() ;
    }
    else
    {
        std::string a = "Can't open file" ;
        throw a ;
    }
}

void MainWindow::ReadFromSUBFile(const char *filename, MainCell *begining)
{
    std::fstream file ;
    file.open(filename,std::ios::in) ;
    if(file.is_open())
    {
        qDebug() << "plik otwarty" ;
        std::string temp ;
        begining->ReadFromFile(&file);
        file.close() ;
    }
    else
    {
        std::string a = "Can't open file" ;
        throw a ;
    }

}

void MainWindow::RefreshDisplayed()
{
    ui->InputStart->setText(QString::fromStdString(InputCurrent->GetStartTime()));
    ui->InputStop->setText(QString::fromStdString(InputCurrent->GetStopTime()));
    ui->InputTextBox->setText(QString::fromStdString(InputCurrent->GetTextBox()));
    ui->OutputStart->setText(QString::fromStdString(OutputCurrent->GetStartTime()));
    ui->OutputStop->setText(QString::fromStdString(OutputCurrent->GetStopTime()));
    ui->OutputTextBox->setText(QString::fromStdString(OutputCurrent->GetTextBox()));
}

void MainWindow::LoadData()
{
    InputCurrent->SetStartTime(ui->InputStart->toPlainText().toStdString());
    InputCurrent->SetStopTime(ui->InputStop->toPlainText().toStdString());
    InputCurrent->SetTextBox(ui->InputTextBox->toPlainText().toStdString());
    OutputCurrent->SetStartTime(ui->OutputStart->toPlainText().toStdString());
    OutputCurrent->SetStopTime(ui->OutputStop->toPlainText().toStdString());
    OutputCurrent->SetTextBox(ui->OutputTextBox->toPlainText().toStdString());
}

bool MainWindow::FirstCondition()
{
    if(InputCurrent == nullptr || OutputCurrent == nullptr )
    {
        QMessageBox::information(this,"Data Error","\n\n       Can't complete operation       \n\n");
        return true ;
    }
    return false;
}

MainWindow::FileStyle MainWindow::DetermineFileType()
{
    switch (InputFileName[InputFileName.length()-2].toLatin1())
    {
        case 'r':
            return SRT ;
        case 'u':
            return SUB ;
    }
    return NONE ;
}

void MainWindow::on_SaveInput_clicked()
{
    if(FirstCondition())return ;
    LoadData();
    RefreshDisplayed();
    InputFileName = QFileDialog::getSaveFileName(this,"Set input file name and destination","","MicroDVD file (*.sub);;SubRip file (*.srt);;All files (*.*)");
    std::ofstream file ;
    file.open(InputFileName.toStdString().c_str());
    if(file.is_open())
    {
        InputFirst->WriteToFile(&file);
        file.close();
    }
    else
    {
         QMessageBox::information(this,"Saving File Error","Can't open file") ;
    }
}

void MainWindow::on_PrevButton_clicked()
{
    if(FirstCondition()) return ;
    if(InputCurrent->PrevItem() == nullptr)
    {
        QMessageBox::information(this,"Traverse Error","End of data\nCan't move further");
        return ;
    }
    LoadData();

    InputCurrent = InputCurrent->PrevItem() ;
    OutputCurrent = OutputCurrent->PrevItem() ;

    RefreshDisplayed();
}

void MainWindow::on_NextButton_clicked()
{
    if(FirstCondition()) return ;
    if(InputCurrent->NextItem() == nullptr)
    {
        QMessageBox::information(this,"Traverse Error","End of data\nCan't move further");
        return ;
    }
    LoadData();

    InputCurrent = InputCurrent->NextItem() ;
    OutputCurrent = OutputCurrent->NextItem() ;

    RefreshDisplayed();

}

void MainWindow::on_InserButton_clicked()
{
    if(FirstCondition()) return ;
    LoadData();

    InputCurrent->Insert() ;
    OutputCurrent->Insert() ;
    InputCurrent = InputCurrent->NextItem() ;
    OutputCurrent = OutputCurrent->NextItem() ;
    RefreshDisplayed();
}

void MainWindow::on_DeleteButton_clicked()
{
    if(FirstCondition()) return ;
    qDebug() << "nastepny"<<(InputCurrent->NextItem()) ;
    qDebug() << "poprzedni" << (InputCurrent->NextItem())<<"\n nullptr " << nullptr ;
    if(InputCurrent->NextItem()!=nullptr)
    {
        qDebug() << "next " ;
        on_NextButton_clicked();
        qDebug() << "refresh" ;
        InputCurrent->PrevItem()->Remove();
        qDebug() << "in del" ;
        OutputCurrent->PrevItem()->Remove();
        qDebug() << "out del ";
    }
    else
    {
        if(InputCurrent->PrevItem()!=nullptr)
        {
            qDebug() << "prev " ;
            on_PrevButton_clicked();
            InputCurrent->NextItem()->Remove();
            OutputCurrent->NextItem()->Remove();
        }
    }
}

void MainWindow::on_SaveOutput_clicked()
{
    if(FirstCondition())return ;
    LoadData();
    RefreshDisplayed();
    OutputFileName = QFileDialog::getSaveFileName(this,"Set output file name and destination","","MicroDVD file (*.sub);;SubRip file (*.srt);;All files (*.*)");
    std::ofstream file ;
    file.open(OutputFileName.toStdString().c_str());
    if(file.is_open())
    {
        OutputFirst->WriteToFile(&file);
        file.close();
    }
    else
    {
         QMessageBox::information(this,"Saving File Error","Can't open file") ;
    }
}

void MainWindow::on_SaveAll_clicked()
{
    on_SaveInput_clicked();
    on_SaveOutput_clicked();
}

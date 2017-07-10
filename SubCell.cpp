#include "SubCell.h"
#include "QDebug"

SubCell::SubCell(std::string Start, std::string Stop, std::string Text):MainCell(Start,Stop,Text)
{
    Number = 1 ;
    next = nullptr ;
    prev = nullptr ;
}

SubCell::~SubCell()
{
    //Tez nic nie robi bo i po co skoro rodzic wszystko niszczy? paskudni ci rodzice
}
void SubCell::Add(std::string Start, std::string Stop, std::string Text)
{
    if(this->next==nullptr)
    {
        SubCell *temp = new SubCell(Start,Stop,Text) ;
        temp->prev = this ;
        this->next = temp ;
    }
    else
    {
        this->next->Add(Start,Stop,Text) ;
    }
}
void SubCell::Remove()
{

    qDebug() << "this " << this << "\nnext " << next << "\n prev " << prev ;
    if(this->prev != nullptr) this->prev->SetNext(this->next);
    if(this->next!=nullptr) this->next->SetPrev(this->prev);
    this->next = nullptr ;
    this->prev = nullptr ;
    delete this ;
}

void SubCell::CopyTimeSnaps(MainCell *source)
{
    qDebug() << "tu zaczyna" ;
    if(source != nullptr)
    {
    this->StartTime = source->GetStartTime() ;
    this->StopTime = source->GetStopTime() ;
    this->TextBox = "" ;
    this->Add("","","");
    this->next->CopyTimeSnaps(source->NextItem());
    }
    else
    {
        return ;
    }

}

MainCell * SubCell::Insert()
{
    SubCell * temp = new SubCell("","","") ;
    temp->prev = this ;
    temp->next = this->next ;
    if(this->next!=nullptr) this->next->SetPrev(temp) ;
    this->next = temp ;
    return temp ;
}

void SubCell::WriteToFile(std::ofstream * OutputFile)
{
    qDebug() << "Poszedl sub" ;
    if(this->StopTime == "" || this->TextBox == "") return ;
    *OutputFile << '{'<< this->StartTime << "}{" << this->StopTime << '}' ;
    *OutputFile << this->TextBox << std::endl ;
    if(next!=nullptr) next->WriteToFile(OutputFile) ;
}
void SubCell::ReadFromFile(std::fstream * InputFile)
{
    std::string temp ;
    if(!InputFile->eof())
    {
        bool WriteToStart = true ;
        bool WriteToStop = false ;
        bool WriteToText = false ;
        std::getline(*InputFile,temp) ;
        for(int i = 0 ; i < temp.length() ; i++)
        {
            if(temp[i]=='{') i++ ;
            if(temp[i]=='}' && WriteToStart)
            {
                WriteToStart = false ;
                WriteToStop = true ;
                i+=2 ;
            }
            if(temp[i]=='}' && WriteToStop)
            {
                WriteToStop = false ;
                WriteToText = true ;
                i++ ;
            }
            if(WriteToStart) this->StartTime += temp[i] ;
            if(WriteToStop) this->StopTime += temp[i] ;
            if(WriteToText) this->TextBox += temp[i] ;
        }
        if(!InputFile->eof())
            {
                this->Add("","","") ;
                this->next->ReadFromFile(InputFile);
            }

    }
}

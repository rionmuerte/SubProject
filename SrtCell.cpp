#include "SrtCell.h"

#include "QDebug"


SrtCell::SrtCell(std::string Start, std::string Stop, std::string Text):MainCell(Start,Stop,Text)
{
    Number = 1 ;
    this->next = nullptr ;
    this->prev = nullptr ;
}

SrtCell::SrtCell(std::string Start, std::string Stop, std::string Text,unsigned int Nr):MainCell(Start,Stop,Text)
{
    Number = Nr ;
    next = nullptr ;
    prev = nullptr ;

}

SrtCell::~SrtCell()
{
    //Nic nie robi bo nie musi, cala reszta jest usuwana poprzez konstruktor klasy matki
}

void SrtCell::Add(std::string Start, std::string Stop, std::string Text)
{
    if(this->next==nullptr)
    {
        SrtCell *temp = new SrtCell(Start,Stop,Text,this->Number+1) ;
        temp->prev = this ;
        this->next = temp ;
    }
    else
    {
        this->next->Add(Start,Stop,Text) ;
    }
}

void SrtCell::Remove()
{
    this->prev->SetNext(this->next);
    this->next->SetPrev(this->prev);
    this->DecreaseAllNumbers() ;
    this->next = nullptr ;
    this->prev = nullptr ;
    delete this ;
}

void SrtCell::CopyTimeSnaps(MainCell *source)
{
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
MainCell * SrtCell::Insert()
{
    SrtCell * temp = new SrtCell("","","",this->Number+1) ;
    temp->prev = this ;
    temp->next = this->next ;
    if(this->next!=nullptr) this->next->SetPrev(temp) ;
    this->next = temp ;
    temp->IncreaseAllNumbers();
    return temp ;
}

void SrtCell::WriteToFile(std::ofstream * OutputFile)
{
    if(this->StopTime == "" || this->TextBox == "") return ;
    *OutputFile << this->Number << std::endl ;
    *OutputFile << this->StartTime << " --> " << this->StopTime << std::endl ;
    *OutputFile << this->TextBox << std::endl ;
    if(next!=nullptr) next->WriteToFile(OutputFile) ;
}
void SrtCell::ReadFromFile(std::fstream * InputFile)
{
    std::string temp ;
    if(!InputFile->eof())
    {
        std::getline(*InputFile,temp) ;
        int i = 0 ;
        bool Stop = false;
        bool WriteToStop = false;
        while(i<=temp.length())
        {
            if(temp[i] == ' ' && temp[i+1] == '-')
            {
                WriteToStop = true ;
                i+=4 ;
            }
            if(WriteToStop)
            {
                StopTime += temp[i] ;
            }
            else
            {
                StartTime += temp[i] ;
            }
            i++ ;
        }
        while(std::getline(*InputFile,temp))
        {
            Stop = true ;
            if(atoi(temp.c_str())==Number+1) break ;
            TextBox += temp ;
            TextBox += '\n' ;
        }
        if(!InputFile->eof())
        {
            qDebug() << "doszlo i tu" ;
            this->Add("","","") ;
            this->next->ReadFromFile(InputFile);
        }
    }
}

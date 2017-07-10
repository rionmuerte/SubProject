#ifndef MAINCELL_H
#define MAINCELL_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

class MainCell
{
    public:

        virtual void SetStartTime(std::string Time) {StartTime = Time;}
        virtual void SetStopTime(std::string Time) {StopTime = Time ;}
        virtual void SetTextBox(std::string Text) { TextBox = Text ; }

        virtual std::string GetStartTime() {return StartTime ;}
        virtual std::string GetStopTime()  {return StopTime ;}
        virtual std::string GetTextBox()  {return TextBox ;}
        virtual int GetNumber() {return Number;}

        virtual MainCell * Insert() = 0 ;
        virtual void Add(std::string Start, std::string Stop, std::string Text) = 0 ;
        virtual void Remove() = 0 ;

        MainCell * NextItem() {return next ;}
        MainCell * PrevItem() {return prev ;}
        void SetNext(MainCell * Next) {next = Next ;}
        void SetPrev(MainCell * Prev) {prev = Prev ;}

        virtual void WriteToFile(std::ofstream * OutputFile) = 0 ;
        virtual void ReadFromFile(std::fstream * InputFile) = 0 ;
        MainCell(std::string Start, std::string Stop, std::string Text):StartTime(Start),StopTime(Stop),TextBox(Text){}

        virtual void CopyTimeSnaps(MainCell * source) = 0 ;

        virtual void IncreaseAllNumbers()
        {
            if(this->next!=nullptr)
            {
                this->next->Number ++ ;
                this->next->IncreaseAllNumbers();
            }
        }

        virtual void DecreaseAllNumbers()
        {
            if(this->next!=nullptr)
            {
                this->next->Number -- ;
                this->next->DecreaseAllNumbers();
            }
        }

        MainCell() = delete ;
        virtual ~MainCell()
        {
            if(next != nullptr) delete next ;
        }

    protected:

        std::string StartTime ;
        std::string StopTime ;
        std::string TextBox ;

        unsigned int Number ;

        MainCell * next ;
        MainCell * prev ;

};

#endif // MAINCELL_H

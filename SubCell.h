#ifndef SUBCELL_H
#define SUBCELL_H

#include "MainCell.h"


class SubCell : public MainCell
{
    public:
        SubCell(std::string Start, std::string Stop, std::string Text);
        virtual ~SubCell();


        virtual void WriteToFile(std::ofstream * OutputFile) override;
        virtual void ReadFromFile(std::fstream * InputFile) override ;

        virtual MainCell * Insert() override ;
        virtual void Add(std::string Start, std::string Stop, std::string Text) override ;
        virtual void Remove() override ;

        virtual void CopyTimeSnaps(MainCell * source) override ;

};

#endif // SUBCELL_H

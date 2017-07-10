#ifndef SRTCELL_H
#define SRTCELL_H

#include "MainCell.h"


class SrtCell : public MainCell
{
    public:
        SrtCell(std::string Start, std::string Stop, std::string Text);
        virtual ~SrtCell();

        virtual void WriteToFile(std::ofstream * OutputFile) override;
        virtual void ReadFromFile(std::fstream * InputFile) override ;

        virtual MainCell * Insert() override ;
        virtual void Add(std::string Start, std::string Stop, std::string Text) override ;
        virtual void Remove() override ;

        virtual void CopyTimeSnaps(MainCell * source) override ;

    private:
        SrtCell(std::string Start, std::string Stop, std::string Text,unsigned int Nr);

};

#endif // SRTCELL_H

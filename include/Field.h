#ifndef FIELD_H
#define FIELD_H

#include <IField.h>

using namespace std;

// The class represents field implementation
class Field : public IField
{

    public:
        // Constructors:
        Field(int sizeRows, int sizeCols, int emptyTileValue = 0 );

        // IField members :
        virtual int  GetScore();
        virtual void SetScore(int score);
        virtual bool IsGameOver();
        virtual void SetGameIsOver();
        virtual int  GetSizeRows();
        virtual int  GetSizeCols();
        virtual int  GetDefaultTileValue();
        virtual int  GetTileValue(int r, int c);
        virtual void SetTileValue(int r, int c, int value);
        virtual void ResetTile(int r, int c);
        virtual void ResetField();

        virtual ~Field();
    protected:
    private:
        int score;
        int sizeRows;
        int sizeCols;
        int defaultTileValue;
        int* fieldTilesArr;
        bool isGameOver;
};

#endif // FIELD_H

#ifndef IFIELD_H
#define IFIELD_H

using namespace std;

// The class represents generic field interface
class IField
{
    public:
        virtual int  GetScore() = 0;
        virtual void SetScore(int score) = 0;
        virtual bool IsGameOver() = 0;
        virtual void SetGameIsOver() = 0;
        virtual int  GetSizeRows() = 0;
        virtual int  GetSizeCols() = 0;
        virtual int  GetDefaultTileValue() = 0;
        virtual int  GetTileValue(int r, int c) = 0;
        virtual void SetTileValue(int r, int c, int value) = 0;
        virtual void ResetTile(int r, int c)  = 0;
        virtual void ResetField()  = 0;
        virtual ~IField() {};
    protected:
    private:
};

#endif // IFIELD_H

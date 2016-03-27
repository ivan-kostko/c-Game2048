#ifndef GAME_H
#define GAME_H

#include <time.h>       /* time */

#include "IField.h"
#include "IGame.h"

using namespace std;

// The class represent "2048 like" game rules implementation
class Game : public IGame
{
    public:

        // Constructors:
        Game(IField* field, int value2 = 2, int value4 = 4, int winningGameValue = 2048);

        // IGame members
        virtual void  MakeTurn(Move::Tilt t);

        void  ApplyTurn(int rowDirection, int colDirection);
        bool  MoveTileInDirection(int rowDirection, int colDirection, int tileRow, int tileCol, bool mergeAllowed);
        bool  MergeTileInDirection(int rowDirection, int colDirection, int tileRow, int tileCol);
        void  PlaceNewRandomValueOnRandomEmptyTile();
        int   GenerateNewTileRandomValue();
        int   NextIntRand();
        int** GetAllTilesContaining(int value);
        int   CountTilesContaining(int value);
        virtual ~Game();

    protected:
    private:
        IField* field;
        int value2, value4, winningGameValue, moveCount;
        time_t timer = time(NULL);
        // the member is used at MakeTurn metod to signal, whether current turn is valid or not
        bool isCurrentTurnValid;
};

#endif // GAME_H

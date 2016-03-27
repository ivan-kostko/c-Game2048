#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Game.h"

using namespace std;


// Constructor
Game::Game(IField* field, int value2, int value4, int winningGameValue)
{
    this->field = field;
    this->value2 = value2;
    this->value4 = value4;
    this->winningGameValue = winningGameValue;
    this->moveCount = 0;
}

// Converts Move::Tilt into directions and Applies the turn
void Game::MakeTurn(Move::Tilt t)
{
    this->moveCount++;
    switch (t)
    {
        case Move::UP :
            this->ApplyTurn(-1 , 0);
            break;
        case Move::DOWN :
            this->ApplyTurn(1 , 0);
            break;
        case Move::LEFT :
            this->ApplyTurn(0 , -1);
            break;
        case Move::RIGHT :
            this->ApplyTurn(0 , 1);
            break;
        default :
                break;
    }

    // if there are no "empty" tiles after move - the game is over...
    if (this->CountTilesContaining(this->field->GetDefaultTileValue()) == 0)
    {
        this->field->SetGameIsOver();
    }

    // Check that some tiles were merged or moved. If there is no "empty space"
    if(this->isCurrentTurnValid || this->moveCount == 1)
        this->PlaceNewRandomValueOnRandomEmptyTile();

}

// Applys turn in direction.
void Game::ApplyTurn(int rowDirection, int colDirection)
{
    // we r pessimists...
    this->isCurrentTurnValid = false;

    // Calculate absolute value of directions. I used ^2 not to use math. If it would be allowed - math::abs() is preferable
    int absRowDirection = rowDirection*rowDirection;
    int absColDirection = colDirection*colDirection;

    // the only one direction should be set, and could have only 1 or -1 value.
    // So, the method invocation is valid ONLY when absRowDirection+absColDirection == 1
    if ( absRowDirection + absColDirection != 1 )
        return;

    // For x loop we do not care about order. Important is: whether we loop by columns or rows.
    int xStart = 0;

    // So, abs*Direction = 0 will exclude one part of sum, when another abs*Direction = 1 will keep needed value
    int xEnd = ( absRowDirection*this->field->GetSizeRows() )
                + ( absColDirection*this->field->GetSizeCols() );
    int xStep = 1;

    // For y loop it is important whether we loop ascending or descending.
    // Here we should loop tiles from destination side (for ex.: if tilt is to the left, then we should loop from max col to 0)
    // abs*Direction = 0 excludes whether rows or columns:
    // when abs*Direction = 1, ((1+rowDirection)/2) will be 1 when dirction = 1 (ascending loop) or otherwise 0 (for descending loop)
    // (this->field->GetSize*() - 1) sets number of loops
    int yStart = (absRowDirection*((1+rowDirection)/2)*(this->field->GetSizeRows() - 1))
                    + (absColDirection*((1+colDirection)/2)*(this->field->GetSizeCols() - 1));
    // The same as yStart, except ((1-rowDirection)/2) will be 0 when dirction = 1 (ascending loop) or otherwise 1 (for descending loop)
    int yEnd = (absRowDirection*((1-rowDirection)/2)*(this->field->GetSizeRows() - 1))
                    + (absColDirection*((1-colDirection)/2)*(this->field->GetSizeCols() - 1));;
    // should loop against to direction
    int yStep = -(rowDirection + colDirection);

    bool mergeAllowed;
    int  tileRow;
    int  tileCol;
    for (int x = xStart; x < xEnd; x+=xStep)
    {
        // on a new row or column merge should be allowed again
        mergeAllowed = true;

        // on destination edge tiles wont move.
        // so, we could start loop from next position
        for(int y = yStart + yStep; (yStep) * y <= yEnd; y+=yStep)
        {
            tileRow = x*absColDirection + y*absRowDirection;
            tileCol = y*absColDirection + x*absRowDirection;

            // perform move only when tile is not empty. Otherwise it makes no sense and could unjustifiably change mergeAllowed value
            if (this->field->GetTileValue(tileRow, tileCol) != this->field->GetDefaultTileValue())
            {
                // if this one is merged, then next is not allowed to be merged
                mergeAllowed = (!this->MoveTileInDirection(rowDirection, colDirection, tileRow, tileCol, mergeAllowed));
            }
        }
    }
}

// Moves tile to next to non empty. returnes: true if tile was merged; false not merged.
bool Game::MoveTileInDirection(int rowDirection, int colDirection, int tileRow, int tileCol, bool mergeAllowed)
{
    bool merged = false;
    // Checkvalid direction. If directions are 0s = nothing to move
    if (rowDirection == 0 && colDirection == 0)
        return merged;

    int destinationRow = tileRow + rowDirection;
    int destinationCol = tileCol + colDirection;
    int fieldDefaultValue = this->field->GetDefaultTileValue();
    // check that current and next position are still within field range
    if (0 <= destinationRow && destinationRow < this->field->GetSizeRows()
        && 0 <= destinationCol && destinationCol < this->field->GetSizeCols()
        && 0 <= tileRow && tileRow < this->field->GetSizeRows()
        && 0 <= tileCol && tileCol < this->field->GetSizeCols() )
    {
        // So, both current and destination are within field range
        int currentTileValue = this->field->GetTileValue(tileRow, tileCol);
        int destinationTileValue = this->field->GetTileValue(destinationRow, destinationCol);

        // Move/merge only when current is not empty
        if (currentTileValue != fieldDefaultValue)
        {
            // if destination is "empty", move value to destination and reset current
            if (destinationTileValue == fieldDefaultValue)
            {
                // So, next position is within field range and is "empty"
                // move value to next position
                this->field->SetTileValue(destinationRow, destinationCol, currentTileValue);
                // the tile could be reset, cause destination tile was empty (with default value)
                this->field->ResetTile(tileRow, tileCol);
                merged = MoveTileInDirection( rowDirection, colDirection, destinationRow, destinationCol, mergeAllowed );
                // Since we've moved tile :
                this->isCurrentTurnValid = true;

            }
            else
            {
                // Next position is within field range and is not "empty"
                // If so, lets try to merge if allowed
                if (mergeAllowed)
                {
                    merged = this->MergeTileInDirection(rowDirection, colDirection, tileRow, tileCol);
                }
            }
        }
    }

    return merged;
}

// Merges tile with next non empty(if they r equal). Returnes true if merged; false not merged
bool Game::MergeTileInDirection(int rowDirection, int colDirection, int tileRow, int tileCol)
{
    bool merged = false;
    int nextPositionRow = tileRow + rowDirection;
    int nextPositionCol = tileCol + colDirection;
    // check that current and next position is still within field range
    if (0 <= nextPositionRow && nextPositionRow < this->field->GetSizeRows()
        && 0 <= nextPositionCol && nextPositionCol < this->field->GetSizeCols()
        && 0 <= tileRow && tileRow < this->field->GetSizeRows()
        && 0 <= tileCol && tileCol < this->field->GetSizeCols() )
    {
        // So, both current and destination are within field range
        int currentTileValue = this->field->GetTileValue(tileRow, tileCol);
        int nextTileValue = this->field->GetTileValue(nextPositionRow, nextPositionCol);

        if (nextTileValue == currentTileValue)
        {
            // So, next position is within field range and could be merged
            // merge to next position
            this->field->SetTileValue(nextPositionRow, nextPositionCol, nextTileValue + currentTileValue);
            // reset original position tile
            this->field->ResetTile(tileRow, tileCol);
            // increase the score
            this->field->SetScore(this->field->GetScore() + this->field->GetTileValue(nextPositionRow, nextPositionCol));
            // check if merged tile value has reached winning game value. If so, the game is over.
            if (this->field->GetTileValue(nextPositionRow, nextPositionCol) == this->winningGameValue)
            {
                this->field->SetGameIsOver();
            }
            merged = true;
            // Since we've merged tile...
            this->isCurrentTurnValid = true;
        }
    }
    return merged;
}

// Returns new random value. According to game rules: in 90%% = 2, the rest = 4
int Game::GenerateNewTileRandomValue()
{
    // Due to (rand() % 101) is from 0..100
    if (NextIntRand() % 101 >= 90)
    {
        return this->value4;
    }
    else
    {
        return this->value2;
    }
}

// Generates new random value
int Game::NextIntRand()
{
    // ITs a hot fix to make rand() more random on frequent requests.
    time_t t = time(NULL);
    if (t <= this->timer)
    {
        t = ++this->timer;
    };
    srand (t);
    int r = rand();
    return r;
}

// Places new random value into random empty tile
void Game::PlaceNewRandomValueOnRandomEmptyTile()
{
    // number of tiles containing default value
    int defaultValue = this->field->GetDefaultTileValue();
    int n = this->CountTilesContaining(defaultValue);

    if (n <= 0)
    {
        // It means that whether we devide by zero, or won't reach position.
        return;
    }
    // n != 0
    int x = (NextIntRand() % (n));
    int newValue = GenerateNewTileRandomValue();

    // Set new random genetated value to xth tile initially containing default value
    int SizeRows = this->field->GetSizeRows();
    int SizeCols = this->field->GetSizeCols();

    // loop over Rows and Columns of field
    for (int r = 0; r < SizeRows; r++)
    {
        for(int c = 0; c < SizeCols; c++)
        {
            if (this->field->GetTileValue(r, c) == defaultValue)
                // it is random place, so we don"t really care about ASC/DESC order
                n--;

            if (n == x)
            {
                // So, we"ve found x-th "empty" tile
                this->field->SetTileValue(r, c, newValue);
                break;
            }
        }
        if (n == x)  break;
    }
}

// Returns number of tiles on this->field containing vale v.
int Game::CountTilesContaining(int value)
{
    // contain current count
    int n = 0;
    int SizeRows = this->field->GetSizeRows();
    int SizeCols = this->field->GetSizeCols();

    // loop over Rows and Columns of field
    for (int r = 0; r < SizeRows; r++)
    {
        for(int c = 0; c < SizeCols; c++)
        {
            if (this->field->GetTileValue(r, c) == value)
                n++;
        }
    }
    return n;
}

Game::~Game()
{
    delete this->field;
}

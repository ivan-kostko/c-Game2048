#include <iostream>
#include <malloc.h>

#include "Field.h"

using namespace std;

// Constructor
Field::Field(int sizeRows, int sizeCols, int defaultTileValue)
{
    this->defaultTileValue = defaultTileValue;
    this->sizeRows = sizeRows;
    this->sizeCols = sizeCols;
    this->ResetField();
    this->isGameOver = false;
};

// Gets tile pointer. Numerating from 0. Returns Default if requested tile is out of the field.
// However,it is clients duty to ensure - requested tile is within field range
int Field::GetTileValue(int r, int c)
{
    // The method returns pointer just to keep using code more readable: *GetTile( r,  c) = z;

    // Check if requested row and column are in range of the field
    if ( (0 <= r && r < this->sizeRows)
            && (0 <= c && c < this->sizeCols)
        )
    {
        return fieldTilesArr[ r*(this->sizeRows) + c ];
    }
    return this->defaultTileValue;
}

// Tile value setter
void Field::SetTileValue(int r, int c, int value)
{
    // check tile is within field range
    if ( (0 <= r && r < this->sizeRows)
            && (0 <= c && c < this->sizeCols) )
    {
        this->fieldTilesArr[ r*(this->sizeRows) + c ] = value;
    }

}
// Returns field size rows number
int Field::GetSizeRows()
{
    return this->sizeRows;
}

// Returns field size columns number
int Field::GetSizeCols()
{
    return this->sizeCols;
}

// Returns default Tile value (value on reset)
int Field::GetDefaultTileValue()
{
    return this->defaultTileValue;
}


// Resets all field tiles values to this->valueinitializationTileValue
void Field::ResetField()
{
    this->score = 0;
    this->fieldTilesArr = new int[this->sizeRows*this->sizeCols];
    // ResetTile could be used in loop as well. Yet keep as is.
    std::fill_n(this->fieldTilesArr, this->sizeRows*this->sizeCols, this->defaultTileValue);
}

// Resets tile value to initial/default tile value.
void Field::ResetTile(int r, int c)
{
    this->fieldTilesArr[ r*(this->sizeRows) + c ] = this->defaultTileValue;
}

// Get game over status
bool Field::IsGameOver()
{
    return this->isGameOver;
}

// setter for game over status
void Field::SetGameIsOver()
{
    this->isGameOver = true;
}

// Get current score
int Field::GetScore()
{
    return this->score;
}

// setter for score
void Field::SetScore(int score)
{
    this->score = score;
}


Field::~Field()
{
    delete [] this->fieldTilesArr;
}

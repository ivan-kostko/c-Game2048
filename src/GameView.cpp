#include <iostream>
#include "GameView.h"

using namespace std;

// Constructor.
GameView::GameView(IField * field, IGame * game, std::map<char, Move::Tilt> charToTiltMapping)
{
    this->field = field;
    this->game = game;
    this->charToTiltMapping = charToTiltMapping;
    this->isStopped = false;
}

// Renders field data.
void GameView::Render()
{
    int rows = this->field->GetSizeRows();
    int cols = this->field->GetSizeCols();
    cout << endl;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            cout << this->field->GetTileValue(r,c) << "   \t";
        }
        cout << endl;
    }
    cout << "Current score is : " << this->field->GetScore() << endl;
}

// Runs in a loop rendering and press key handling while game is not stopped or over
void GameView::Run()
{
    while (!this->field->IsGameOver() && !this->isStopped)
    {
        this->Render();
        this->HandlePressedButton();
    }
}

// Reads pressed button. If button mapped to Game turn - calls it.
void GameView::HandlePressedButton()
{
    cout << "Press 'z' to Stop the game;" << endl;
    char pressed;

    // Have not found more apropreate way to read pressed button
    std::string line;
    getline(std::cin, line);
    pressed = line[0];

    // z and r are defined as hardcode...
    switch (pressed) {
        case 'r' : this->Render();
            break;
        case 'z' : this->isStopped = true;
            break;
        default :
            if ( this->charToTiltMapping.count(pressed) )
                this->game->MakeTurn(this->charToTiltMapping[pressed]);
            break;
    }
}

//d-tor
GameView::~GameView()
{
    delete this->field;
    delete this->game;
}

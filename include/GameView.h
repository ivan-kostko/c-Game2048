#include <map>

#include "IField.h"
#include "IGame.h"
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

using namespace std;

class GameView
{
    public:
        GameView(IField * field, IGame * game, std::map<char, Move::Tilt> charToTiltMapping);
        void Render();
        void HandlePressedButton();
        void Run();
        ~GameView();
    protected:
    private:
        IField * field;
        IGame * game;
        std::map<char, Move::Tilt> charToTiltMapping;
        bool isStopped;
};

#endif // GAMEVIEW_H

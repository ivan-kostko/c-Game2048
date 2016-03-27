#ifndef IGAME_H
#define IGAME_H

using namespace std;

// The enum containing move types
struct Move
{
    enum Tilt
        {
            UP,
            DOWN,
            LEFT,
            RIGHT
        };
 };

// The class represents game controller interface
class IGame
{
    public:
        virtual void MakeTurn(Move::Tilt t) = 0;

        virtual ~IGame() {};

    protected:
    private:
};

#endif // IGAME_H

#include <Game.h>
#include <GameView.h>
#include <Field.h>

using namespace std;

int main()
{
    // Instantiate new field.
    Field * f = new Field( 4, 4, 0);

    // Instantiate new game for the field
    Game * g = new Game(f);

    // Map characters to Tilt`s
    std::map<char, enum Move::Tilt> charToTiltMapping;
    charToTiltMapping.insert(std::make_pair('w', Move::UP));
    charToTiltMapping.insert(std::make_pair('a', Move::LEFT));
    charToTiltMapping.insert(std::make_pair('s', Move::DOWN));
    charToTiltMapping.insert(std::make_pair('d', Move::RIGHT));

    GameView * gv = new GameView(f,g, charToTiltMapping);
    gv->Run();

    delete gv;
    delete g;
    delete f;
    return 0;
}

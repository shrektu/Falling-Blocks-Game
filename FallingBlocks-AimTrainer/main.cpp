#include <iostream>
#include "Game.h"




int main()
{
    // Inicjalizacja generatora liczb losowych
    srand(static_cast<unsigned>(time(NULL)));

    // Inicjalizacja silnika gry z klasy
    Game game;
    
    // Pêtla gry
    while (game.window_is_open() && !game.is_game_over())
    {
        // Aktualizacja stanu gry
        game.update();


        // Renderowanie stanu gry
        game.render();
    }


    // Koniec programu
    return 0;
}

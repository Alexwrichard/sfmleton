#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>

#include "gamestatemanager.h"
#include "teststate.h"
using namespace std;


queue<sf::Event> getInput( sf::RenderWindow &app );


int main( int argc, char *argv[] ) {
    game::state::manager man;
    sf::RenderWindow app;

    sf::VideoMode vmode = sf::VideoMode::getDesktopMode();
    app.create( vmode, "sfmleton");

    if( !app.isOpen() ) {
        cerr << "Error creating SFML renderwindow" << endl;
        return EXIT_FAILURE;
    }

    man.push( new game::state::test );

    while( !man.empty() and app.isOpen() ) {
        // get input/window events
        queue<sf::Event> events = getInput(app);

        // update state
        man.top().update( events, man );

        if( man.empty() )
            break;

        // window should be open
        if( !app.isOpen() ) {
            clog << "SFML RenderWindow not open." << endl;
            break;
        }

        // render state
        man.top().render( app );
    }

    return EXIT_SUCCESS;
}


queue<sf::Event> getInput( sf::RenderWindow &app ) {
    queue<sf::Event> queue;

    sf::Event e;
    while( app.pollEvent(e) )
        queue.push( e );

    return queue;
}

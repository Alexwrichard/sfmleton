#include <iostream>
#include "teststate.h"
#include "resourcemanager.h"
using namespace std;



void game::state::test::init() {
}

void game::state::test::cleanup() {
}


void game::state::test::pause() {
    Resource_Manager man;
    
    vector<std::string> res(6);
    
    res[0] = "Resources/21_21.jpg";
    res[1] = "Resources/20_20.jpg";
    res[2] = "Resources/22_22.jpg";
    res[2] = "Resources/19_19.jpg";
    res[3] = "Resources/18_18.jpg";
    res[4] = "Resources/17_17.jpg";
    res[5] = "Resources/22_22.jpg";
    
    man.set_memory_size(10000);
    man.create(res);
    
}

void game::state::test::resume() {
}


void game::state::test::update( std::queue<sf::Event> events, state::manager &man ) {
    while( !events.empty() ) {
        sf::Event e = events.front();

        if( e.type == sf::Event::KeyPressed )
            switch( e.key.code ) {
                case sf::Keyboard::Escape:
                    man.pop();
                    break;

                case sf::Keyboard::Return:
                    man.push( new game::state::test );
                    break;

                default:
                    break;
            }

        events.pop();
    }
}

void game::state::test::render( sf::RenderWindow &app ) {
    app.display();
}

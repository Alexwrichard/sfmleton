#include <iostream>
#include "teststate.h"

using namespace std;

void game::state::test::init() {
    
    if(!res_man.exists("BigPlayer.png"))
        res_man.create("BigPlayer.png");
    
    PlayerImage = res_man.get("BigPlayer.png");
    PlayerTexture.loadFromMemory(PlayerImage.buf, PlayerImage.buf_size);
    Player.setTexture(PlayerTexture);
    Player.setPosition(10, 50);
    
//-------------------//
    if(!res_man.exists("BigPlayerHitbox.png"))
        res_man.create("BigPlayerHitbox.png");
    
    PlayerImage = res_man.get("BigPlayerHitbox.png");
    PlayerHitboxTexture.loadFromMemory(PlayerImage.buf, PlayerImage.buf_size);
    PlayerHitbox.setTexture(PlayerHitboxTexture);
    PlayerHitbox.setPosition(10, 34 + Player.getGlobalBounds().height);
    
//-------------------//
    if(!res_man.exists("greenbox.png"))
        res_man.create("greenbox.png");
    
    BoxImage = res_man.get("greenbox.png");
    BoxTexture.loadFromMemory(BoxImage.buf, BoxImage.buf_size);
    Box1.setTexture(BoxTexture);
    Box2.setTexture(BoxTexture);
    Box3.setTexture(BoxTexture);
    
    Box1.setPosition(550, 400);
    Box2.setPosition(550, 528);
    Box3.setPosition(614, 464);
    
    col.make_group("solid");
    
    col.add_to_group(Box1, "solid");
    col.add_to_group(Box2, "solid");
    col.add_to_group(Box3, "solid");
    
    move_speed = 1.f;
    
    Time = Clock.restart();
}

void game::state::test::cleanup() {
    
}

void game::state::test::pause() {
    
}

void game::state::test::resume() {
}

void game::state::test::update( std::queue<sf::Event> events, state::manager &man ) {
        
        sf::Vector2f movement(0.f, 0.f);
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (!col.check_collision_top(PlayerHitbox, "solid")) {
                movement.y -= move_speed;
            }
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (!col.check_collision_left(PlayerHitbox, "solid")) {
            movement.x -= move_speed;
            }
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (!col.check_collision_bottom(PlayerHitbox, "solid")) {
                movement.y += move_speed;
            }
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!col.check_collision_right(PlayerHitbox, "solid")) {
                movement.x += move_speed;
            }
        }
    
        Player.move(movement * TimePerFrame.asSeconds());
        PlayerHitbox.move(movement * TimePerFrame.asSeconds());
    
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

    app.clear();
    app.draw(Box1);
    app.draw(Box2);
    app.draw(Box3);
    app.draw(Player);

    app.display();
}

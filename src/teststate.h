#ifndef TESTSTATE_H
#define TESTSTATE_H
#include "gamestatemanager.h"
#include "resourcemanager.h"
#include "collision.h"

namespace game {
    namespace state {
        class test : public game::state::prototype {
            sf::Vector2f movement();
            
            collision col;
            
            sf::Sprite Player;
            sf::Sprite PlayerHitbox;
            
            sf::Sprite Box1;
            sf::Sprite Box2;
            sf::Sprite Box3;
            
            sf::Texture PlayerTexture;
            sf::Texture PlayerHitboxTexture;
            sf::Texture BoxTexture;
            
            sf::Time Time;
            sf::Time timeSinceLastUpdate;
            sf::Time TimePerFrame;
            
            sf::Clock Clock;
            
            Resource_Manager res_man;
            
            Blob PlayerImage;
            Blob BoxImage;

            int move_speed;
            
        public:
            void init();
            void cleanup();

            void pause();
            void resume();

            void update( std::queue<sf::Event> events, state::manager &man );
            void render( sf::RenderWindow &app );
        };
    };
};

#endif /* TESTSTATE_H */

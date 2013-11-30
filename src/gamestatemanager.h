#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H
#include <stack>
#include <queue>
#include <SFML/Graphics.hpp>

namespace game {
    namespace state {

        class prototype;


        class manager {
            std::stack<prototype*> states;

        public:
            void push( prototype *ptr );
            void pop();
            void swap( prototype *ptr );

            bool empty();
            size_t size();

            prototype & top();

            manager() {}
            manager( prototype *ptr );

            // we have a deleted copy constructor,
            // because copying this makese no sense.
            manager( const manager &x ) = delete;
        };


        class prototype {
            // this class is where all states should inherit from.
        public:
            virtual void init()    = 0;
            virtual void cleanup() = 0;

            virtual void pause()   = 0;
            virtual void resume()  = 0;

            virtual void update( std::queue<sf::Event> events, state::manager &man ) = 0;
            virtual void render( sf::RenderWindow &app ) = 0;
        };

    }
}

#endif /* GAMESTATEMANAGER_H */
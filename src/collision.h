//
//  collision.h
//  sfml
//
//  Created by Ozymandias on 1/7/14.
//  Copyright (c) 2014 Ozyma. All rights reserved.
//

#ifndef __sfml__collision__
#define __sfml__collision__

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <vector>

struct group {
    std::vector<sf::Sprite> sprite_list;
    
};

struct collision {
    std::map<std::string, group> group_map;
    
    void make_group(std::string group_name);
    void clear_group(std::string group_name);

    void add_to_group(sf::Sprite Sprite, std::string group_name);
    void remove_from_group(sf::Sprite Sprite, std::string group_name);

    bool check_collision_top(sf::Sprite Sprite1, sf::Sprite Sprite2);
    bool check_collision_left(sf::Sprite Sprite1, sf::Sprite Sprite2);
    bool check_collision_right(sf::Sprite Sprite1, sf::Sprite Sprite2);
    bool check_collision_bottom(sf::Sprite Sprite1, sf::Sprite Sprite2);
    
    bool check_collision_top(sf::Sprite Sprite, std::string group_name);
    bool check_collision_left(sf::Sprite Sprite, std::string group_name);
    bool check_collision_right(sf::Sprite Sprite, std::string group_name);
    bool check_collision_bottom(sf::Sprite Sprite, std::string group_name);
};

#endif /* defined(__sfml__collision__) */

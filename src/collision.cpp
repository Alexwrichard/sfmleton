//
//  collision.cpp
//  sfml
//
//  Created by Ozymandias on 1/7/14.
//  Copyright (c) 2014 Ozyma. All rights reserved.
//

#include "collision.h"

void collision::make_group(std::string group_name) {
    if(group_map.find(group_name) == group_map.end())
        group_map[group_name] = *new group;
}

void collision::add_to_group(sf::Sprite Sprite, std::string group_name) {
    if(group_map.find(group_name) == group_map.end())
        make_group(group_name);
    
    group_map[group_name].sprite_list.push_back(Sprite);
}


bool collision::check_collision_top(sf::Sprite Sprite1, sf::Sprite Sprite2) {
    float top1 = Sprite1.getGlobalBounds().top;
    float left1 = Sprite1.getGlobalBounds().left;
    float left2 = Sprite2.getGlobalBounds().left;
    float right1 = Sprite1.getGlobalBounds().left + Sprite1.getGlobalBounds().width;
    float right2 = Sprite2.getGlobalBounds().left + Sprite2.getGlobalBounds().width;
    float bottom1 = Sprite1.getGlobalBounds().top + Sprite1.getGlobalBounds().height;
    float bottom2 = Sprite2.getGlobalBounds().top + Sprite2.getGlobalBounds().height;
    
    if (left1 + 1 <= right2 && right1 - 1 >= left2 && top1 <= bottom2 && bottom1 >= bottom2 )
        return true;
    
    return false;
}

bool collision::check_collision_left(sf::Sprite Sprite1, sf::Sprite Sprite2) {
    float top1 = Sprite1.getGlobalBounds().top;
    float top2 = Sprite2.getGlobalBounds().top;
    float left1 = Sprite1.getGlobalBounds().left;
    float right1 = Sprite1.getGlobalBounds().left + Sprite1.getGlobalBounds().width;
    float right2 = Sprite2.getGlobalBounds().left + Sprite2.getGlobalBounds().width;
    float bottom1 = Sprite1.getGlobalBounds().top + Sprite1.getGlobalBounds().height;
    float bottom2 = Sprite2.getGlobalBounds().top + Sprite2.getGlobalBounds().height;
    
    if (top1 +1 <= bottom2 && bottom1 -1 >= top2 && left1 <= right2 && right1 >= right2)
        return true;

    
    return false;
}

bool collision::check_collision_right(sf::Sprite Sprite1, sf::Sprite Sprite2) {
    float top1 = Sprite1.getGlobalBounds().top;
    float top2 = Sprite2.getGlobalBounds().top;
    float left1 = Sprite1.getGlobalBounds().left;
    float left2 = Sprite2.getGlobalBounds().left;
    float right1 = Sprite1.getGlobalBounds().left + Sprite1.getGlobalBounds().width;
    float bottom1 = Sprite1.getGlobalBounds().top + Sprite1.getGlobalBounds().height;
    float bottom2 = Sprite2.getGlobalBounds().top + Sprite2.getGlobalBounds().height;
    
    if (top1 + 1 <= bottom2 && bottom1 -1 >= top2 && right1 >= left2 && left1 <= left2) {
        return true;
    }
    
    return false;
}

bool collision::check_collision_bottom(sf::Sprite Sprite1, sf::Sprite Sprite2) {
    float top1 = Sprite1.getGlobalBounds().top;
    float top2 = Sprite2.getGlobalBounds().top;
    float left1 = Sprite1.getGlobalBounds().left;
    float left2 = Sprite2.getGlobalBounds().left;
    float right1 = Sprite1.getGlobalBounds().left + Sprite1.getGlobalBounds().width;
    float right2 = Sprite2.getGlobalBounds().left + Sprite2.getGlobalBounds().width;
    float bottom1 = Sprite1.getGlobalBounds().top + Sprite1.getGlobalBounds().height;
    
    if (left1 + 1 <= right2 && right1 - 1 >= left2 && bottom1 >= top2 && top1 <= top2 )
        return true;
    
    return false;
}

bool collision::check_collision_top(sf::Sprite Sprite, std::string group_name) {
    int i=0;
    
    while(i <= group_map[group_name].sprite_list.size()) {
        if(check_collision_top(Sprite, group_map[group_name].sprite_list[i]) == true)
            return true;
        i++;
    }
    
    return false;
}

bool collision::check_collision_left(sf::Sprite Sprite, std::string group_name) {
    int i=0;
    
    while(i <= group_map[group_name].sprite_list.size()) {
        if(check_collision_left(Sprite, group_map[group_name].sprite_list[i]) == true)
            return true;
        i++;
    }
    
    return false;
}

bool collision::check_collision_right(sf::Sprite Sprite, std::string group_name) {
    int i=0;
    
    while(i <= group_map[group_name].sprite_list.size()) {
        if(check_collision_right(Sprite, group_map[group_name].sprite_list[i]) == true)
            return true;
        i++;
    }
    
    
    return false;
}

bool collision::check_collision_bottom(sf::Sprite Sprite, std::string group_name) {
    int i=0;
    
    while(i <= group_map[group_name].sprite_list.size()) {
        if(check_collision_bottom(Sprite, group_map[group_name].sprite_list[i]) == true)
            return true;
        i++;
    }
    
    return false;
}

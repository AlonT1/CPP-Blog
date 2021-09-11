#include "Player.h"
#include <iostream>

Player::Player(int life, const char* name) : life(life), name(name) {} //ctor defintion
void Player::move() { std::cout << "move" << std::endl; } // move() definition

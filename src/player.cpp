#include <iostream>
#include "player.h"
#include <unistd.h>


Player::Player(int _id):id(_id){
    std::cout << "Create player with id:"<< id << std::endl;
}

Player::~Player(){
    std::cout << "Destruyendo player" << std::endl;
}


void Player::present() {
    std::cout << "Mi id es:"<<id<<std::endl;
}


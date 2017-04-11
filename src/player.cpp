#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>


Player::Player(int _id, Semaforo *waitForACard, Semaforo * endOfTurnGathering, int numberOfPlayers) : id(_id){
    this->waitForACard = waitForACard;
    this->endOfTurnGathering = endOfTurnGathering;
    this->numberOfPlayers = numberOfPlayers;

    sharedMemory.crear(FILE_CONCUDARING,KEY_MEMORY);
}

Player::~Player(){
    std::cout << "Destruyendo player" << std::endl;
}


void Player::present() {
    std::cout << "Mi id es:"<< id << std::endl;
}

bool Player::checkWinner() const {
  Game_t game = this->sharedMemory.leer();
  return game.gameFinished;
}

void Player::play() {
    std::cout << "play" << std::endl;
    Table& table = Table::getInstance();
    int turno = 0;
    while ( turno <= 5 ){
        if (itIsMyTurn(turno)){
            std::cout << "Es mi turno:" << id << "!!!!\n";
            std::cout << "Voy a dormir un rato antes de poner la carta.. zzzz" << "\n";
            sleep(2);
            table.putHand(id);
        }else{
            table.printCards(id);
        }
        turno++;
    }
}

bool Player::itIsMyTurn(int turnNumber) {
    int nextToPlay = turnNumber % numberOfPlayers;
        return nextToPlay == this->id;
}

void Player::setDeckOfCards(DeckOfCards deck) {
    myDeckOfCards = deck;
}


#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>
#include <sys/types.h>

Player::Player(int _id, Semaforo *waitForACard, Semaforo * endOfTurnGathering, int numberOfPlayers) : id(_id),numberOfPlayers(numberOfPlayers){
    this->waitForACard = waitForACard;
    this->endOfTurnGathering = endOfTurnGathering;
    sharedMemory.create(FILE_CONCUDARING,KEY_MEMORY,1);
}


Player::Player(int _id, int _numberOfPlayers):id(_id),numberOfPlayers(_numberOfPlayers){
  std::cout << "Se creo un player con el id:"<< getpid() << "\n";
}
Player::~Player(){
  std::cout << "Se destruye un player con el id:"<< getpid() << "\n";

}


void Player::present() const {
    std::cout << "Mi id es:"<< id << std::endl;
}

bool Player::checkWinner() const {
  Game_t game = this->sharedMemory.read();
  return game.gameFinished;
}

void Player::play() {
    std::cout << "play" << std::endl;
    Table& table = Table::getInstance();
    int turno = 0;
    while ( turno <= 10 ){
        if (itIsMyTurn(turno)){
            //std::cout << "Es mi turno:" << id << "!!!!\n";
            //std::cout << "Voy a dormir un rato antes de poner la carta.. zzzz" << "\n";
            sleep(2);
            int card = myDeckOfCards.getCard();
            table.putCard(card);
        }else{
            table.printCards(id);
        }
        turno++;
    }
}

bool Player::itIsMyTurn(int turnNumber) const {
    int nextToPlay = turnNumber % numberOfPlayers;
    return nextToPlay == this->id;
}

void Player::setDeckOfCards(DeckOfCards& deck) {
    myDeckOfCards = deck;
}

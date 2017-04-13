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
    //std::cout << "Mi id es:"<< id << std::endl;
}

bool Player::checkWinner() const {
  Game_t game = this->sharedMemory.read();
  return game.gameFinished;
}

void Player::play() {
    //std::cout << "play" << std::endl;
    Table& table = Table::getInstance();
    int turno = 0;
    while ( turno <= 2 ){
        if (itIsMyTurn(turno)){
            sleep(2);
            int card = myDeckOfCards.getCard();
            table.putCard(card);
        }
        //En este deck SOLO obtengo las 2 ultima cartas de la mesa
        DeckOfCards deck = table.getLastTwoCards();
        if (deck.theCardsAreSame() or deck.at() == 7){
            table.putHand(id);
        }
        int idLosser = table.getIdLosser();
        if (idLosser == id){
            DeckOfCards deck = table.getCards();
            myDeckOfCards =myDeckOfCards + deck;
        }
        //turno++;
        increaseTurn(turno);
    }
}

bool Player::itIsMyTurn(int turnNumber) const {
    int nextToPlay = turnNumber % numberOfPlayers;
    return nextToPlay == this->id;
}

void Player::setDeckOfCards(DeckOfCards& deck) {
    myDeckOfCards = deck;
}

int Player::increaseTurn(int turn) {
    if ( endOfTurnGathering->numberOfProcessesWaiting() == numberOfPlayers -1){
        endOfTurnGathering->add(numberOfPlayers -1);
    } else {
        endOfTurnGathering->wait();
    }
    return turn++;

}

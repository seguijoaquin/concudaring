#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>
#include <sys/types.h>




Player::Player(int _id, int _numberOfPlayers):id(_id),numberOfPlayers(_numberOfPlayers){
    //std::cout << "Se creo un player con el id:"<< getpid() << "\n";
    endOfTurnGathering = Semaforo(FILE_CONCUDARING,KEY_SEM_END_OF_TURN_GATHERING);
    waitForACard = Semaforo(FILE_CONCUDARING,KEY_SEM_WAIT_FOR_A_CARD);
}
Player::~Player(){
  //std::cout << "Se destruye un player con el id:"<< getpid() << "\n";
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
    table.setNumberOfPlayers(numberOfPlayers);
    int turno = 0;
    while ( turno <= 2 ){
        if (itIsMyTurn(turno)){
            sleep(2); //TODO: ACORDARSE DE SACARLO
            int card = myDeckOfCards.getCard();
            table.putCard(card);
            waitForACard.add(numberOfPlayers -1);
        } else {
            waitForACard.wait();
        }

        //En este deck SOLO obtengo las 2 ultima cartas de la mesa
        DeckOfCards deck = table.getLastTwoCards();
        deck.print();
        if (deck.theCardsAreSame() or deck.at() == 7){
            std::cout << "voy a poner la mano" << std::endl;
            table.putHand(id);
            std::cout << "sali" << std::endl;
            int idLoser = table.getIdLoser();
            if (idLoser == id){
                DeckOfCards deck = table.getCards();
                myDeckOfCards = myDeckOfCards + deck;
            }
        }

        turno = increaseTurn(turno);

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
    if ( endOfTurnGathering.numberOfProcessesWaiting() == numberOfPlayers -1){
        endOfTurnGathering.add(numberOfPlayers -1);
    } else {
        endOfTurnGathering.wait();
    }
    return turn + 1;

}

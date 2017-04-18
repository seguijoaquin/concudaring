#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>
#include <sys/types.h>




Player::Player(int _id, int _numberOfPlayers):id(_id),numberOfPlayers(_numberOfPlayers){
    endOfTurnGathering = Semaforo(FILE_CONCUDARING,KEY_SEM_END_OF_TURN_GATHERING);
    waitForACard = Semaforo(FILE_CONCUDARING,KEY_SEM_WAIT_FOR_A_CARD);
    noOneWon.create(FILE_CONCUDARING,KEY_MEM_NO_ONE_WON,1);
    //noOneWon.write(false);
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
    Table& table = Table::getInstance();
    table.setNumberOfPlayers(numberOfPlayers);
    int turno = 0;
    while (!noOneWon.read()){
        if (itIsMyTurn(turno)) {
            std::cout << "Soy el jugador ["<<id<<"] y  es mi turno\n";
            sleep(2); //TODO: ACORDARSE DE SACARLO
            int card = myDeckOfCards.getCard();
            std::cout << "Soy el jugador ["<<id<<"] y  voy a poner la carta["<<card<<"]\n";
            table.putCard(card);
        }
        //En este deck SOLO obtengo las 2 ultima cartas de la mesa
        std::cout << "Soy el jugador["<<id<<"] y  estoy esperando ver cartas\n";
        DeckOfCards deck = table.getLastTwoCards();
        deck.print();
        int idLoser;
        if (deck.theCardsAreSame() or deck.at() == 7){
            std::cout << "Soy el jugador ["<<id<<"] voy a poner la mano" << std::endl;
            table.putHand(id);
            std::cout << "sali ["<< id <<"] y voy mirar quién perdió!" << std::endl;
            idLoser = table.getIdLoser();
            //Si perdí
            if (idLoser == id){
                std::cout << "Soy el jugador ["<<id<<"] y  perdí\n";
                DeckOfCards deck = table.getCards();
                //myDeckOfCards = myDeckOfCards + deck;
                //myDeckOfCards.addDeck(deck);
                std::cout << "Soy el jugador ["<<id<<"] y  ya tomé todas las cartas de la mesa\n";
            }
        }
        if (itIsMyTurn(turno) && idLoser != id){
            std::cout << "Soy el jugador ["<<id<<"] y me fijo si gané\n";
            if (myDeckOfCards.isEmpty()){
                noOneWon.write(true);
            }

        }
        std::cout << "Soy el jugador ["<<id<<"], incremento en turno\n";
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

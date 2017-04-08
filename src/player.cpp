#include <iostream>
#include "player.h"
#include <unistd.h>


Player::Player(int _id, Semaforo *waitForACard, Semaforo * endOfTurnGathering, int numberOfPlayers) : id(_id){
    this->waitForACard = waitForACard;
    this->endOfTurnGathering = endOfTurnGathering;
    this->numberOfPlayers = numberOfPlayers;
}

Player::~Player(){
    std::cout << "Destruyendo player" << std::endl;
}


void Player::present() {
    std::cout << "Mi id es:"<< id << std::endl;
}


void Player::play() {

    //TODO: Cambiar por una condicion real

    int turno = 0;

    while ( turno < 3 ){

        if ( itIsMyTurn(turno)) {

            //TODO: Logica de tirar carta y demas

            std::cout << id << ": ES mi turno" << std::endl;
            //TODO: BORRAR EL SLEEP!!

            sleep(3);
            std::cout << id << ": PUSE LA CARTA" << std::endl;

            //Al terminar de jugar habilita N - 1 lugares en el semaforo para que puedan pasar los demas.

            this->waitForACard->add(numberOfPlayers - 1);

        } else {
            std::cout << id << ": NO es mi turno. Espero a que jueguen" << std::endl;
            this->waitForACard->p();
        }
        std::cout << id << ": VER CARTA" << std::endl;




        //TODO: Logica de actuar con respecto a que carta salio.




        if ( this->endOfTurnGathering->numberOfProcessesWaiting() == numberOfPlayers - 1 ){
            endOfTurnGathering->add(numberOfPlayers - 1 );
        } else {
            endOfTurnGathering->p();
        }
        turno++;
    }

}

bool Player::itIsMyTurn(int turnNumber) {
    //TODO: ver si lo vamos a hacer asi
    int nextToPlay = turnNumber % numberOfPlayers;
    return nextToPlay == this->id;
}

void Player::setDeckOfCards(DeckOfCards deck) {
    myDeckOfCards = deck;
}


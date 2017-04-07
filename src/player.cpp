#include <iostream>
#include "player.h"
#include <unistd.h>


Player::Player(int _id, Semaforo *waitForACard, Semaforo * waitToSeeIfThereIsAWinner, int numberOfPlayers) : id(_id){
    this->waitForACard = waitForACard;
    this->waitToSeeIfThereIsAWinner = waitToSeeIfThereIsAWinner;
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
    //Simulo 2 turnos
    for ( int i =0; i < 2 ; i ++ ) { //while ( theGameIsNotOver() ){

        if ( itIsMyTurn(i)) {
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

        if (itIsMyTurn(i)){
            std::cout << id << ": ME FIJO SI GANE" << std::endl;

            if ( iHaveNoCardsInMyDeck() ){
                //TODO: GANO HACER LA LOGICA PARA SALIR DEL WHILE
            }
            waitToSeeIfThereIsAWinner->add(numberOfPlayers - 1);
        } else {
            //TODO: Otro Semaforo para ver si gano
            std::cout << id << ": Espero a ver si la persona que jugo gano" << std::endl;
            this->waitToSeeIfThereIsAWinner->p();

        }
        //SI gano entonces sale del while sino hay otro turno.
    }
}

bool Player::itIsMyTurn(int turnNumber) {
    //TODO: ver si lo vamos a hacer asi
    int nextToPlay = turnNumber % numberOfPlayers;
    return nextToPlay == this->id;
}

void Player::setDeckOfCards(DeckOfCards deck){
    myDeckOfCards = deck;
}



bool Player::iHaveNoCardsInMyDeck() {
    //return myDeckOfCards.isEmpty();
    return false;
}


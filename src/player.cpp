#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>
#include <sys/types.h>




Player::Player(int _id, int _numberOfPlayers):id(_id),numberOfPlayers(_numberOfPlayers){
    endOfTurnGathering = Semaforo(FILE_CONCUDARING,KEY_SEM_END_OF_TURN_GATHERING);
    noOneWon.create(FILE_CONCUDARING,KEY_MEM_NO_ONE_WON,1);
    //noOneWon.write(false);
}
Player::~Player(){
  //std::cout << "Se destruye un player con el id:"<< getpid() << "\n";
}


void Player::present() const {
    //std::cout << "Mi id es:"<< id << std::endl;
}


void Player::play() {
    Table& table = Table::getInstance();
    table.setNumberOfPlayers(numberOfPlayers);
    int turno = 0;
    while (!noOneWon.read()){
        this->myDeckOfCards.print();
        if (itIsMyTurn(turno)) {
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_ES_MI_TURNO,id);
            int card = myDeckOfCards.getCard();
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_PONIENDO_CARTA,card);
            table.putCard(card);
            //std::cout << "Soy el jugador ["<<id<<"] y  mi mazo es: \n";
        }
        //En este deck SOLO obtengo las 2 ultima cartas de la mesa
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_ESPERANDO_VER_CARTA,id);

        checkCardsAndPerformAction();
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_VIENDO_CARTA,id);

        if (itIsMyTurn(turno)){
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_VERIFICO_SI_GANE,id);
            if (myDeckOfCards.isEmpty()){
                noOneWon.write(true);
            }
        }

        Logger::getInstance()->insert(KEY_PLAYER,MSJ_INCREMENTO_TURNO,id);
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

void Player::checkCardsAndPerformAction() {
    Table& table = Table::getInstance();
    DeckOfCards lastTwoCards = table.getLastTwoCards();
    lastTwoCards.print();
    int idLoser;

    //TODO: CUANDO FUNCIONE REFACTORIZAR
    if (lastTwoCards.theCardsAreSame() or lastTwoCards.at() == 7){
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_PONGO_MANO,id);
        table.putHand(id);
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_VERIFICO_PERDEDOR,id);
        idLoser = table.getIdLoser();
        //Si perdí
        if (idLoser == id){
          Logger::getInstance()->insert(KEY_PLAYER,MSJ_PERDI);
            DeckOfCards deck = table.getCards();
            //myDeckOfCards = myDeckOfCards + deck;
            //myDeckOfCards.addDeck(deck);
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_TOME_CARTAS_DE_MESA,id);
        }
    } else if(lastTwoCards.at() == 10){
        //TODO: BUenos dias seniorita
    } else if(lastTwoCards.at() == 11){
        //TODO: Buenas noches caballero
    } else if(lastTwoCards.at() == 12){
        //TODO: Saludo militar.
    }
}

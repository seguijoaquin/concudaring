#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>
#include <sys/types.h>




Player::Player(int _id, int _numberOfPlayers):id(_id),numberOfPlayers(_numberOfPlayers){
    communicationChannel = CommunicationChannel(numberOfPlayers,id);
}
Player::~Player(){
}


void Player::present() const {
    //std::cout << "Mi id es:"<< id << std::endl;
}


void Player::play() {
    Table& table = Table::getInstance();
    table.setNumberOfPlayers(numberOfPlayers);
    int turno = 0;

    do  {

        //this->myDeckOfCards.print();

        if (itIsMyTurn(turno)) {
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_ES_MI_TURNO,id);
            int card = myDeckOfCards.getCard();
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_PONIENDO_CARTA,card);
            table.putCard(card);
        }

        //En este deck SOLO obtengo las 2 ultima cartas de la mesa
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_ESPERANDO_VER_CARTA,id);

        checkCardsAndPerformAction();
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_VIENDO_CARTA,id);

        turno++;
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_INCREMENTO_TURNO,id);
    } while ( !thereIsAWinner() );

    communicationChannel.cerrar();
    communicationChannel.eliminar();

}

bool Player::itIsMyTurn(int turnNumber) const {
    int nextToPlay = turnNumber % numberOfPlayers;
    return nextToPlay == this->id;
}

void Player::setDeckOfCards(DeckOfCards& deck) {
    myDeckOfCards = deck;
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

bool Player::thereIsAWinner() {
    //Aviso a los demas si gane o no
    if (myDeckOfCards.isEmpty()){
        communicationChannel.sendToAll("g");
        return true;
    } else {
        communicationChannel.sendToAll("n");
    }

    // Me fijo si alguno de los demas gano
    for (int i = 0; i < numberOfPlayers - 1 ; ++i) {
        std::string data = communicationChannel.receive(1);
        if(data == "g"){
            return true;
        }
    }
    return false;
}

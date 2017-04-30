#include <iostream>
#include "player.h"
#include "table.h"
#include <unistd.h>
#include <sys/types.h>
#include "stdlib.h"



Player::Player(int _id, int _numberOfPlayers):id(_id),numberOfPlayers(_numberOfPlayers){
    gatheringPoint = Semaforo(FILE_CONCUDARING,'g');
    thereIsACard = Semaforo(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);
    communicationChannel = CommunicationChannel("./communicationChannel",numberOfPlayers,id);
    specialCardActions = CommunicationChannel("./specialCardActions", numberOfPlayers,id);
    turno = 0;
}

Player::~Player(){
}

void Player::present() const {
}

void Player::play() {
    Table& table = Table::getInstance();
    table.setNumberOfPlayers(numberOfPlayers);
    do  {
        if (itIsMyTurn(turno)) {
            Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_ES_MI_TURNO);
            int card = myDeckOfCards.getCard();
            Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_PONIENDO_CARTA,card);
            table.putCard(card);
        }
        checkCardsAndPerformAction();
    } while ( !thereIsAWinner() );
    gather();
    freeCommunicationChannels();
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
    Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_ESPERANDO_VER_CARTA);
    DeckOfCards lastTwoCards = table.getLastTwoCards();

    int idLoser;

    Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_VIENDO_CARTA);
    //TODO: CUANDO FUNCIONE REFACTORIZAR
    if (lastTwoCards.theCardsAreSame() or lastTwoCards.at() == 7){
        Logger::getInstance()->insert(KEY_PLAYER,id,turno,ATREVIDO);
        specialCardActions.sendToAll(ATREVIDO);
        Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_PONGO_MANO);
        table.putHand(id);
        waitUntilTheOtherPlayersSaid(ATREVIDO);
        Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_VERIFICO_PERDEDOR);
        idLoser = table.getIdLoser();
        //Si perdí
        if (idLoser == id){
            Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_PERDI);
            DeckOfCards deck = table.getCards();
            myDeckOfCards = myDeckOfCards + deck;
            myDeckOfCards.print();
            Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_TOME_CARTAS_DE_MESA);
        }
    } else if(lastTwoCards.at() == 10){
        sayOrDoSomethingAndWaitForTheRestToDoTheSame(BUENOS_DIAS_SENIORITA);
    } else if(lastTwoCards.at() == 11){
        sayOrDoSomethingAndWaitForTheRestToDoTheSame(BUENAS_NOCHES_CABALLERO);
    } else if(lastTwoCards.at() == 12){
        sayOrDoSomethingAndWaitForTheRestToDoTheSame(SALUDO_MILITAR);
    }
}

//TODO: Ver si es necesario
void Player:: gather(){
    gatheringPoint.wait();
    gatheringPoint.barrier();
}

bool Player::thereIsAWinner() {
    //Aviso a los demas si gane o no
    if (myDeckOfCards.isEmpty()){
        Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_GANE_Y_LE_AVISO_A_LOS_DEMAS);
        communicationChannel.sendToAll(GANE);
        return true;
    } else {
        communicationChannel.sendToAll(NO_GANE);
    }
    Logger::getInstance()->insert(KEY_PLAYER,id,turno,MSJ_INCREMENTO_TURNO);
    // Me fijo si alguno de los demas gano
    for (int i = 0; i < numberOfPlayers - 1 ; ++i) {
        MSG_t data = communicationChannel.receive(GANE.size());
        if(data.message == GANE){
            std::stringstream ss;
            ss << MSJ_OTRO_JUGADOR_GANO << data.id;
            Logger::getInstance()->insert(KEY_PLAYER,id,turno,ss.str());
            return true;
        }
    }
    turno++;
    return false;
}

void Player::waitUntilTheOtherPlayersSaid(std::string message) {
    bool thePlayerSaidTheMessage[numberOfPlayers];
    thePlayerSaidTheMessage[id] = true;

    for (int i = 0; i < numberOfPlayers - 1 ; ++i) {
        MSG_t data = specialCardActions.receive(message.size());
        std::stringstream ss;
        ss << "Recibi: " + data.message + " de: " << data.id;
        Logger::getInstance()->insert(KEY_PLAYER,id,turno,ss.str());
        if(data.message == message){
            thePlayerSaidTheMessage[data.id] = true;
        }
    }
    bool theyAllSaidTheMessage = true;

    for (int j = 0; j < numberOfPlayers ; ++j) {
        theyAllSaidTheMessage = theyAllSaidTheMessage && thePlayerSaidTheMessage[j];
    }

    if (theyAllSaidTheMessage) {

    }
}

void Player::sayOrDoSomethingAndWaitForTheRestToDoTheSame(std::string messageOrAction) {
    Logger::getInstance()->insert(KEY_PLAYER,id,turno,messageOrAction);
    specialCardActions.sendToAll(messageOrAction);
    waitUntilTheOtherPlayersSaid(messageOrAction);
}

void Player::freeCommunicationChannels() {
    communicationChannel.cerrar();
    communicationChannel.eliminar();
    specialCardActions.cerrar();
    specialCardActions.eliminar();
}

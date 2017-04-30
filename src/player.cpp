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
}

Player::~Player(){
}

void Player::present() const {
}

void Player::play() {
    Table& table = Table::getInstance();
    table.setNumberOfPlayers(numberOfPlayers);
    int turno = 0;
    do  {
        if (itIsMyTurn(turno)) {
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_ES_MI_TURNO,id);
            int card = myDeckOfCards.getCard();
            Logger::getInstance()->insert(KEY_PLAYER,MSJ_PONIENDO_CARTA,card);
            table.putCard(card);
        }
        checkCardsAndPerformAction(turno);
        turno++;
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


void Player::checkCardsAndPerformAction(int turno) {
    Table& table = Table::getInstance();
    Logger::getInstance()->insert(KEY_PLAYER,MSJ_ESPERANDO_VER_CARTA,id);
    DeckOfCards lastTwoCards = table.getLastTwoCards();
    int idLoser;
    Logger::getInstance()->insert(KEY_PLAYER,MSJ_VIENDO_CARTA,id);
    //TODO: CUANDO FUNCIONE REFACTORIZAR
    if (lastTwoCards.theCardsAreSame() or lastTwoCards.at() == 7){
        /*Logger::getInstance()->insert(KEY_PLAYER,MSJ_PONGO_MANO,id);
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
        }*/
    } else if(lastTwoCards.at() == 10){
        std::string message("Buenos dias seniorita");
        sayOrDoSomethingAndWaitForTheRestToDoTheSame(message);
    } else if(lastTwoCards.at() == 11){
        std::string message("Buenas noches caballero");
        sayOrDoSomethingAndWaitForTheRestToDoTheSame(message);
    } else if(lastTwoCards.at() == 12){
        std::string message("Saludo militar");
        sayOrDoSomethingAndWaitForTheRestToDoTheSame(message);
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
        Logger::getInstance()->insert(KEY_PLAYER,MSJ_GANE_Y_LE_AVISO_A_LOS_DEMAS,id);
        communicationChannel.sendToAll("g");
        return true;
    } else {
        communicationChannel.sendToAll("n");
    }
    Logger::getInstance()->insert(KEY_PLAYER,MSJ_INCREMENTO_TURNO,id);
    // Me fijo si alguno de los demas gano
    for (int i = 0; i < numberOfPlayers - 1 ; ++i) {
        MSG_t data = communicationChannel.receive(1);
        if(data.message == "g"){
            std::stringstream ss;
            ss << MSJ_OTRO_JUGADOR_GANO << data.id << ". Soy:";

            Logger::getInstance()->insert(KEY_PLAYER,ss.str(),id);
            return true;
        }
    }

    return false;
}

void Player::waitUntilTheOtherPlayersSaid(std::string message) {
    bool thePlayerSaidTheMessage[numberOfPlayers];
    thePlayerSaidTheMessage[id] = true;

    for (int i = 0; i < numberOfPlayers - 1 ; ++i) {
        MSG_t data = specialCardActions.receive(message.size());
        std::stringstream ss;
        ss << "Recibi: " + data.message + " de: " << data.id << ". Soy: ";
        Logger::getInstance()->insert(KEY_PLAYER, ss.str(),id);
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
    Logger::getInstance()->insert(KEY_PLAYER, messageOrAction + ". Soy: ",id);
    specialCardActions.sendToAll(messageOrAction);
    waitUntilTheOtherPlayersSaid(messageOrAction);
}

void Player::freeCommunicationChannels() {
    communicationChannel.cerrar();
    communicationChannel.eliminar();
    specialCardActions.cerrar();
    specialCardActions.eliminar();
}

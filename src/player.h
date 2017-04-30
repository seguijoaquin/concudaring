#ifndef CONCUDARING_PLAYER_H
#define CONCUDARING_PLAYER_H


#include "utils/Semaforo.h"
#include "deckOfCards.h"
#include "utils/SharedMemory.h"
#include "utils/types.h"
#include "utils/constants.h"
#include "table.h"
#include "utils/logger/logger.h"
#include "utils/logger/mensajes.h"
#include "utils/FifoEscritura.h"
#include "utils/FifoLectura.h"
#include "CommunicationChannel.h"

class Player {
private:
    const int id;
    const int numberOfPlayers;
    DeckOfCards myDeckOfCards;
    Semaforo thereIsACard;
    Semaforo gatheringPoint;

    CommunicationChannel communicationChannel;
    CommunicationChannel specialCardActions;

public:
    Player(int _id,int _numberOfPlayers);
    ~Player();
    void present() const ;

    void play();
    void setDeckOfCards(DeckOfCards& deck);
    bool itIsMyTurn(int turnNumber) const;
    void checkCardsAndPerformAction(int turno);
    bool thereIsAWinner();
    void waitUntilTheOtherPlayersSaid(std::string message);
    void gather();
    void sayOrDoSomethingAndWaitForTheRestToDoTheSame(std::string messageOrAction);
    void freeCommunicationChannels();


};


#endif //CONCUDARING_PLAYER_H

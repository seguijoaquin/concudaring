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

const std::string BUENOS_DIAS_SENIORITA("¡Buenos dias seniorita!");
const std::string BUENAS_NOCHES_CABALLERO("¡Buenas noches caballero!");
const std::string SALUDO_MILITAR("¡Saludo militar!");
const std::string GANE("g");
const std::string NO_GANE("n");
const std::string ATREVIDO("¡Atrevido!");

const std::string COMMUNICATION_CHANNEL_FILE("./CommunicationChannel");
const std::string SPECIAL_CARD_ACTIONS("./SpecialCardActions");

class Player {
private:
    int turno;
    const int id;
    const int numberOfPlayers;
    DeckOfCards myDeckOfCards;
    Semaforo thereIsACard;
    Semaforo gatheringPoint;
    CommunicationChannel communicationChannel;
    CommunicationChannel specialCardActions;
    void sendInformationTheJudge();
    bool itIsMyTurn() const;
    void checkCardsAndPerformAction();
    bool thereIsAWinner();
    void waitUntilTheOtherPlayersSaid(std::string message);
    void gather();
    void sayOrDoSomethingAndWaitForTheRestToDoTheSame(std::string messageOrAction);
    void freeCommunicationChannels();
public:
    Player(int _id,int _numberOfPlayers);
    ~Player();
    void play();
    void setDeckOfCards(DeckOfCards& deck);
};


#endif //CONCUDARING_PLAYER_H

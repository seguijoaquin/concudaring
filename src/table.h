#ifndef CONCUDARING_TABLE_H
#define CONCUDARING_TABLE_H


#include "deckOfCards.h"
#include "utils/SharedMemory.h"
#include "utils/constants.h"
#include "utils/Semaforo.h"

class Table {
private:
    Table();
    SharedMemory<int> cards; //destinado para colocar las cartas
    SharedMemory<int> i;     //destinado para colocar el puntero al mazo
    SharedMemory<int> idHand; //destinado para guardar el id del ultimo jugador que puso la mano
    SharedMemory<int> numberOfPlayersPutHand; //destinado para saber cantidad de jugadores que pusieron la mano.
    int numberOfPlayers;
    Semaforo thereIsCard;
    Semaforo writeIdLosser;
    Table(const Table& table);
    void operator=(const Table& table);
    ~Table();
    void createSemaforo();
    void printCards() const ;
public:
    static Table& getInstance();
    void putCard(int card);
    void putHand(int id);
    int  getIdLoser();
    DeckOfCards getLastTwoCards();
    DeckOfCards getCards();
    void setNumberOfPlayers(int _numberOfPlayers);
};


#endif //CONCUDARING_TABLE_H

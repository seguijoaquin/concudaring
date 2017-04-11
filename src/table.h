#ifndef CONCUDARING_TABLE_H
#define CONCUDARING_TABLE_H


#include "deckOfCards.h"
#include "utils/MemoriaCompartida2.h"
#include "utils/Semaforo.h"

class Table {
private:
    Table();
    MemoriaCompartida2<int> cards; //destinado para colocar las cartas
    MemoriaCompartida2<int> i;     //destinado para colocar el puntero al mazo
    MemoriaCompartida2<int> idHand; //destinado para guardar el id del ultimo jugador que puso la mano
    Semaforo putCard;
    //Semaforo turnOver;
    Table(const Table& table);
    void operator=(const Table& table);
    ~Table();
    void createSemaforo();
public:
    static Table& getInstance();
    void putHand(int id);
    int  getIdLosser();
    void printCards(int id);
};


#endif //CONCUDARING_TABLE_H

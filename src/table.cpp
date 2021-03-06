#include <iostream>
#include "table.h"
#include "utils/logger/logger.h"
#include "utils/logger/mensajes.h"

#define NOMBRE "/bin/ls"
#define NOMBRE2 "/bin/echo"

using namespace std;

Table::Table(){
    cards.create(NOMBRE,KEY_SHME_TABLE_CARDS,40);
    i.create(FILE_CONCUDARING,KEY_SHME_TABLE_I,1);
    //acá es donde se pone los id de los jugadores, a medida que ponen las mano.
    idHand.create(FILE_CONCUDARING,KEY_SHME_TABLE_ID_HAND,1);
    numberOfPlayersPutHand.create(FILE_CONCUDARING,KEY_SHME_TABLE_NUMBER_PLAYER_PUT_HAND,1);
    createSemaforo();
}

void Table::createSemaforo() {
    thereIsCard = Semaforo(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);
    writeIdLosser = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_LOSER);
}

Table& Table::getInstance() {
    //std::cout << "getInstance" << std::endl;
    static Table instance;
    return instance;
}

//Inserto una carta en la mesa
void Table::putCard(int card) {
    int pos = i.read();
    cards[pos] = card;
    i.write(pos+1);
    std::stringstream ss;
    ss<<  cards[pos-1] <<"-"<< cards[pos];
    Logger::getInstance()->insert(KEY_MESA,ss.str());
    thereIsCard.add(numberOfPlayers);


}


//Devuelvo todas las cartas que se encuentra en la mesa
DeckOfCards Table::getCards() {
    int size = i.read();
    DeckOfCards deck;
    for (int j = 0; j < size; ++j) {
        deck.addCard(cards[j]);
    }
    i.write(0); //Posicion el puntero al principio
    return deck;
}


//Devuelvo las 2 ultimas cartas que se encuentran en la mesa
DeckOfCards Table::getLastTwoCards(){
    thereIsCard.wait();
    DeckOfCards deck;
    int lastPosition = i.read() -1;
    if (lastPosition == 0){
        deck.addCard(cards[lastPosition]);
        return deck;
    }
    deck.addCard(cards[lastPosition-1]);
    deck.addCard(cards[lastPosition]);
    return deck;
}


void Table::setNumberOfPlayers(int _numberOfPlayers) {
    numberOfPlayers = _numberOfPlayers;
}

//Muestro el mazo de cartas que hay en la mesa
void Table::printCards() const {
    int size = i.read();
     std::cout << "Soy la mesa y muestro las cartas\n";
     for (int j = 0; j < size ; j++) {
         std::cout << "["<<j<<"]:" << cards[j] <<" ";
     }
    std::cout << "\n";
}


void Table::putHand(int id) {
    writeIdLosser.wait();
    idHand.write(id);
    int aux = numberOfPlayersPutHand.read() + 1;
    numberOfPlayersPutHand.write(aux);
    writeIdLosser.signal();
}

int Table::getIdLoser() {
    int result = -1;
    writeIdLosser.wait();
    if (numberOfPlayersPutHand.read() == numberOfPlayers){
        result = idHand.read();
        numberOfPlayersPutHand.write(0);
    }
    writeIdLosser.signal();
    return result;

}

Table::~Table(){
}

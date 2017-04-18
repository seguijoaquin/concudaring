#include <iostream>
#include "table.h"
#include <unistd.h>

#define NOMBRE "/bin/ls"
#define NOMBRE2 "/bin/echo"

using namespace std;

Table::Table(){
    //std::cout << "Construyendo una mesa con process id:" <<getpid()<< std::endl;
    cards.create(NOMBRE,'c',40);
    i.create(NOMBRE2,'i',1);
    idHand.create(NOMBRE,'d',1);  //acá es donde se pone los id de los jugadores, a medida que pone las mano.
    //numberOfPlayersPutHand.create(NOMBRE,'n',1);
    createSemaforo();
}

void Table::createSemaforo() {
    char nombre[] = "/bin/cat";
    thereIsCard = Semaforo(nombre,'t');
    writeIdLosser = Semaforo(nombre,'w');
    readIdLosser = Semaforo(nombre,'r');

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
    printCards(90);
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
}


//Devuelvo las 2 ultimas cartas que se encuentran en la mesa
DeckOfCards Table::getLastTwoCards(){
    thereIsCard.wait();
    DeckOfCards deck;
    int lastPosition = i.read() -1;
    //std::cout << "lastPOsition:" <<lastPosition << "i:["<< i.read()<<"]\n";
    deck.addCard(cards[lastPosition]);
    if (lastPosition == 0){
        //std::cout << "lastposicion es 0";
        return deck;
    }
    deck.addCard(cards[lastPosition-1]);
    return deck;
}


void Table::setNumberOfPlayers(int _numberOfPlayers) {
    numberOfPlayers = _numberOfPlayers;
}

//Muestro el mazo de cartas que hay en la mesa
void Table::printCards(int id) {
    int size = i.read();
     std::cout << "Soy el JUGADOR:" << id << " Y muestro las cartas\n";
     for (int j = 0; j < size ; j++) {
         std::cout << "["<<j<<"]:" << cards[j] <<" ";
     }
    std::cout << "\n";
}


void Table::putHand(int id) {
    writeIdLosser.wait();
    idHand.write(id);
    writeIdLosser.signal();
}

int Table::getIdLoser() {
    if (readIdLosser.numberOfProcessesWaiting() == numberOfPlayers -1){
        readIdLosser.add(numberOfPlayers-1);
    }else{
        readIdLosser.wait();
    }
    return idHand.read();
}

Table::~Table(){
  //std::cout << "Destruyendo una mesa con process id:" <<getpid()<< std::endl;
}

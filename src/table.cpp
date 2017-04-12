#include <iostream>
#include "table.h"
#include <stdlib.h>  //rand()
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NOMBRE "/bin/ls"
#define NOMBRE2 "/bin/echo"

using namespace std;

Table::Table(){
    std::cout << "Construyendo una mesa con process id:" <<getpid()<< std::endl;
    cards.create(NOMBRE,'o',40);
    i.create(NOMBRE2,'r',1);
    idHand.create(NOMBRE,'p',1);
    createSemaforo();
}

void Table::createSemaforo() {
    char nombre[] = "/bin/cat";
    thereIsCard = Semaforo(nombre,0,'a');

}

Table& Table::getInstance() {
    std::cout << "getInstance" << std::endl;
    static Table instance;
    return instance;
}

//Inserto una carta en la mesa
void Table::putCard(int card) {
    //turnOver.p();
    int pos = i.read();
    cards[pos] = card;
    i.write(pos+1);
    thereIsCard.add(2);
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
    int lastPosition = i.read() - 1;
    DeckOfCards deck;
    deck.addCard(cards[lastPosition]);
    deck.addCard(cards[lastPosition-1]);
    return deck;
}


//Muestro el mazo de cartas que hay en la mesa
void Table::printCards(int id) {
    thereIsCard.p();
    // int size = i.read();
    // std::cout << "Soy el JUGADOR:" << id << " Y muestro las cartas\n";
    // for (int j = 0; j < size ; j++) {ma
    //     std::cout << "["<<j<<"]:" << cards[j] <<" ";
    // }
    // std::cout << "\n";
}

//TODO:Acá tengo que poner un bloque para que no mas de un proceso este poniendo su mano
void Table::putHand(int id) {
    idHand.write(id);
}

int Table::getIdLosser() {
    return idHand.read();
}

Table::~Table(){
  std::cout << "Destruyendo una mesa con process id:" <<getpid()<< std::endl;
}

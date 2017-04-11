#include <iostream>
#include "table.h"
#include <stdlib.h>  //rand()
#include <sys/ipc.h>
#include <sys/shm.h>

#define NOMBRE "/bin/ls"
#define NOMBRE2 "/bin/echo"

using namespace std;

Table::Table(){
    std::cout << "Construyendo una mesa" << std::endl;
    cards.crear(NOMBRE,'o',40);
    i.crear(NOMBRE2,'r',1);
    createSemaforo();
}

void Table::createSemaforo() {
    char nombre[] = "/bin/cat";
    putCard = Semaforo(nombre,0);

}

Table& Table::getInstance() {
    std::cout << "getInstance" << std::endl;
    static Table instance;
    return instance;
}


void Table::putHand(int id) {
    //turnOver.p();
    int pos = i.leer();
    cards[pos] = id;
    i.escribir(pos+1);
    putCard.add(2);
}

void Table::printCards(int id) {
    putCard.p();
    int size = i.leer();
    std::cout << "Soy el JUGADOR:" << id << " Y muestro las cartas\n";
    for (int j = 0; j < size ; j++) {
        std::cout << "["<<j<<"]:" << cards[j] <<" ";
    }
    std::cout << "\n";
}

Table::~Table(){
    std::cout << "Destruyendo una mesa" << std::endl;
    //putCard.eliminar();
}
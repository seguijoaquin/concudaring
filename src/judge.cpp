#include "judge.h"
#include "utils/constants.h"
#include <iostream>
#include <sstream>


Judge::Judge(){
  numberOfPlayerThatWrote.create(FILE_CONCUDARING,KEY_SHME_JUDGE_NUMBER,1);
  writeNumberOfCards = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_NUMBER_OF_CARDS);
  turno = -1;
}

void Judge::setNumberOfPlayers(int _numberOfPlayers){
  numberOfPlayers = _numberOfPlayers;
  createMemory();
}

Judge& Judge::getInstance() {
    static Judge instance;
    return instance;
}


void Judge::createMemory(){
  numberOfPlayerCards.create(FILE_CONCUDARING,KEY_SHME_JUDGE,numberOfPlayers);
}

void Judge::writeNumberOfPlayerCards(int id, int number){
  numberOfPlayerCards[id] = number;
  turno++;
  incrementNumber();
}


void Judge::incrementNumber(){
  int number = numberOfPlayerThatWrote.read() + 1;
  numberOfPlayerThatWrote.write(number);
  if (number == numberOfPlayers){
    printInformation();
    resetNumberOfPlayerThatWrote();
  }

}

void Judge::printInformation() const{
  std::stringstream logPlayer;
  for (size_t i = 0; i < numberOfPlayers; i++){
    logPlayer <<"J["<<i<< "] = " << numberOfPlayerCards[i] << " ";
  }
  logPlayer << "\n";
  std::cout << "[  Juez  ]  " << logPlayer.str();
}

void Judge::resetNumberOfPlayerThatWrote(){
  numberOfPlayerThatWrote.write(0);
}

Judge::~Judge(){

}

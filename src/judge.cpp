#include "judge.h"
#include "utils/constants.h"
#include <iostream>
#include <sstream>
#include <unistd.h> //fork()


Judge::Judge(){
  writeNumberOfCards = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_NUMBER_OF_CARDS);
  conditionSem = Semaforo(FILE_CONCUDARING,KEY_SEM_JUDE_CONDITION);
  condition.create(FILE_CONCUDARING,KEY_SHME_JUDGE_CONDITION,1);
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
  writeNumberOfCards.wait();
  numberOfPlayerCards[id] = number;
  writeNumberOfCards.signal();

}


void Judge::printInformation(){
  std::stringstream logPlayer;
  std::cout << "Print information\n";
  //writeNumberOfCards.wait();
  std::cout << "Print information 2\n";
  for (size_t i = 0; i < numberOfPlayers; i++){
    logPlayer <<"J["<<i<< "] = " << numberOfPlayerCards[i] << " ";
  }
  //writeNumberOfCards.signal();
  logPlayer << "\n";
  std::cout << "[  Juez  ]  " << logPlayer.str();
}



void Judge::start(){
  std::cout << "El juez empieza a imrpimir la infor\n";
  pid_t pid = fork();
  if (pid == 0){
    while(iCanContinue()){
      //sleep(1);
      std::cout << "Dentro del while\n";
      printInformation();
    }
  }
}

bool Judge::iCanContinue(){
  std::cout<< "I can continue\n";
  int valor;
  conditionSem.wait();
  valor =  condition.read();
  conditionSem.signal();
  std::cout<< "I can exit::"<< valor<<"\n";
  return valor;
}
void Judge::stop(){
  std::cout << "judge stop\n";
  conditionSem.wait();
  condition.write(false);
  conditionSem.signal();
}

Judge::~Judge(){
}

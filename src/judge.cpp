#include "judge.h"
#include "utils/constants.h"
#include <iostream>
#include <sstream>
#include <unistd.h> //fork()
#include <sys/types.h>
#include <sys/wait.h>
#include "utils/logger/logger.h"
#include "utils/logger/mensajes.h"

Judge::Judge(){
  writeNumberOfCards = Semaforo(FILE_CONCUDARING,KEY_SEM_WRITE_NUMBER_OF_CARDS);
  conditionSem = Semaforo(FILE_CONCUDARING,KEY_SEM_JUDE_CONDITION);
  condition.create(FILE_CONCUDARING,KEY_SHME_JUDGE_CONDITION,1);
  condition.write(true);
  timeBetweenChecks = 0.1;
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
  writeNumberOfCards.wait();
  conditionSem.wait();
  for (size_t i = 0; i < numberOfPlayers; i++){
    logPlayer <<"J["<<i<< "] = " << numberOfPlayerCards[i] << "; ";
  }
  writeNumberOfCards.signal();
  conditionSem.signal();
  //logPlayer << "\n";
  Logger::getInstance()->insert(KEY_JUDGE,logPlayer.str());
  //std::cout << "[  Juez  ]  " << logPlayer.str();
}



void Judge::start(){
  printer = fork();
  if (printer == 0){
    setNumberOfPlayers(numberOfPlayers);
    while(iCanContinue()){
      sleep(timeBetweenChecks);
      printInformation();
    }
    exit(0);
  }else{
    Logger::getInstance()->insert(KEY_JUDGE,LANZA_JUEZ);
  }
}

bool Judge::iCanContinue(){
  int valor;
  conditionSem.wait();
  valor =  condition.read();
  conditionSem.signal();
  return valor;
}

void Judge::stop(){
  conditionSem.wait();
  condition.write(false);
  conditionSem.signal();
  waitpid(printer,NULL,0);
  std::cout << "[  Judge  ] stop.\n";
}

Judge::~Judge(){
}

void Judge::setTimeBetweenChecks(double timeBetweenChecks) {
  this->timeBetweenChecks = timeBetweenChecks;
}

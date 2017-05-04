#ifndef JUDGE_H
#define  JUDGE_H


#include "utils/SharedMemory.h"
#include "utils/Semaforo.h"


class Judge{
private:
  Judge();
  ~Judge();
  SharedMemory<int> numberOfPlayerCards;
  SharedMemory<bool> condition;
  Semaforo writeNumberOfCards;
  Semaforo conditionSem;
  int numberOfPlayers;
    double timeBetweenChecks;
  pid_t shower;
  void createMemory();
  void printInformation();
  bool iCanContinue();
public:
  static Judge& getInstance();
  void writeNumberOfPlayerCards(int id, int cant);
  void setNumberOfPlayers(int numberOfPlayers);
    void setTimeBetweenChecks(double timeBetweenChecks);
  void start();
  void stop();
};





#endif /*JUDGE_H*/

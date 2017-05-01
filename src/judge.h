#ifndef JUDGE_H
#define  JUDGE_H


#include "utils/SharedMemory.h"
#include "utils/Semaforo.h"


class Judge{
private:
  Judge();
  ~Judge();
  SharedMemory<int> numberOfPlayerCards;
  SharedMemory<int> numberOfPlayerThatWrote;
  Semaforo writeNumberOfCards;
  int numberOfPlayers;
  void createMemory();
  void incrementNumber();
  void printInformation() const;
  void resetNumberOfPlayerThatWrote();
  int turno;
public:
  static Judge& getInstance();
  void writeNumberOfPlayerCards(int id, int cant);
  void setNumberOfPlayers(int numberOfPlayers);
};





#endif /*JUDGE_H*/

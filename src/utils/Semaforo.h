#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaforo {

private:
    int id;

public:
    Semaforo();
    Semaforo (char* nombre , char fkey );
    virtual ~Semaforo();
    int inicializar (int valorInicial);
    int wait(); // decrementa
    int signal(); // incrementa
    void eliminar ();
    int numberOfPlayersWaiting();
    int add(int value); //incrementa en value
    int numberOfProcessesWaiting();

};

#endif /* SEMAFORO_H_ */

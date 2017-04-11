#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaforo {

private:
    int id;
    int valorInicial;
    int inicializar ();

public:
    Semaforo();
    Semaforo (char* nombre ,int valorInicial, char fkey );
    virtual ~Semaforo();

    int p (); // decrementa
    int v (); // incrementa
    void eliminar ();
    int numberOfPlayersWaiting();
    int add(int value); //incrementa en value
    int numberOfProcessesWaiting();
};

#endif /* SEMAFORO_H_ */
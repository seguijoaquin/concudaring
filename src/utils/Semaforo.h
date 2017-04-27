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
    Semaforo (const std::string& nombre , char fkey );
    virtual ~Semaforo();
    int inicializar (int valorInicial);
    int wait(); // decrementa
    int signal(); // incrementa
    void eliminar ();
    int add(short value); //incrementa en value
    void barrier();

};

#endif /* SEMAFORO_H_ */

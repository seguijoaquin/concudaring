#ifndef CONCUDARING_SEMAFORO_H
#define CONCUDARING_SEMAFORO_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

#define MAX_RETRIES 10

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

class Semaforo {

private:
    int id;

    int init(key_t key, int nsems);

public:
    Semaforo();
    Semaforo(const std::string& nombre, char key);
    ~Semaforo();

    int inicializar (int valorInicial) const;

    int p () const; // decrementa
    int v () const; // incrementa
    void eliminar () const;
    int wait() const {return p();}
    int signal() const {return v();}
    int numberOfProcessesWaiting();
    int add(int i);
};

#endif //CONCUDARING_SEMAFORO_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Semaforo.h"
#include <unistd.h>
#include <sys/types.h>

void printAndExitOnError(int status,const char* message) {
    if( status == -1 ){
        perror(message);
        exit(EXIT_FAILURE);
    }
}


Semaforo::Semaforo() {}

Semaforo :: Semaforo ( char* nombre, int valorInicial,char fkey ) {
    this->valorInicial = valorInicial;
    key_t clave = ftok (nombre,fkey);
    printAndExitOnError(clave,"Error Semaforo Constructor ftok");
    this->id = semget ( clave,1,0666 | IPC_CREAT);
    std::cout << "Create semaforo id:" << id << "en proceso :"<< getpid() <<std::endl;
    printAndExitOnError(this->id,"Error Semaforo Constructor smget");
    this->inicializar ();
}

Semaforo ::~ Semaforo () {
}

int Semaforo :: inicializar () {
    union semnum {
        int val;
        struct semid_ds* buf;
        ushort* array;
    };

    semnum init;
    init.val = this->valorInicial;
    int resultado = semctl ( this->id,0,SETVAL,init );
    printAndExitOnError(resultado,"Error inicializar semaforo semctl");
    //TODO: chequear
    return resultado;
}

int Semaforo :: p () {

    struct sembuf operacion;
    operacion.sem_num = 0; // numero de semaforo
    operacion.sem_op = -1; // restar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( this->id,&operacion,1 );
    printAndExitOnError(resultado,"Error inicializar semaforo semctl");
    //TODO: chequear
    return resultado;
}

int Semaforo :: v () {
    return add(1);
}

int Semaforo :: add (int value) {

    struct sembuf operacion;
    operacion.sem_num = 0; // numero de semaforo
    operacion.sem_op = value; // sumar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( this->id,&operacion,1 );
    printAndExitOnError(resultado,"Error inicializar semaforo semctl");
    //TODO: chequear

    return resultado;
}


void Semaforo :: eliminar () {
    int semctlStatus = semctl ( this->id,0,IPC_RMID );
    printAndExitOnError(semctlStatus,"Error semaforo eliminar semctl");
    //TODO: chequear
}

int Semaforo::numberOfProcessesWaiting() {
    int status = semctl(this->id,0,GETNCNT);
    if (status == -1 ){
        perror("numberOfPlayersWaiting GETNCNT");
    }
    return status;

}

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Semaforo.h"
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void printOnError(int status, const char *message) {
    //TODO: Agregar logger
    if( status == -1 ){
        std::cout <<"errno: "<< errno << std::endl;
        perror(message);
    }
}


Semaforo::Semaforo() {}

Semaforo :: Semaforo ( const std::string& nombre,char fkey ) {
    key_t clave = ftok (nombre.c_str(),fkey);
    printOnError(clave, "Error Semaforo Constructor ftok");
    this->id = semget ( clave,1,0666 | IPC_CREAT);
    //std::cout << "Create semaforo id:" << id << "en proceso :"<< getpid() <<std::endl;
    printOnError(this->id, "Error Semaforo Constructor smget");
}

Semaforo ::~ Semaforo () {
}

int Semaforo :: inicializar (int valorInicial) {
    union semnum {
        int val;
        struct semid_ds* buf;
        ushort* array;
    };

    semnum init;
    init.val = valorInicial;
    int resultado = semctl ( this->id,0,SETVAL,init );
    printOnError(resultado, "Error inicializar semaforo semctl");
    //TODO: chequear
    return resultado;
}

int Semaforo :: wait() {

    struct sembuf operacion;
    operacion.sem_num = 0; // numero de semaforo
    operacion.sem_op = -1; // restar 1 al semaforo
    operacion.sem_flg = 0;
    int resultado = semop ( this->id,&operacion,1 );
    printOnError(resultado, "Error Semaforo wait()");
    //TODO: chequear
    return resultado;
}

int Semaforo :: signal() {
    return add(1);
}

int Semaforo :: add (short value) {

    struct sembuf operacion;
    operacion.sem_num = 0; // numero de semaforo
    operacion.sem_op = value; // sumar value al semaforo
    operacion.sem_flg = 0;
    int resultado = semop ( this->id,&operacion,1 );
    printOnError(resultado, "Error Semaforo add()");
    //TODO: chequear

    return resultado;
}


void Semaforo :: eliminar () {
    int semctlStatus = semctl ( this->id,0,IPC_RMID );
    printOnError(semctlStatus, "Error semaforo eliminar semctl");
    //TODO: chequear
}

void Semaforo::barrier() {
    std::cout << "entro al barrier\n";
    this->add(0);
    std::cout << "salgo del barrier\n";
    //std::cout << "El valor del semaforo es:" << semctl(id,0,GETVAL)<< "\n";
}

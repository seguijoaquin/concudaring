#include "Semaforo.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "logger/logger.h"
#include "logger/mensajes.h"

Semaforo :: Semaforo ( const std::string& nombre, char key ) {
    key_t clave = ftok ( nombre.c_str(), key );
    this->id = semget ( clave,1,0666 | IPC_CREAT );
    if (this->id == -1) {
        std::string mensaje = std::string("Error en Constructor Semaforo->semget(): ") + std::string(strerror(errno)) + std::string("\n");
        throw mensaje;

    }
}

Semaforo :: Semaforo () {

}

Semaforo::~Semaforo() {

}

int Semaforo :: inicializar (int valorInicial) const {

    union semnum {
        int val;
        struct semid_ds* buf;
        ushort* array;
    };

    semnum init;
    init.val = valorInicial;
    int resultado = semctl ( this->id,0,SETVAL,init );
    if (resultado == -1) {
        std::string mensaje = std::string("Error en Iniciar Semaforo->semctl(): ") + std::string(strerror(errno)) + std::string("\n");
        throw mensaje;
    }
    return resultado;
}

int Semaforo :: signal() {
    return add(1);
}

void Semaforo :: eliminar () const {
    semctl ( this->id,0,IPC_RMID );
}

void Semaforo::barrier() {
    this->add(0);
}

int Semaforo::add(int i) {
  //std::cout << "Hago signal con ps: " << getpid() << " id semaforo: "<< id << std::endl;
  struct sembuf operacion;

  operacion.sem_num = 0;	// numero de semaforo
  operacion.sem_op  = i;
  operacion.sem_flg = 0;

  int resultado = semop ( this->id,&operacion,1 );
  if (resultado == -1) std::cout << "Proceso: "<< getpid() << " Error en signal(): " << strerror(errno)<< std::endl;
  return resultado;

}

int Semaforo :: wait() {
    return add(-1);
}

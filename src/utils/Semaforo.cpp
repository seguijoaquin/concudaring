#include "Semaforo.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "logger/logger.h"
#include "logger/mensajes.h"

Semaforo :: Semaforo ( const std::string& nombre, char key ) {
    key_t clave = ftok ( nombre.c_str(), key );
    this->id = semget ( clave,1,0666 | IPC_CREAT );
    //this->id = init(clave,1);
    Logger::getInstance()->insert(KEY_SEMAPHORE,"Se crea semaforo con sem.id: ", this->id);
    if (this->id == -1) {
        std::string mensaje = std::string("Error en Constructor Semaforo->semget(): ") + std::string(strerror(errno)) + std::string("\n");
        throw mensaje;

    }
}

Semaforo :: Semaforo () {

}

/*int Semaforo::init(key_t key, int nsems) { *//* key from ftok() *//*
    int i;
    union semun arg;
    struct semid_ds buf;
    struct sembuf sb;
    int semid;

    semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

    if (semid >= 0) { *//* we got it first *//*
        //std::cout << "semget valido" << std::endl;
        sb.sem_op = 1; sb.sem_flg = 0;
        arg.val = 1;

        for(sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++) {
            *//* do a semop() to "free" the semaphores. *//*
            *//* this sets the sem_otime field, as needed below. *//*
            if (semop(semid, &sb, 1) == -1) {
                int e = errno;
                semctl(semid, 0, IPC_RMID); *//* clean up *//*
                errno = e;
                return -1; *//* error, check errno *//*
            }
        }

    } else if (errno == EEXIST) { *//* someone else got it first *//*
        //std::cout << "semget invalido EEXISTS" << std::endl;
        int ready = 0;

        semid = semget(key, nsems, 0); *//* get the id *//*
        if (semid < 0) return semid; *//* error, check errno *//*

        *//* wait for other process to initialize the semaphore: *//*
        arg.buf = &buf;
        for(i = 0; i < MAX_RETRIES && !ready; i++) {
            semctl(semid, nsems-1, IPC_STAT, arg);
            if (arg.buf->sem_otime != 0) {
                ready = 1;
            } else {
                sleep(1);
            }
        }
        if (!ready) {
            errno = ETIME;
            return -1;
        }
    } else {
        return semid; *//* error, check errno *//*
    }
    return semid;
}*/

Semaforo::~Semaforo() {
    //std::cout << "Destructor Semaforo " << id << "en proceso :" << getpid() << std::endl;
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

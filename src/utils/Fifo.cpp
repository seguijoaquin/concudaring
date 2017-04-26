
#include "Fifo.h"

#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

using namespace std;

Fifo::Fifo(std::string nombre) {

    this->nombre = nombre;
    // se crea el fifo
    mknod(nombre.c_str(), S_IFIFO | 0666, 0);
    // se inicializa el descriptor en -1
    this->fileDes = -1;
}

Fifo::~Fifo() {
    if (this->fileDes != -1) {
        close(this->fileDes);
    }
}

void Fifo::abrir(int __oflag) {
    if (this->fileDes == -1) {
        this->fileDes = open(this->nombre.c_str(), __oflag);
    }
}

ssize_t Fifo::escribir(char* dato, size_t datoSize) {
    // se abre el file descriptor para escritura
    if (this->fileDes == -1) {
        this->fileDes = open(this->nombre.c_str(), O_WRONLY);
    }
    // se escriben los datos en el fifo
    ssize_t resultado = write(this->fileDes, (const void *) dato, datoSize);

    if (resultado == -1) {
      std::cout << "Error en Fifo:"<< this->nombre <<"->escribir(): " << strerror(errno) << " en proceso [" << getpid() << "]" << std::endl;
    }
    return resultado;
}

ssize_t Fifo::leer(char* buffer, size_t buffSize) {
    ssize_t resultado = 0;

    if (this->fileDes == -1) {
      this->fileDes = open(this->nombre.c_str(), O_RDONLY);
    }

    while (resultado == 0) {
        resultado = read(this->fileDes, (void *) buffer, buffSize);
    }

    if (resultado == -1) {
      std::cout << "Error en Fifo:" << this->nombre <<"->leer(): " << strerror(errno) << " en proceso [" << getpid() << "]" << std::endl;
    }
    return resultado;
}

void Fifo::cerrar() {
    close(this->fileDes);
    this->fileDes = -1;
}

void Fifo::eliminar() {
    unlink(this->nombre.c_str());
}

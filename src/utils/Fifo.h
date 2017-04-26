
#ifndef CONCUDARING_FIFO_H
#define CONCUDARING_FIFO_H

#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

class Fifo {
private:
    std::string nombre;
    int fileDes;

public:
    Fifo ( std::string nombre );
    virtual ~Fifo();

    ssize_t escribir ( char* dato,size_t datoSize );
    ssize_t leer ( char* buffer,size_t buffSize );
    void abrir(int _oflag);
    void cerrar ();
    void eliminar ();
};

#endif //CONCUDARING_FIFO_H

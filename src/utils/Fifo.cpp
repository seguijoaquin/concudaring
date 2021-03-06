#include "Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );
}

Fifo::Fifo() {

}

Fifo::~Fifo() {
}

void Fifo::cerrar() {
	close ( fd );
	fd = -1;
}

void Fifo::eliminar() const {

	int status =unlink ( nombre.c_str() );
    if(status == -1 ){
        perror("FIFO Unlink error");
    }

}




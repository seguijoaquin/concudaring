#include "FifoEscritura.h"

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
	fd = open ( nombre.c_str(),O_WRONLY );
}

ssize_t FifoEscritura::escribir(const void* buffer,const ssize_t buffsize) const {
	int status = write ( fd,buffer,buffsize );
	if ( status == -1 ) perror("Error write FIFO");
	return status;
}

FifoEscritura::FifoEscritura() {
}

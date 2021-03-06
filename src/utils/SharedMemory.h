#ifndef MEMORIACOMPARTIDA2_H_
#define MEMORIACOMPARTIDA2_H_

#define SHM_OK			 0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <iostream>


template <class T> class SharedMemory {
private:
	int shmId;
	T*	ptrDatos;

	int cantidadProcesosAdosados () const;

public:
	//TODO: Borrar
	void printID();
	SharedMemory ();
	~SharedMemory ();
	int create(const std::string &archivo, const char letra, int tamanio);
	void write(const T &dato);
	T read() const;
	T& operator[](std::size_t idx);
	const T& operator[](std::size_t idx) const;


};

template <class T> SharedMemory<T> :: SharedMemory() : shmId(0), ptrDatos(NULL) {
}

template <class T> SharedMemory<T> :: ~SharedMemory() {
	// detach del bloque de memoria
	int status = shmdt ( static_cast<void*> (this->ptrDatos) );
	if(status == -1 ) perror("Error shmdt:");
	int procAdosados = this->cantidadProcesosAdosados ();
	if ( procAdosados == 0 ) {
		shmctl ( this->shmId,IPC_RMID,NULL );
	}
}

template <class T> int SharedMemory<T> :: create(const std::string &archivo, const char letra, int tamanio) {

	// generacion de la clave
	key_t clave = ftok ( archivo.c_str(),letra );
	if ( clave == -1 )
		return ERROR_FTOK;
	else {
		// creacion de la memoria compartida
		this->shmId = shmget ( clave,sizeof(T)*tamanio,0644|IPC_CREAT );

		if ( this->shmId == -1 )
			return ERROR_SHMGET;
		else {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* ptrTemporal = shmat ( this->shmId,NULL,0 );

			if ( ptrTemporal == (void *) -1 ) {
				return ERROR_SHMAT;
			} else {
				this->ptrDatos = static_cast<T*> (ptrTemporal);
				return SHM_OK;
			}
		}
	}
}


template <class T> void SharedMemory<T> :: write(const T &dato) {
	* (this->ptrDatos) = dato;
}

template <class T> T SharedMemory<T> :: read() const {
	return ( *(this->ptrDatos) );
}

template <class T> const T& SharedMemory<T>::operator[](std::size_t idx) const {
	return this->ptrDatos[idx];
}


template <class T> T& SharedMemory<T>::operator[](std::size_t idx){
	return this->ptrDatos[idx];
}

template <class T> int SharedMemory<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}

template <class T> void SharedMemory<T> ::printID() {
	std::cout << this->shmId << std::endl;
}


#endif /* MEMORIACOMPARTIDA2_H_ */

//
// Created by diego on 26/04/17.
//

#include "CommunicationChannel.h"
#include <string>
#include <iostream>
#include <cstring>

CommunicationChannel::CommunicationChannel(std::string nombre,int numberOfProcesses, int _id):id(_id) {
    std::string aux;

    for (int i = 0; i < numberOfProcesses  ; ++i) {
        aux = nombre;
        aux.append(std::to_string(i));
        if ( i != id){
            writeFifos.push_back(FifoEscritura(aux));
            writeFifos[i].abrir();
        } else{
            //pusheo un FIFO de juguete para poder manejarlos segun el id en caso de ser necesario
            //es decir si quiero escribir en el fifo del que tiene id 10 hago writeFifos[10].escribir
            writeFifos.push_back(FifoEscritura());
            myReadFifo= FifoLectura(aux);
            myReadFifo.abrir();
        }
    }
}
CommunicationChannel::CommunicationChannel() {

}



void CommunicationChannel::cerrar() {
    myReadFifo.cerrar();
    for (int i = 0; i < writeFifos.size() ; ++i) {
        if (i != id)
            writeFifos[i].cerrar();
    }
}

void CommunicationChannel::eliminar() {
    myReadFifo.eliminar();
}



void serialize(int32_t id, std::string msg, char* buffer){
    int32_t *int_buffer = (int32_t*) buffer;
    *int_buffer = id;
    int_buffer++;
    buffer = (char*) int_buffer;
    strcpy(buffer,msg.c_str());
}

MSG_t deserialize(char* buffer,int length){
    MSG_t message;

    int32_t* int_buffer = (int32_t*) buffer;
    message.id = *int_buffer;
    int_buffer++;
    char* msg = (char*) int_buffer;
    message.message = std::string(msg);

    message.message.resize(length);
    return message;
}


int CommunicationChannel::sendToAll(std::string data) {
    char buffer[BUFFSIZE];
    memset(buffer,0,BUFFSIZE);
    serialize((int32_t)id,data,buffer);
    ssize_t msgSize = data.length()+sizeof(int32_t);


    for (int i = 0; i < writeFifos.size() ; ++i) {
        if (i != id){
            writeFifos[i].escribir( static_cast<const void*> (buffer),msgSize);
        }
    }
    return 0;
}

MSG_t CommunicationChannel::receive(ssize_t length) {
    char buffer[BUFFSIZE];
    memset(buffer,0,BUFFSIZE);
    ssize_t expectedSize = length + sizeof(int32_t);
    ssize_t bytes_readed =  myReadFifo.leer( buffer,expectedSize);
    MSG_t message = deserialize(buffer,bytes_readed - sizeof(int32_t));
    return message;
}

CommunicationChannel::~CommunicationChannel() {

}



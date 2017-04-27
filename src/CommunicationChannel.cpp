//
// Created by diego on 26/04/17.
//

#include "CommunicationChannel.h"
#include <string>
#include <iostream>

CommunicationChannel::CommunicationChannel(int numberOfProcesses, int _id):id(_id) {
    std::string nombre = "./communicationFifo";
    std::string aux;

    for (int i = 0; i < numberOfProcesses  ; ++i) {
        aux = nombre;
        aux.append(std::to_string(i));
        if ( i != id){
            writeFifos.push_back(FifoEscritura(aux));
            writeFifos[i].abrir();
        } else{
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

int CommunicationChannel::sendToAll(std::string data) {
    for (int i = 0; i < writeFifos.size() ; ++i) {
        if (i != id){
            int pid = fork();
            if (pid == 0 ){
                writeFifos[i].escribir( static_cast<const void*> (data.c_str()),data.length());
                cerrar();
                exit(0);
            }
        }

    }
    return 0;
}

std::string CommunicationChannel::receive(int length) {
    char buffer[BUFFSIZE];
    ssize_t bytes_readed =  myReadFifo.leer( buffer,length);

    std::string message(buffer);
    message.resize(bytes_readed);

    return message;
}

CommunicationChannel::~CommunicationChannel() {

}

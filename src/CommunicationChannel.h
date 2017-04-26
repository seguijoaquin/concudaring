//
// Created by diego on 26/04/17.
//

#ifndef CONCUDARING_COMMUNICATIONCHANNEL_H
#define CONCUDARING_COMMUNICATIONCHANNEL_H


#include <vector>
#include "FifoEscritura.h"
#include "FifoLectura.h"

class CommunicationChannel {
private:
    std::vector<FifoEscritura> writeFifos;
    FifoLectura myReadFifo;
    int id;
    static const int BUFFSIZE = 100;
public:

    CommunicationChannel(int numberOfProcesses,int id);
    ~CommunicationChannel();
    void cerrar();
    void eliminar();
    int sendToAll(std::string data);
    std::string receive(int length);
};


#endif //CONCUDARING_COMMUNICATIONCHANNEL_H

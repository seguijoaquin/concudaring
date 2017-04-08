#include <unistd.h>
#include <iostream>
#include "Semaforo.h"

int main (){
    Semaforo semaforo("/bin/ls",0);

    int pid = fork();

    if ( pid == 0 ){
        semaforo.p();
    } else {

        int pid2 = fork();

        if ( pid2 == 0 ){
            semaforo.p();
        } else {
            sleep(1);
            std::cout <<"number of players waiting: " <<semaforo.numberOfPlayersWaiting() << std::endl ;
            semaforo.add(2);

        }
    }

    return 0;

}
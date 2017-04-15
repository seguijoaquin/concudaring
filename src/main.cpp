#include <iostream>
#include <cstring>
#include "concudaring.h"
#include "utils/constants.h"
#include "utils/types.h"
#include "utils/SharedMemory.h"


bool createTmpFile() {
  FILE* file = fopen(FILE_CONCUDARING, "w");

  if (!file) {
    std::cerr << "Error en fopen():" << strerror(errno) << '\n';
    return false;
  } else {
    fclose(file);
    return true;
  }
}

int main(int argc, char** argv) {

  if (createTmpFile()) {
   SharedMemory<Game_t> sharedMemory;
  int memState = sharedMemory.create(FILE_CONCUDARING,KEY_MEMORY,2);
  if (memState == SHM_OK) {
      Concudaring concudaring(atoi(argv[1]));
      //concudaring.start();
  } else {
      //ERROR AL INICIALIZAR MEMORIA COMPARTIDA
      //Liberar recursos tomados y salir
  }
  } else {
    //ERROR AL QUERER CREAR ARCHIVO TEMPORAL PARA FTOK
  }
}

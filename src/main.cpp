#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <getopt.h>
#include "concudaring.h"
#include "utils/constants.h"
#include "utils/types.h"
#include "utils/SharedMemory.h"
#include "utils/logger/logger.h"
#include "utils/logger/mensajes.h"


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

void printMinPlayersError() {
    Logger::getInstance()->insert(KEY_ERROR, ERROR_MIN_PLAYERS);
}

void printHelp() {
  std::cout << "Para lanzar el juego: ./Concudaring -p <cantidad_jugadores>" << '\n';
  std::cout << "NOTA: El juego no puede iniciar con menos de 4 jugadores." << '\n';
  std::cout << "Para consultar cantidad de cartas de cada jugador: ./Concudaring -j" << '\n';
}


static const char *optString = "jp:h";

static const struct option longOpts[] = {
  { "judge", no_argument, NULL, 'j' },
  { "players", required_argument, NULL,'p' },
  { "help", no_argument, NULL, 'h' },
  { NULL, no_argument, NULL, 0 }
};

int main(int argc, char** argv) {

  __pid_t pid_main = getpid();

  Semaforo thereIsCard(FILE_CONCUDARING,KEY_SEM_THERE_IS_CARD);
  Semaforo endOfTurnGathering(FILE_CONCUDARING,KEY_SEM_END_OF_TURN_GATHERING);
  Semaforo writeIdLosser(FILE_CONCUDARING,KEY_SEM_WRITE_LOSER);
  Semaforo readIdLosser(FILE_CONCUDARING,KEY_SEM_READ_LOSER);
  thereIsCard.inicializar(0);
  endOfTurnGathering.inicializar(0);
  writeIdLosser.inicializar(1);
  readIdLosser.inicializar(0);

  if (createTmpFile()) {
    SharedMemory<Game_t> sharedMemory;
    int memState = sharedMemory.create(FILE_CONCUDARING,KEY_MEMORY,2);
    if (memState == SHM_OK) {


      Logger::getInstance()->insert(KEY_GAME, MENSAJE_INICIO_JUEGO);

      int opt = 0;
      int longIndex;
      opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

      while (opt != -1) {
        switch (opt) {
          case 'p': {
            int numberPlayers = atoi(optarg);
            if (numberPlayers >= 4) {
              Logger::getInstance()->insert(KEY_GAME,MENSAJE_CANTIDAD_JUGADORES,atoi(optarg));
              Concudaring concudaring(numberPlayers);
            } else {
              printMinPlayersError();
            }
            break;
          }
          case 'j': {
            Logger::getInstance()->insert(KEY_JUDGE, CONSULTA_JUEZ);
            break;
          }
          case 'h': {
            printHelp();
            break;
          }
        }
        break;
      }


    } else {
      //ERROR AL INICIALIZAR MEMORIA COMPARTIDA
      Logger::getInstance()->insert(KEY_ERROR, ERROR_INICIO_MEMORIA);
      //Liberar recursos tomados y salir
    }
  } else {
    Logger::getInstance()->insert(KEY_ERROR, ERROR_CREACION_TMP_FILE);
    //ERROR AL QUERER CREAR ARCHIVO TEMPORAL PARA FTOK
  }
  if (pid_main == getpid()) {
    //Libero recursos
    thereIsCard.eliminar();
    endOfTurnGathering.eliminar();
    writeIdLosser.eliminar();
    readIdLosser.eliminar();
    Logger::getInstance()->insert(KEY_GAME, MENSAJE_FIN_JUEGO);
  }
}

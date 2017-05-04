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

const double JUDGE_DEFAULT_TIME_BETWEEN_CHECKS = 0.1;

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
  std::cout << "Para setear el tiempo entre las consultas del Arbitro/Juez utilizar: -j" << '\n';
  std::cout << "Ejemplo: ./Concudaring -j 0.5 -p 4 " << '\n';
  std::cout << "NOTA: El parametro -j debe estar antes que el -p. Sino se utilizara el valor por defecto 0.1" << '\n';
}


static const char *optString = "j:p:h";

static const struct option longOpts[] = {
  { "judge", required_argument, NULL, 'j' },
  { "players", required_argument, NULL,'p' },
  { "help", no_argument, NULL, 'h' },
  { NULL, no_argument, NULL, 0 }
};

int main(int argc, char** argv) {

  __pid_t pid_main = getpid();

  //Solo para borrar en cada arranque el archivo log.
  system("chmod +x deleteFile.sh");
  system("./deleteFile.sh");

  if (createTmpFile()) {
    SharedMemory<Game_t> sharedMemory;
    int memState = sharedMemory.create(FILE_CONCUDARING,KEY_MEMORY,2);
    if (memState == SHM_OK) {


      Logger::getInstance()->insert(KEY_GAME, MENSAJE_INICIO_JUEGO);

      int opt = 0;
      int longIndex;
      opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

      double judgeTimeBetweenChecks = JUDGE_DEFAULT_TIME_BETWEEN_CHECKS;

      while (opt != -1) {
        switch (opt) {
          case 'j': {
            judgeTimeBetweenChecks = atof(optarg);
            Logger::getInstance()->insert(KEY_JUDGE, CONSULTA_JUEZ);
            break;
          }
          case 'p': {
            int numberPlayers = atoi(optarg);
            if (numberPlayers >= 4) {
              Logger::getInstance()->insert(KEY_GAME,MENSAJE_CANTIDAD_JUGADORES,atoi(optarg));
              Concudaring concudaring(numberPlayers,judgeTimeBetweenChecks);
            } else {
              printMinPlayersError();
            }
            break;
          }
          case 'h': {
            printHelp();
            break;
          }
        }
        opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
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
    Logger::getInstance()->insert(KEY_GAME, MENSAJE_FIN_JUEGO);
  }
}

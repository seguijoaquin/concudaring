//
// Created by joaquin on 27/10/16.
//

#ifndef RESTORRENTE_LOGGER_H
#define RESTORRENTE_LOGGER_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define KEY_PLAYER "Jugador"
#define KEY_ERROR "ERROR"
#define KEY_GAME "Game"
#define KEY_JUDGE "Juez"
#define KEY_SEMAPHORE "Semaforo"

const std::string TURN_KEY("Turno");

class Logger {

private:

    static Logger* logger;
    std::fstream ioFile;
    Logger();
    std::string getTime();

public:
    static Logger* getInstance();
    void debug(std::string message);
    void info(std::string message);
    void warn(std::string message);
    void error(std::string message);
    void insert(std::string key, std::string value, int param);
    void insert(std::string key,int id, int turn,std::string value);
    void insert(std::string key,int id, int turn,std::string value,int param);
    void insert(std::string key, std::string value);
    virtual ~Logger();

};

#endif //RESTORRENTE_LOGGER_H

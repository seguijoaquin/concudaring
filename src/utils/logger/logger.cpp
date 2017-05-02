#include "logger.h"

Logger* Logger::logger = NULL;

Logger::Logger() {
    std::string c;
    ioFile.open("logger.log", std::ios_base::out | std::ios_base::app);
}

Logger* Logger::getInstance() {
    if (Logger::logger == NULL)
        Logger::logger = new Logger();
    return Logger::logger;
}

Logger::~Logger() {
    ioFile.close();
}

std::string Logger::getTime() {

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[50];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 50, "%d/%m/%Y %H:%M:%S - ", timeinfo);

    return std::string(buffer);
}

void Logger::warn(std::string message) {
    std::string key = "WARN";
    Logger::insert(key, message);
}

void Logger::info(std::string message) {
    std::string key = "INFO";
    Logger::insert(key, message);
}

void Logger::debug(std::string message) {
    std::string key = "DEBUG";
    Logger::insert(key, message);
}

void Logger::error(std::string message) {
    std::string key = "ERROR";
    Logger::insert(key, message);
}

void Logger::insert(std::string key, std::string value, int param) {
    std::stringstream log;
    log << value << param;
    Logger::insert(key, log.str());
}


//TODO: Refactor
void Logger::insert(std::string key,int id,int turn,std::string value) {
    if (ioFile.is_open()) {

        std::stringstream ss;
        ss << "[" << key << ":" << id << "] "
           << "[" << "Turno" << ":" << turn << "] "
           << value;

        std::cout << ss.str() << std::endl;

        ioFile << Logger::getTime() << ss.str();
        ioFile.flush();
    }
}



void Logger::insert(std::string key, std::string value) {
    if (ioFile.is_open()) {

        std::cout << "[" << key << ":" << getpid() << "] " << value
                  << std::endl;

        ioFile << Logger::getTime() << "[" << key << ":" << getpid() << "] "
               << value << std::endl;
        ioFile.flush();
    }
}

void Logger::insert(std::string key, int id, int turn, std::string value, int param) {
    if (ioFile.is_open()) {

        std::stringstream ss;
        ss << "[" << key << ":" << id << "] "
           << "[" << TURN_KEY << ":" << turn << "] "
           << value << param;

        std::cout << ss.str() << std::endl;

        ioFile << Logger::getTime() << ss.str();
        ioFile.flush();
    }
}

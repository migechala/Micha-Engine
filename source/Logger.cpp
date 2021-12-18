#include <fstream>
#include <string>
#include <iostream>

#include "Logger.h"

void Logger::m_deleteLastLog(){
    m_file.open("../log.txt");
    m_file << "\n";
    m_file.close();
}

void Logger::m_writeToFile(const std::string &write) {
    m_file.open("../log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    m_file << write + "\n";
    m_file.close();
}
void Logger::resetLogger(){
    m_deleteLastLog();
}
void Logger::Log(const std::string &msg, LOG_VALUES severity, const char* filename, int line) {
    std::string _msg;
    switch (severity) {
        case LOG_VALUES::INFO:
            _msg = "INFO: ";
            break;
        case LOG_VALUES::WARNING:
            _msg = "WARN: ";
            break;
        case LOG_VALUES::ERROR:
            _msg = "ERR: ";
            break;
    }
    _msg.append(msg);
    _msg.append("               >>");
    _msg.append(filename);
    _msg.append(":");
    _msg.append(std::to_string(line));
    m_writeToFile(_msg);
    std::cout << _msg << std::endl;
}

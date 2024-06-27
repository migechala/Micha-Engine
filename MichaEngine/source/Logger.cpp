#include "Logger.h"

#include <fstream>
#include <iostream>
#include <string>

void Logger::deleteLastLog() {
  file.open("../log.txt");
  file << "\n";
  file.close();
}

void Logger::setLogLevel(LOG_LEVEL level) { ll = level; }

void Logger::writeToFile(const std::string &write) {
  file.open("../log.txt",
            std::fstream::in | std::fstream::out | std::fstream::app);
  file << write + "\n";
  file.close();
}
void Logger::resetLogger() { deleteLastLog(); }
void Logger::log(const std::string &msg, LOG_VALUES severity, LOG_LEVEL level,
                 const char *filename, int line) {
  if (ll < level) return;
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
  _msg.append("               >>| ");
  _msg.append(filename);
  _msg.append(":");
  _msg.append(std::to_string(line));
  writeToFile(_msg);
  std::cout << _msg << std::endl;
}

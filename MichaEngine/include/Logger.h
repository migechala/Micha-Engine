//
// Created by migecha on 7/17/21.
//

#pragma once

#include <fstream>
#include <string>

enum LOG_VALUES { INFO, WARNING, ERROR };
enum LOG_LEVEL { PRIORITY, MEDIUM, LOW };

namespace Logger {
static std::ofstream m_file;
static int ll = LOG_LEVEL::LOW;
void resetLogger();
void m_writeToFile(const std::string &write);
void m_deleteLastLog();
void setLogLevel(LOG_LEVEL level);
void Log(const std::string &msg, LOG_VALUES severity, LOG_LEVEL level,
         const char *filename, int line);
};  // namespace Logger

#define LOG_INFO(msg, level) \
  Logger::Log(msg, LOG_VALUES::INFO, level, __FILE__, __LINE__);
#define LOG_WARN(msg, level) \
  Logger::Log(msg, LOG_VALUES::WARNING, level, __FILE__, __LINE__);
#define LOG_ERR(msg) \
  Logger::Log(msg, LOG_VALUES::ERROR, LOG_LEVEL::PRIORITY, __FILE__, __LINE__);
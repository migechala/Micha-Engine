//
// Created by migecha on 7/17/21.
//

#pragma once

#include <fstream>
#include <string>

enum LOG_VALUES { INFO, WARNING, ERROR };

namespace Logger {
static std::ofstream m_file;
void resetLogger();
void m_writeToFile(const std::string &write);
void m_deleteLastLog();
void Log(const std::string &msg, LOG_VALUES severity, const char *filename,
         int line);
};  // namespace Logger

#define LOG_INFO(msg) Logger::Log(msg, LOG_VALUES::INFO, __FILE__, __LINE__);
#define LOG_WARN(msg) Logger::Log(msg, LOG_VALUES::WARNING, __FILE__, __LINE__);
#define LOG_ERR(msg) Logger::Log(msg, LOG_VALUES::ERROR, __FILE__, __LINE__);
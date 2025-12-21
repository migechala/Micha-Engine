// Created by migecha on 7/17/21.

#pragma once

#include <fstream>
#include <string>

#include "imgui.h"

#define CHECK_RESULT(fnc)                                                                                              \
  {                                                                                                                    \
    auto res = fnc;                                                                                                    \
    if (res != 0) {                                                                                                    \
      std::cout << "ERR: " << __FILE__ << "(" << __LINE__ << ") SDL_Error(" << SDL_GetError() << ")  err: " << res     \
                << " in " << #fnc << std::endl;                                                                        \
      exit(-2);                                                                                                        \
    }                                                                                                                  \
  }

struct LogApp {
  ImGuiTextBuffer buf;
  ImGuiTextFilter filter;
  ImVector<int> lineOffsets;
  bool autoScroll;

  // Constructor to initialize the LogApp
  LogApp();

  // Clears the log buffer
  void clear();

  // Adds a log entry with formatted message
  void addLog(std::string fmt, ...);

  // Draws the log window with a title and open flag
  void draw(const char *title, bool *p_open = nullptr);
};

// Enumeration for log values (severity)
enum LOG_VALUES { INFO, WARNING, ERROR };

// Enumeration for log levels (priority)
enum LOG_LEVEL { PRIORITY, MEDIUM, LOW };

namespace Logger {
static std::ofstream file;      // File stream for logging
static LogApp logApp;           // LogApp instance for managing logs
static int ll = LOG_LEVEL::LOW; // Default log level

// Resets the logger (closes file, clears logs)
void resetLogger();

// Writes a message to the log file
void writeToFile(const std::string &write);

// Writes a message to the window log
void writeToWindow(const std::string &write);

// Deletes the last log entry
void deleteLastLog();

// Sets the log level for filtering messages
void setLogLevel(LOG_LEVEL level);

// Displays the log application window
void showLogApp(bool *open_app);

// Logs a message with severity, level, and source details
void log(const std::string &msg, LOG_VALUES severity, LOG_LEVEL level, const char *filename, int line);
} // namespace Logger

// Macros for logging with different severities
#define LOG_INFO(msg, level) Logger::log(msg, LOG_VALUES::INFO, level, __FILE__, __LINE__);

#define LOG_WARN(msg, level) Logger::log(msg, LOG_VALUES::WARNING, level, __FILE__, __LINE__);

#define LOG_ERR(msg) Logger::log(msg, LOG_VALUES::ERROR, LOG_LEVEL::PRIORITY, __FILE__, __LINE__);

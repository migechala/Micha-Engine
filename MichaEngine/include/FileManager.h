#pragma once

#include <map>
#include <string>
#include <vector>
class FileManager {
  static FileManager *instance;
  std::unordered_map<std::string, std::string> settingsMap;

public:
  static FileManager *getInstance();
  static int getFileCountInDirectory(const std::string &path);
  std::unordered_map<std::string, std::string> readSettings(const std::string &path);
  std::string getSettings(std::string key);
  static std::vector<std::string> getFiles(const std::string &path);
  ~FileManager();
};
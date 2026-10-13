#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
class FileManager {
  std::unordered_map<std::string, std::string> settingsMap;

public:
  static FileManager *getInstance();
  static int getFileCountInDirectory(const std::string &path);
  std::unordered_map<std::string, std::string> readSettings(const std::string &path);
  std::string getSettings(std::string key);
  static std::vector<std::string> getFiles(const std::string &path);
  static std::filesystem::path getPath(const std::string &path);
  ~FileManager() = default;
};
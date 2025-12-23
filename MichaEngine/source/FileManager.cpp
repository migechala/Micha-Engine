#include "FileManager.h"

#include <dirent.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>

#include "Logger.h"

FileManager *FileManager::instance = nullptr;
static const std::filesystem::path BASE_DIR = BASE_PATH;

FileManager *FileManager::getInstance() {
  if (instance == nullptr) {
    instance = new FileManager();
  }
  return instance;
}

int FileManager::getFileCountInDirectory(const std::string &path) {
  std::filesystem::path p(path);
  if (p.is_relative()) {
    p = std::filesystem::path(BASE_DIR) / p;
    ;
  }
  if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p)) {
    return -1;
  }
  return static_cast<int>(std::distance(std::filesystem::directory_iterator{p}, std::filesystem::directory_iterator{}));
}

std::vector<std::string> FileManager::getFiles(const std::string &path) {
  std::filesystem::path p(path);
  if (p.is_relative()) {
    p = std::filesystem::path(BASE_DIR) / p;
  }
  std::vector<std::string> files;

  std::transform(std::filesystem::directory_iterator{p}, std::filesystem::directory_iterator{},
                 std::back_inserter(files), [](const auto &entry) { return entry.path().filename().string(); });

  return files;
}

std::filesystem::path FileManager::getPath(const std::string &path) {
  std::filesystem::path p(path);
  if (p.is_relative()) {
    p = std::filesystem::path(BASE_DIR) / p;
  }
  return p;
}

std::unordered_map<std::string, std::string> FileManager::readSettings(const std::string &path) {
  std::ifstream settings;
  std::filesystem::path p(getPath(path));

  settings.open(p);
  std::string line;
  if (settings.is_open()) {
    while (getline(settings, line)) {
      // Write all content of settings file to vector fileContents
      std::string key = line.substr(0, line.find(":"));
      key = key.erase(0, key.find_first_not_of(" \t\n\v\f\r")).erase(key.find_last_not_of(" \t\n\v\f\r") + 1);
      std::string value = line.substr(line.find(":") + 1);
      value = value.erase(0, value.find_first_not_of(" \t\n\v\f\r")).erase(value.find_last_not_of(" \t\n\v\f\r") + 1);
      settingsMap[key] = value;
    }
  } else {
    throw std::invalid_argument("Failed to open file " + path);
  }
  return settingsMap;
}
std::string FileManager::getSettings(std::string key) {
  if (settingsMap.empty()) {
    LOG_ERR("Settings map is empty or is not initialized. Ensure that you have "
            "called readSettings() before calling getSettings()");
    return "";
  }
  if (settingsMap.find(key) == settingsMap.end()) {
    LOG_ERR("Key " + key + " not found in settings map");
    return key + " NOT FOUND";
  }
  return settingsMap[key];
}

FileManager::~FileManager() { delete instance; }

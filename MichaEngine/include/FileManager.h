#pragma once

#include <string>
#include <vector>
class FileManager {
  static FileManager *instance;
  static void removeCharacters(std::string &str, char c);

 public:
  static FileManager *getInstance();

  static int getFileCountInDirectory(const std::string &path);
  std::string getSettingsFromJson(std::string path, std::string tree,
                                  std::string child);
  static std::vector<std::string> getFiles(const std::string &path);
  ~FileManager();
};
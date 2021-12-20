#pragma once

#include <vector>
#include <string>
class FileManager
{
    static FileManager *m_instance;
    void removeCharacters(std::string &str, char c);

public:
    static FileManager *getInstance();

    int getFileCountInDirectory(const std::string &p_path);
    std::string getSettingsFromJson(std::string path, std::string tree, std::string child);
    std::vector<std::string> getFiles(const std::string &p_path);
};
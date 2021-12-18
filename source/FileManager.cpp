#include "FileManager.h"
#include "Logger.h"
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <fstream>

FileManager *FileManager::m_instance = nullptr;

FileManager *FileManager::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new FileManager();
    }
    return m_instance;
}

int FileManager::getFileCountInDirectory(const std::string &p_path)
{
    DIR *dp;
    int i = 0;
    struct dirent *ep;
    dp = opendir(p_path.c_str());

    if (dp != nullptr)
    {
        while ((ep = readdir(dp)))
            i++;

        (void)closedir(dp);
    }
    else
    {

        perror("Couldn't open the directory");
        return -1;
    }

    return i;
}

std::vector<std::string> FileManager::getFiles(const std::string &p_path)
{
    std::vector<std::string> files;
    struct dirent *entry;
    DIR *dir = opendir(p_path.c_str());

    if (dir == nullptr)
    {
        return files;
    }
    while ((entry = readdir(dir)) != nullptr)
    {
        files.push_back(entry->d_name);
    }
    closedir(dir);
    return files;
}

std::string FileManager::getSettingsFromJson(std::string path, std::string tree, std::string child)
{
    char START = '{', END = '}', NEXT = ',', VALUE = ':';
    std::vector<std::string> fileContents;
    std::string ret = ":(";

    std::ifstream json;
    json.open(path);
    std::string line;
    if (json.is_open())
    {
        while (getline(json, line))
        {
            // Write all content of json file to vector fileContents
            fileContents.push_back(line);
        }
    }
    else
    {
        LOG_ERR("Failed to open config file " + path);
    }
    json.close();

    bool continueLoops = true;
    for (int i = 0; i < fileContents.size() && continueLoops; ++i)
    {
        //Loop through each value in fileContents to find tree string
        std::string line = fileContents[i];
        if (line.find(tree) != std::string::npos)
        {
            int skip = i;            
            // Get skip from index i to the first opening brace ( { )
            for (; fileContents[skip].find(START) == std::string::npos && skip < fileContents.size(); ++skip);

            // Get closest closing brace after skip ( } )
            for (int j = skip; fileContents[j].find(END) == std::string::npos && j < fileContents.size(); ++j)
            {
                if (fileContents[j].find("\"" + child + "\"") != std::string::npos)
                {
                    std::string line = fileContents[j];
                    size_t pos = line.find(child);
                    size_t Ipos = 1;
                    
                    for(; Ipos + pos < line.size() && line[Ipos + pos] != '"'; ++Ipos);

                    line = line.substr(Ipos + pos + 2);
                    // remove " and ,
                    line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
                    line.erase(std::remove(line.begin(), line.end(), ','), line.end());
                    continueLoops = false;
                    ret = line;
                    break;
                }
            }
        }
    }
    return ret;
}
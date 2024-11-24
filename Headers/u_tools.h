#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, std::map<std::string, std::string>> readIniFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::map<std::string, std::map<std::string, std::string>> Ini_data;
    std::string line, section, key, value;
    std::map<std::string, std::string> sectionData;

    while (std::getline(file, line)) {
        if (line[0] == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
            Ini_data[section] = sectionData;
            sectionData.clear();
        }

        else if (line.find('=') != std::string::npos) {
            size_t eqPos = line.find('=');
            key = line.substr(0, eqPos);
            value = line.substr(eqPos + 1);
            sectionData[key] = value;
        }
    }

    file.close();
    return Ini_data;
}
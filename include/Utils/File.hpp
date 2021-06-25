#ifndef FILE_HPP
#define FILE_HPP

#include "Core/Log.hpp"
#include <fstream>
#include <sstream>

namespace te {

inline void fileToString(const std::string& path, std::string& string) {
    std::ifstream file;
    // ensure ifstream objects can throw exceptions:
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        file.open(path);
        std::stringstream fileStream;
        // read file's buffer contents into streams
        fileStream << file.rdbuf();
        // close file handlers
        file.close();
        // convert stream into string
        string = fileStream.str();
    } catch (std::ifstream::failure& e) {
        TE_CORE_ERROR("File not successfully read!");
    }
}

}  // namespace te

#endif

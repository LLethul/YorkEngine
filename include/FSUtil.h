#ifndef FSUTIL_H_
#define FSUTIL_H_

#include <cstddef>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <sys/stat.h>
#include <vector>

namespace FSUtil {
    inline std::vector<char> read_binary(const std::string& filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filepath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    inline std::string read_string(const std::string& path) {
        std::ifstream file(path, std::ios::ate, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file at path: " + path);
        };

        size_t fsize = static_cast<size_t>(file.tellg());
        std::string str;

        file.seekg(0);
        file.read(str.data(), fsize);
        file.close();

        return str;
    }
}

#endif // FSUTIL_H_
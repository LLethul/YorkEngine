#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <string>

namespace YorkLogger {
    inline void info(std::string message) {
        std::cout << "[YORK INFO] => " << message << std::endl;
    }
    
    inline void warn(std::string message) {
        std::cout << "[YORK WARN] => " << message << std::endl;
    }
    inline void error(std::string message) {
        std::cout << "[YORK ERROR] => " << message << std::endl;
    }

}

#endif // LOGGER_H_
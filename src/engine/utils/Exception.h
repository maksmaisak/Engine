//
// Created by Maksym Maisak on 2019-02-01.
//

#ifndef SAXION_Y2Q2_RENDERING_EXCEPTION_H
#define SAXION_Y2Q2_RENDERING_EXCEPTION_H

#include <exception>

namespace utils {

    struct Exception : public std::exception {

        Exception(const std::string& message) : message(message) {}

        const char* what() const throw() override {
            return message.c_str();
        }

        std::string message;
    };
}

#endif //SAXION_Y2Q2_RENDERING_EXCEPTION_H

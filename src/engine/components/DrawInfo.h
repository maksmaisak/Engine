//
// Created by Maksym Maisak on 25/10/18.
//

#ifndef SAXION_Y2Q1_CPP_DRAWINFO_H
#define SAXION_Y2Q1_CPP_DRAWINFO_H

#include <memory>
#include <SFML/Graphics.hpp>

namespace en {

    struct DrawInfo {

        std::shared_ptr<sf::Drawable> drawablePtr;
    };
}

#endif //SAXION_Y2Q1_CPP_DRAWINFO_H

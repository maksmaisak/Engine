//
// Created by Maksym Maisak on 27/9/18.
//

#include "Input.h"

namespace en {

    using Keys = sf::Keyboard;

    sf::Vector2f getMoveInput() {

        return {
            getAxisHorizontal(),
            getAxisVertical()
        };
    }

    float getAxisHorizontal() {

        if (Keys::isKeyPressed(Keys::Right) || Keys::isKeyPressed(Keys::D)) return  1.f;
        if (Keys::isKeyPressed(Keys::Left ) || Keys::isKeyPressed(Keys::A)) return -1.f;

        return 0.f;
    }

    float getAxisVertical() {

        if (Keys::isKeyPressed(Keys::Up  ) || Keys::isKeyPressed(Keys::W)) return  1.f;
        if (Keys::isKeyPressed(Keys::Down) || Keys::isKeyPressed(Keys::S)) return -1.f;

        return 0.f;
    }
}

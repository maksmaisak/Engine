//
// Created by Maksym Maisak on 2019-03-01.
//

#ifndef SAXION_Y2Q2_RENDERING_SOUND_H
#define SAXION_Y2Q2_RENDERING_SOUND_H

#include <memory>
#include <SFML/Audio.hpp>

namespace en {

    class LuaState;

    class Sound {

    public:
        Sound(const std::shared_ptr<sf::SoundBuffer>& buffer);
        Sound(const std::string& filepath);
        virtual ~Sound() = default;

        bool isValid() const;
        sf::Sound& getUnderlyingSound();

        static void initializeMetatable(LuaState& lua);

    private:
        std::shared_ptr<sf::SoundBuffer> m_buffer;
        sf::Sound m_sound;
    };
}

#endif //SAXION_Y2Q2_RENDERING_SOUND_H

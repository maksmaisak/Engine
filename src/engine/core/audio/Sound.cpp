//
// Created by Maksym Maisak on 2019-03-01.
//

#include "Sound.h"
#include <cassert>
#include "Resources.h"
#include "LuaState.h"
#include "LuaStack.h"
#include "MetatableHelper.h"

using namespace en;

Sound::Sound(const std::shared_ptr<sf::SoundBuffer>& buffer) : m_buffer(buffer) {

    if (m_buffer) {
        m_sound = sf::Sound(*m_buffer);
    }
}

Sound::Sound(const std::string& filepath) : Sound(Resources<sf::SoundBuffer>::get(filepath)) {}

sf::Sound& Sound::getUnderlyingSound() {
    return m_sound;
}

bool Sound::isValid() const {
    return m_buffer.get();
}

void Sound::initializeMetatable(LuaState& lua) {

    lua.setField("play" , [](const std::shared_ptr<Sound>& sound) {sound->getUnderlyingSound().play(); });
    lua.setField("pause", [](const std::shared_ptr<Sound>& sound) {sound->getUnderlyingSound().pause();});
    lua.setField("stop" , [](const std::shared_ptr<Sound>& sound) {sound->getUnderlyingSound().stop(); });

    lua::addProperty(lua, "volume", lua::property(
        [](const std::shared_ptr<Sound>& sound) {return sound->getUnderlyingSound().getVolume();},
        [](const std::shared_ptr<Sound>& sound, float volume) {sound->getUnderlyingSound().setVolume(volume);}
    ));

    lua::addProperty(lua, "pitch", lua::property(
        [](const std::shared_ptr<Sound>& sound) {return sound->getUnderlyingSound().getPitch();},
        [](const std::shared_ptr<Sound>& sound, float pitch) {sound->getUnderlyingSound().setPitch(pitch);}
    ));

    lua::addProperty(lua, "playingOffset", lua::property(
        [](const std::shared_ptr<Sound>& sound) {return sound->getUnderlyingSound().getPlayingOffset().asSeconds();},
        [](const std::shared_ptr<Sound>& sound, float offset) {sound->getUnderlyingSound().setPlayingOffset(sf::seconds(offset));}
    ));

    lua::addProperty(lua, "loop", lua::property(
        [](const std::shared_ptr<Sound>& sound) {return sound->getUnderlyingSound().getLoop();},
        [](const std::shared_ptr<Sound>& sound, bool loop) {sound->getUnderlyingSound().setLoop(loop);}
    ));

    lua::addProperty(lua, "duration", lua::readonlyProperty([](const std::shared_ptr<Sound>& sound) {
        return sound->getUnderlyingSound().getBuffer()->getDuration().asSeconds();
    }));

    lua::addProperty(lua, "status", lua::readonlyProperty(
        [](const std::shared_ptr<Sound>& sound) {
            sf::Sound::Status status = sound->getUnderlyingSound().getStatus();
            switch (status) {
                case sf::Sound::Status::Playing:
                    return "Playing";
                case sf::Sound::Status::Paused:
                    return "Paused";
                case sf::Sound::Status::Stopped:
                    return "Stopped";
                default:
                    return "INVALID_SOUND_STATUS";
            }
        }
    ));
}

namespace en {

    template<>
    struct ResourceLoader<sf::SoundBuffer> {

        inline static std::shared_ptr<sf::SoundBuffer> load(const std::string& filepath) {

            auto buffer = std::make_shared<sf::SoundBuffer>();
            if (!buffer->loadFromFile(filepath)) {
                std::cerr << "Couldn't load sound from: " << filepath << std::endl;
                return nullptr;
            }

            return buffer;
        }
    };
}

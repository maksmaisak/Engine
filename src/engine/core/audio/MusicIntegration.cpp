//
// Created by Maksym Maisak on 2019-03-04.
//

#include "MusicIntegration.h"
#include <memory>

using namespace en;

std::shared_ptr<sf::Music> ResourceLoader<sf::Music>::load(const Name& filepath) {

    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile(filepath)) {
        return nullptr;
    }

    return music;
}

void lua::InitializeMetatable<sf::Music>::initializeMetatable(en::LuaState& lua) {

    lua.setField("play" , [](const std::shared_ptr<sf::Music>& music) {music->play(); });
    lua.setField("pause", [](const std::shared_ptr<sf::Music>& music) {music->pause();});
    lua.setField("stop" , [](const std::shared_ptr<sf::Music>& music) {music->stop(); });

    lua::addProperty(lua, "volume", lua::property(
        [](const std::shared_ptr<sf::Music>& music) {return music->getVolume();},
        [](const std::shared_ptr<sf::Music>& music, float volume) {music->setVolume(volume);}
    ));

    lua::addProperty(lua, "pitch", lua::property(
        [](const std::shared_ptr<sf::Music>& music) {return music->getPitch();},
        [](const std::shared_ptr<sf::Music>& music, float pitch) {music->setPitch(pitch);}
    ));

    lua::addProperty(lua, "playingOffset", lua::property(
        [](const std::shared_ptr<sf::Music>& music) {return music->getPlayingOffset().asSeconds();},
        [](const std::shared_ptr<sf::Music>& music, float offset) {music->setPlayingOffset(sf::seconds(offset));}
    ));

    lua::addProperty(lua, "loop", lua::property(
        [](const std::shared_ptr<sf::Music>& music) {return music->getLoop();},
        [](const std::shared_ptr<sf::Music>& music, bool loop) {music->setLoop(loop);}
    ));

    lua::addProperty(lua, "duration", lua::readonlyProperty([](const std::shared_ptr<sf::Music>& music) {
        return music->getDuration().asSeconds();
    }));

    lua::addProperty(lua, "status", lua::readonlyProperty(
        [](const std::shared_ptr<sf::Music>& music) {
            sf::Sound::Status status = music->getStatus();
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
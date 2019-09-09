//
// Created by Maksym Maisak on 2019-04-21.
//

#ifndef ENGINE_TESTCOMPONENTPOOL_H
#define ENGINE_TESTCOMPONENTPOOL_H

#include <vector>
#include <chrono>
#include "ComponentPool.h"
#include "Transform.h"

/// Some benchmark functions to test the performance of the data-oriented EntityRegistry versus your regular heap-allocated stuff.

template<typename F, typename... Args>
inline std::chrono::duration<float, std::milli> measureTime(F&& f, Args&&... args) {

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    f(args...);
    auto delta = clock::now() - start;
    return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(delta);
}

inline void testIterationEntityRegistry() {

    struct Data {std::byte data[sizeof(en::Transform)];};
    en::EntityRegistry registry;
    for (std::size_t i = 0; i < 1000000ul; ++i)
        registry.add<Data>(registry.makeEntity(), Data{});

    std::size_t count = 0;
    auto time = measureTime([&registry, &count](){

        std::size_t counter = 0;
        for (en::Entity e : registry.with<Data>()) {
            const Data& data = registry.get<Data>(e);
            ++counter;
        }
        count = counter;
    });
    std::cout << "Iterated through " << count << " Entities with Data: " << time.count() << "ms\n";
}

inline void testIterationHeapAllocated() {

    struct Component {
        virtual ~Component() = default;
    };

    struct Data : Component {
        std::byte data[sizeof(en::Transform)];
    };

    struct GameObject {
        std::vector<std::unique_ptr<Component>> components;
    };

    std::vector<std::unique_ptr<GameObject>> gameObjects {};
    for (std::size_t i = 0; i < 1000000ul; ++i) {
        auto gameObject = std::make_unique<GameObject>();
        gameObject->components.push_back(std::make_unique<Data>());
        gameObjects.push_back(std::move(gameObject));
    }

    std::size_t count = 0;
    auto time = measureTime([&gameObjects, &count]() {

        std::size_t counter = 0;
        for (const auto& gameObjectPtr : gameObjects)
            for (const auto& componentPtr : gameObjectPtr->components)
                if (dynamic_cast<Data*>(componentPtr.get()) != nullptr)
                    ++counter;
        count = counter;
    });
    std::cout << "Iterated through " << count << " GameObjects with Data: " << time.count() << "ms\n";
}

#endif //ENGINE_TESTCOMPONENTPOOL_H

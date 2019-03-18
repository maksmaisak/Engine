//
// Created by Maksym Maisak on 4/11/18.
//

#ifndef SAXION_Y2Q1_CPP_TRANSFORMABLEHIERARCHYSYSTEM_H
#define SAXION_Y2Q1_CPP_TRANSFORMABLEHIERARCHYSYSTEM_H

#include "Engine.h"
#include "Messaging.h"
#include "EntityEvents.h"
#include "Transform.h"

namespace en {

    /// Performs some initialization of en::Transform when they're added.
    /// Destroys their child entities when they're removed.
    class TransformHierarchySystem : public System,
        Receiver<ComponentAdded<Transform>>,
        Receiver<ComponentWillBeRemoved<Transform>>
    {
        void receive(const ComponentAdded<Transform>& info) override;
        void receive(const ComponentWillBeRemoved<Transform>& info) override;
    };
}

#endif //SAXION_Y2Q1_CPP_TRANSFORMABLEHIERARCHYSYSTEM_H

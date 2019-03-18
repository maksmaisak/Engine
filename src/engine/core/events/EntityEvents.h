//
// Created by Maksym Maisak on 24/10/18.
//

#ifndef SAXION_Y2Q1_CPP_ENTITYEVENTS_H
#define SAXION_Y2Q1_CPP_ENTITYEVENTS_H

#include "Entity.h"

namespace en {

    struct EntityCreated {
        en::Entity entity;
    };

    struct EntityWillBeDestroyed {
        en::Entity entity;
    };

    template<typename TComponent>
    struct ComponentAdded {
        en::Entity entity;
        TComponent& component;
    };

    template<typename TComponent>
    struct ComponentWillBeRemoved {
        en::Entity entity;
        TComponent& component;
    };
}

#endif //SAXION_Y2Q1_CPP_ENTITYEVENTS_H

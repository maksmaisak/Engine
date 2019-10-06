//
// Created by Maksym Maisak on 2019-02-14.
//

#ifndef ENGINE_UIEVENTSYSTEM_H
#define ENGINE_UIEVENTSYSTEM_H

#include "System.h"
#include "Entity.h"
#include "glm.h"

namespace en {

    class Transform;
	struct UIRect;

    /// Dispatches mouse events to UIRects
    class UIEventSystem : public System {

    public:
        void update(float dt) override;

    private:
        void updateRect(Entity e, UIRect& rect, Transform& transform, const glm::vec2& mousePosition);
    };
}

#endif //ENGINE_UIEVENTSYSTEM_H

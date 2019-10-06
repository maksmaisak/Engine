//
// Created by Maksym Maisak on 2019-02-14.
//

#ifndef ENGINE_UIEVENTS_H
#define ENGINE_UIEVENTS_H

namespace en {

    struct UIEvent {
        Entity entity;
    };

    struct MouseButtonEvent : UIEvent {
        int button;
    };

    struct MouseEnter : public UIEvent {};
    struct MouseOver  : public UIEvent {};
    struct MouseLeave : public UIEvent {};

    struct MouseDown : MouseButtonEvent {};
    struct MouseHold : MouseButtonEvent {};
    struct MouseUp   : MouseButtonEvent {};
}

#endif //ENGINE_UIEVENTS_H

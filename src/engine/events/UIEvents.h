//
// Created by Maksym Maisak on 2019-02-14.
//

#ifndef SAXION_Y2Q2_RENDERING_UIEVENTS_H
#define SAXION_Y2Q2_RENDERING_UIEVENTS_H

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
    struct MouseHold  : MouseButtonEvent {};
    struct MouseUp    : MouseButtonEvent {};
}

#endif //SAXION_Y2Q2_RENDERING_UIEVENTS_H

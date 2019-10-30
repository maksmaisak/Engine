//
// Created by Maksym Maisak on 30/10/19.
//

#ifndef ENGINE_AICONTROLLERDIAGNOSTICSSYSTEM_H
#define ENGINE_AICONTROLLERDIAGNOSTICSSYSTEM_H

#include "System.h"

namespace en {
    class Actor;
}

/// Shows diagnostics on the currently selected AIController
class AIControllerDiagnosticsSystem : public en::System {

public:
    void draw() override;

private:
    void showAIController(const en::Actor& actor);
};

#endif //ENGINE_AICONTROLLERDIAGNOSTICSSYSTEM_H
